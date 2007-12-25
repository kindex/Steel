/*id*********************************************************
	File: engine/engine.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс Object - прототип класса, от которого должны наследоваться 
		классы, которые должны обрабатываться классом Engine.
		Класс Engine (движок) через функцию inject() получает объекты, которые
		должны быть обработаны, но обрабатывает их только внутри process().
		Для этого объекты должны поддерживать интервейс (interface) обмена 
		информацией с движком. От этого интерфейса наследуется Engine.
 ************************************************************/
#include "../steel.h"
#include "engine.h"
#include "game_object.h"
#include "../common/logger.h"
#include "engine_types.h"
#include "../common/utils.h"

void Shadow::fill(GameObject* object)
{
	this->object = object;
	objectId = object->getObjectId();
//	modificationTime = childrenModificationTime = -1;
}


bool Engine::makeShadowForObject(GameObject* object, const InterfaceId id)
{
	uid objectId = object->getObjectId();
	if (idHash[id].find(objectId) != idHash[id].end())
	{
		log_msg("error engine", "Duplicate object " + IntToStr(objectId) + " in shadow");
		return false;
	}

	Shadow* newStorage = shadowClassFactory(object, id);
	if (newStorage == NULL)
	{
		log_msg("error engine", "Cannot find shadow for object " + IntToStr(objectId));
		return false;
	}

	int shadowIndex = addShadow(newStorage, id);

	idHash[id][objectId] = shadowIndex;
	newStorage->shadowIndex = shadowIndex;
	newStorage->fill(object);
	makeShadowForObjectPost(object, newStorage);
	return true;
}

int Engine::addShadow(Shadow* newStorage, const InterfaceId)
{
	shadows.push_back(newStorage);
    return shadows.size() - 1; // index
}

void Engine::deleteShadowForObject(int sid, const InterfaceId id)
{
	deleteShadowForObjectPost(sid);

	Shadow *shadow = shadows[sid];
	idHash[id].erase(shadow->objectId);
	delete shadow;

	if(size_t(sid + 1) < shadows.size())
	{
		shadows[sid] = shadows.back();
		idHash[id][shadows[sid]->objectId] = sid;
	}
	shadows.pop_back();
}

void Engine::deleteShadowForChildren(int sid, const InterfaceId id)
{
	int count = shadows[sid]->children.size();
	for(int i = 0; i < count; i++)
	{
		int n = findSid(shadows[sid]->children[i], id);
		deleteShadowForChildren(n, id);
		deleteShadowForObject(n, id);
	}
}

Shadow* Engine::getShadow(GameObject* object, const InterfaceId iid)
{
	uid id = object->getObjectId();
    int sid = findSid(id, iid);

	if (sid >= 0)
    {
		return shadows[sid];
    }
	else
    {
		return NULL;
    }
}

Shadow* Engine::getShadow(uid id, const InterfaceId iid)
{
    int sid = findSid(id, iid);
	if (sid >= 0)
    {
		return shadows[sid];
    }
	else
    {
		return NULL;
    }
}

void Camera::set(const v3 &_position, const v3& _direction, const v3& _upVector)
{
	v3 eye = _position;
	v3 up = _upVector;
	v3 dir = _direction;
	v3 right = dir.crossProduct(up).getNormalized();
	up = -dir.crossProduct(right);

	position	= _position;
	direction	= _direction.getNormalized();
	upVector	= up;
}

void Camera::setUpVector(const v3& _upVector)
{ 
	upVector = _upVector.getNormalized();
}
