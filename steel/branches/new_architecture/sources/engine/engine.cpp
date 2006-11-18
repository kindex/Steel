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


bool Engine::makeStorageForObject(GameObject *object)
{
	uid objectId = object->getId();
	if(idHash.find(objectId) != idHash.end())
	{
		log_msg("error engine", "Duplicate object " + IntToStr(objectId) + " in storage");
		return false;
	}

	int storageIndex = shadows.size();
	
	Shadow* newStorage = getStorageClass(object);
	if(newStorage == NULL)
	{
		log_msg("error engine", "Cannot find storage for object " + IntToStr(objectId));
		return false;
	}

	shadows.push_back(newStorage);

	idHash[objectId] = storageIndex;
	newStorage->storageIndex = storageIndex;
	newStorage->fill(object);
	makeStorageForObjectPost(object, newStorage);
	return true;
}

void Engine::deleteStorageForObject(int sid)
{
	deleteStorageForObjectPost(sid);

	Shadow *storage = shadows[sid];
	idHash.erase(storage->objectId);
	delete storage;

	if(size_t(sid + 1) < shadows.size())
	{
		shadows[sid] = shadows.back();
		idHash[shadows[sid]->objectId] = sid;
	}
	shadows.pop_back();
}

void Engine::deleteStorageForChildren(int sid)
{
	int count = shadows[sid]->children.size();
	for(int i = 0; i < count; i++)
	{
		int n = findSid(shadows[sid]->children[i]);
		deleteStorageForChildren(n);
		deleteStorageForObject(n);
	}
}

Engine::Shadow* Engine::getStorage(GameObject *object)
{
	uid id = object->getId();
//	assert(idHash.find(id) != idHash.end(), "Object not found in physic storage");

	if(idHash.find(id) != idHash.end())
		return shadows[findSid(id)];
	else
		return NULL;
}

Engine::Shadow* Engine::getStorage(uid id)
{
	if(idHash.find(id) != idHash.end())
		return shadows[findSid(id)];
	else
		return NULL;
}
