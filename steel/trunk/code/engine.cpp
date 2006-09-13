/*id*********************************************************
	File: engine.h
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс Interface - прототип класса, от которого должны наследоваться 
		классы, которые должны обрабатываться классом Engine.
		Класс Engine (движок) через функцию inject() получает объекты, которые
		должны быть обработаны, но обрабатывает их только внутри process().
		Для этого объекты должны поддерживать интервейс (interface) обмена 
		информацией с движком.
 ************************************************************/
#include "steel.h"
#include "engine.h"

void Engine::Storage::fill(Interface *object)
{
	this->object = object;
	objectId = object->getId();
	modificationTime = childrenModificationTime = -1;
}

void Engine::Storage::cache()
{
	modificationTime = object->getModificationTime();
}


bool Engine::makeStorageForObject(Interface *object)
{
	uid objectId = object->getId();
	if(idHash.find(objectId) != idHash.end())
	{
		log_msg("error engine", "Duplicate object " + IntToStr(objectId) + " in storage");
		return false;
	}

	int storageIndex = storages.size();
	
	Storage* newStorage = getStorageClass(object);
	if(newStorage == NULL)
	{
		log_msg("error engine", "Cannot find storage for object " + IntToStr(objectId));
		return false;
	}

	storages.push_back(newStorage);

	idHash[objectId] = storageIndex;
	newStorage->storageIndex = storageIndex;
	newStorage->fill(object);
	makeStorageForObjectPost(object, newStorage);
	return true;
}

void Engine::deleteStorageForObject(int sid)
{
	deleteStorageForObjectPost(sid);

	Storage *storage = storages[sid];
	idHash.erase(storage->objectId);
	delete storage;

	if(sid + 1 < storages.size())
	{
		storages[sid] = storages.back();
		idHash[storages[sid]->objectId] = sid;
	}
	storages.pop_back();
}

void Engine::deleteStorageForChildren(int sid)
{
	int count = storages[sid]->children.size();
	for(int i = 0; i < count; i++)
	{
		int n = findSid(storages[sid]->children[i]);
		deleteStorageForChildren(n);
		deleteStorageForObject(n);
	}
}
