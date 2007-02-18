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
		�������������� graph_interface ��� OpenGL_Engine
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"

bool OpenGL_Engine::isSupportingInterface(IN const InterfaceId id)
{
	return (id & (INTERFACE_GRAPH)) == id;
}

void OpenGL_Engine::addLight(Light* light)
{
	LightShadow *shadow = new LightShadow();
	shadow->light = light;
	shadow->shadow = currentShadow;
	shadow->object = currentShadow->object;
	shadow->changed = true;

	lights[light->id] = shadow;
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
	uid childUid = child->getId();
	uidVector::const_iterator it = find(shadow.children, childUid);
	if(it != currentShadow->children.end()) return ; // child have been added before
	if(!child->beforeInject(*this)) return; // shild don't want to be added
	if(!makeShadowForObject(child)) return;
	shadow.children.push_back(childUid);
	setCurrentObject(child);
	child->bindEngine(*this);
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

void OpenGL_Engine::setVertexes(const Vertexes* vertexes) // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
{
	GS(currentShadow)->vertexes = vertexes;
	GS(currentShadow)->aabbCalculated = false;
}

void OpenGL_Engine::setNormals(const Normals* normals) // ������ �������� � ��������
{
	GS(currentShadow)->normals = normals;
}
void OpenGL_Engine::setLines(const GLines* lines) // ������� ������ ��� ����� � ����� ����� (for debug)
{
	GS(currentShadow)->lines = lines;
}

void OpenGL_Engine::setFaceMaterials(const FaceMaterialVector* faceMaterials)// ������ �������� ������, ������� �������� ������������ (�����) + ���������
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

const ProcessInfo& OpenGL_Engine::getProcessInfo()
{
	return info;
}
