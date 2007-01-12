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

void OpenGL_Engine::addLight(Light* light)
{
	if(currentShadow != NULL)
	{
		LightShadow *shadow = new LightShadow();
		shadow->light = light;
		shadow->shadow = currentShadow;
		shadow->object = currentShadow->object;
		shadow->changed = true;

		lights[light->id] = shadow;
	}
}

void OpenGL_Engine::removeLight(uid id)
{
	map<uid, LightShadow*>::iterator it = lights.find(id);
	delete it->second;
	lights.erase(it);
}

void OpenGL_Engine::updateLight(uid id, Light* light)
{
	map<uid, LightShadow*>::iterator it = lights.find(id);
	it->second->light = light;
	it->second->changed = true;
}

bool OpenGL_Engine::setCurrentObject(GameObject* object)
{
	uid id = object->getId();
	currentShadow = getShadow(id);
	if(currentShadow == NULL)
	{
		return false;
	}

	return false;
}

void OpenGL_Engine::setPosition(ObjectPosition position)
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->position = position;
	}
}

void OpenGL_Engine::setPositionKind(PositionKind kind)
{
	if(currentShadow != NULL)
	{
		currentShadow->positionKind = kind;
	}
}


void OpenGL_Engine::addChild(GameObject* child)
{
	if(currentShadow != NULL)
	{
		addChild(*currentShadow, child);
	}
}

void OpenGL_Engine::addChild(GraphShadow &shadow, GameObject *child)
{
	uid childUid = child->getId();
	
	uidVector::const_iterator it = find(shadow.children, childUid);

	if(it != currentShadow->children.end()) return ; // child have been added before

	if(!child->beforeInject(this->interfaceId)) return; // shild don't want to be added

	if(!makeShadowForObject(child)) return;

	shadow.children.push_back(childUid);

	setCurrentObject(child);

	child->bindEngine(this->interfaceId, this);

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

void OpenGL_Engine::setVertexes(const Vertexes* vertexes) // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->vertexes = vertexes;
		GS(currentShadow)->aabbCalculated = false;
	}
}

void OpenGL_Engine::setNormals(const Normals* normals) // список нормалей в вершинам
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->normals = normals;
	}
}
void OpenGL_Engine::setLines(const GLines* lines) // индексы вершин дл€ линий и цвета линий (for debug)
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->lines = lines;
	}
}

void OpenGL_Engine::setFaceMaterials(const FaceMaterials* faceMaterials)// массив индексов вершин, которые образуют треугольники (грани) + материалы
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->faceMaterials = faceMaterials;
	}
}

void OpenGL_Engine::setTexCoordsCount(unsigned int size)
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->texCoords.resize(size);
	}
}

void OpenGL_Engine::setTexCoords(unsigned int texNumber, const TexCoords* coords)
{
	if(currentShadow != NULL && static_cast<int>(GS(currentShadow)->texCoords.size()) > texNumber)
	{
		GS(currentShadow)->texCoords[texNumber] = coords;
	}
}

void OpenGL_Engine::setAABB(const AABB &aabb)
{
	if(currentShadow != NULL)
	{
		GS(currentShadow)->aabb = aabb;
		GS(currentShadow)->aabbCalculated = true;
	}
}
