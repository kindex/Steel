/*id*********************************************************
	File: physic/physic_engine.cpp
	Unit: physic engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Абстрактный физический движок
		Обрабатывает движение объектов.
		Поддерживает ирерархию объектов, кеширует информацию об	объектах.
************************************************************/

#include "../steel.h"
#include "physic_engine.h"
#include "../res/res_main.h"

bool PhysicEngine::init(Config* _conf)
{
	conf = _conf;
	if(conf == NULL)
	{
		log_msg("error graph conf res","Cannot find renderer config file");
		return false;
	}

	total.collisionCount = 0;

	return true;
}

bool PhysicEngine::inject(PhysicObject *object)
{
	// если объект не хочет добавляться
	if(!object->PhysicBeforeInject()) return false;
	// кешируем объект
	if(!makeStorageForObject(object)) return false;
	makeStorageForChildren(object);
	// список глобальных объектов
	objects.push_back(object);

	return true;
}


bool PhysicEngine::clear()
{
	while(!objects.empty())
	{
		remove(objects.back());
	}

	return true;
}

bool PhysicEngine::remove(PhysicObject *object)
{
	deleteStorageForChildren(idHash[object->getId()]);
	deleteStorageForObject(idHash[object->getId()]);
	
	for(steel::vector<PhysicObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		if(*it == object)
		{
			objects.erase(it);
			break;
		}
	object->PhysicAfterRemove();

	return true;
}