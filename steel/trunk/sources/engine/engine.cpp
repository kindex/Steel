/*id*********************************************************
	File: engine/engine.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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

void Engine::Shadow::fill(GameObject *object)
{
	this->object = object;
	objectId = object->getId();
//	modificationTime = childrenModificationTime = -1;
}

bool Engine::Shadow::cache()
{
/*	ModificationTime newTime = object->getModificationTime();
	if(modificationTime < newTime)
	{
		modificationTime = newTime;
		return true;
	}
	else*/
		return false;
}


bool Engine::makeShadowForObject(GameObject *object)
{
	uid objectId = object->getId();
	if(idHash.find(objectId) != idHash.end())
	{
		log_msg("error engine", "Duplicate object " + IntToStr(objectId) + " in shadow");
		return false;
	}

	int shadowIndex = shadows.size();
	
	Shadow* newStorage = getShadowClass(object);
	if(newStorage == NULL)
	{
		log_msg("error engine", "Cannot find shadow for object " + IntToStr(objectId));
		return false;
	}

	shadows.push_back(newStorage);

	idHash[objectId] = shadowIndex;
	newStorage->shadowIndex = shadowIndex;
	newStorage->fill(object);
	makeShadowForObjectPost(object, newStorage);
	return true;
}

void Engine::deleteShadowForObject(int sid)
{
	deleteShadowForObjectPost(sid);

	Shadow *shadow = shadows[sid];
	idHash.erase(shadow->objectId);
	delete shadow;

	if(size_t(sid + 1) < shadows.size())
	{
		shadows[sid] = shadows.back();
		idHash[shadows[sid]->objectId] = sid;
	}
	shadows.pop_back();
}

void Engine::deleteShadowForChildren(int sid)
{
	int count = shadows[sid]->children.size();
	for(int i = 0; i < count; i++)
	{
		int n = findSid(shadows[sid]->children[i]);
		deleteShadowForChildren(n);
		deleteShadowForObject(n);
	}
}

Engine::Shadow* Engine::getShadow(GameObject *object)
{
	uid id = object->getId();
//	assert(idHash.find(id) != idHash.end(), "Object not found in physic shadow");

	if(idHash.find(id) != idHash.end())
		return shadows[findSid(id)];
	else
		return NULL;
}

Engine::Shadow* Engine::getShadow(uid id)
{
	if(idHash.find(id) != idHash.end())
		return shadows[findSid(id)];
	else
		return NULL;
}
