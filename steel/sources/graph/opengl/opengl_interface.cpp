/*id*********************************************************
	File: graph/opengl/opengl_interface.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		»мплементриует graph_interface дл€ OpenGL_Engine
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "../../engine/game_object.h"

namespace opengl
{

void OpenGL_Engine::addLight(Light* light)
{
	LightShadow* shadow = new LightShadow();
	shadow->light = light;
	shadow->shadow = currentShadow;
	shadow->object = currentShadow->object;
	shadow->changed = true;

	lights[light->id] = shadow;
}

void OpenGL_Engine::removeLight(uid id)
{
	std::map<uid, LightShadow*>::iterator it = lights.find(id);
	delete it->second;
	lights.erase(it);
}

void OpenGL_Engine::updateLight(uid id, Light* light)
{
	std::map<uid, LightShadow*>::iterator it = lights.find(id);
	it->second->light = light;
	it->second->changed = true;
}

bool OpenGL_Engine::setCurrentObject(GameObject* object, const InterfaceId)
{
	uid id = object->getObjectId();
	currentShadow = getShadow(id);
	if (currentShadow == NULL)
	{
		return false;
	}

	return true;
}

void OpenGL_Engine::setPosition(const ObjectPosition& position)
{
	GS(currentShadow)->position = position;
}

void OpenGL_Engine::setPositionKind(PositionKind kind)
{
	currentShadow->positionKind = kind;
}


void OpenGL_Engine::addChild(GameObject* child)
{
	addChild(*currentShadow, child);
}

void OpenGL_Engine::addChild(GraphShadow &shadow, GameObject *child)
{
	uid childUid = child->getObjectId();
	uidVector::const_iterator it = find(shadow.children, childUid);
	if (it != currentShadow->children.end())
    {
        return ; // child have been added before
    }
	if (!child->beforeInject(*this, INTERFACE_GRAPH))
    {
        return; // shild don't want to be added
    }
	if (!makeShadowForObject(child, INTERFACE_GRAPH))
    {
        return;
    }
	shadow.children.push_back(childUid);
	setCurrentObject(child, INTERFACE_GRAPH);
	child->bindEngine(*this, INTERFACE_GRAPH);
	currentShadow = &shadow;
}


void OpenGL_Engine::deleteChild(GameObject* child)
{
	// TODO:
}

void OpenGL_Engine::clearChildren()
{
	// TODO:
}

void OpenGL_Engine::setVertexes(const VertexVector* vertexes) // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
{
	GS(currentShadow)->vertexes = vertexes;
	GS(currentShadow)->aabbCalculated = false;
}

void OpenGL_Engine::setNormals(const Normals* normals) // список нормалей в вершинам
{
	GS(currentShadow)->normals = normals;
}
void OpenGL_Engine::setLines(const GLines* lines) // индексы вершин дл€ линий и цвета линий (for debug)
{
	GS(currentShadow)->lines = lines;
}

void OpenGL_Engine::setFaceMaterials(const FaceMaterialVector* faceMaterials)// массив индексов вершин, которые образуют треугольники (грани) + материалы
{
	GS(currentShadow)->faceMaterials = faceMaterials;
}

void OpenGL_Engine::setTexCoordsCount(unsigned int size)
{
	GS(currentShadow)->texCoords.resize(size);
}

void OpenGL_Engine::setTexCoords(unsigned int texNumber, const TexCoords* coords)
{
	if (static_cast<int>(GS(currentShadow)->texCoords.size()) > texNumber)
	{
		GS(currentShadow)->texCoords[texNumber] = coords;
	}
}

void OpenGL_Engine::setAABB(const AABB &aabb)
{
	GS(currentShadow)->aabb = aabb;
	GS(currentShadow)->aabbCalculated = true;
}

void OpenGL_Engine::setGraphText(const GraphTextVector& text)
{
	GS(currentShadow)->text = text;
}

const ProcessInfo& OpenGL_Engine::getProcessInfo()
{
	return info;
}

} // namespace opengl
