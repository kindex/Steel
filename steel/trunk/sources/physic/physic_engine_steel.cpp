/*id*********************************************************
	File: physic/physic_engine_steel.cpp
	Unit: steel physic engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Steel Версия физического движка. 
		Обрабатывает движение частиц, полигональных тел и шаров.
		Поддерживает ирерархию объектов, кеширует информацию об	объектах.
************************************************************/
#include "../steel.h"
#include "physic_engine_steel.h"


/////////////////////////////////// PHYSIC CORE ////////////////////////////////////

void PhysicEngineSteel::PhysicStorage::fill(Object *object)
{
	Storage::fill(object);

	collisionType = ((PhysicObject*)object)->getCollisionType();
}

void PhysicEngineSteel::makeStorageForObjectPost(Object *object, Storage *storage)
{
	PhysicEngine::makeStorageForObjectPost(object, storage);
	if(((PhysicStorage*)storage)->collisionType == COLLISION_PARTICLE)
	{
		((PhysicStorage*)storage)->partiecleSetId = particleSet.size();
		particleSet.push_back(storage->storageIndex);
	}
}

void PhysicEngineSteel::deleteStorageForObjectPost(int sid)
{
	PhysicStorage *storage = (PhysicStorage*)storages[sid];
	if(storage)
	if(storage->collisionType == COLLISION_PARTICLE)
	{
		((PhysicStorage*)storages[particleSet.back()])->partiecleSetId = 
			((PhysicStorage*)storages[sid])->partiecleSetId;
		particleSet[((PhysicStorage*)storages[sid])->partiecleSetId] = particleSet.back();
		particleSet.pop_back();
	}
}




void PhysicEngineSteel::makeStorageForChildren(Object *object)
{
	int count = ((PhysicObject*)object)->getPhysicChildrenCount();
	for(int i = 0; i < count; i++)
	{
		PhysicObject *child = ((PhysicObject*)object)->getPhysicChildren(i);
		makeStorageForObject(child);
		makeStorageForChildren(child);
	}
}

bool PhysicEngineSteel::PhysicStorage::cache()
{
	position = ((PhysicObject*)object)->getPosition().getTranslation();
	if(Storage::cache())
	{
		mass = ((PhysicObject*)object)->getMass();
		material = ((PhysicObject*)object)->getPMaterial();
		return true;
	}
	else
		return false;
}



void PhysicEngineSteel::prepare(PhysicObject *object, steel::time globalTime, steel::time time, matrix34 matrix, PhysicObject *parent)
{
	object->ProcessPhysic(globalTime, time, globalFrameNumber);

	int sid = idHash[object->getId()];
		
	storages[sid]->cache();

	if(storages[sid]->childrenModificationTime < object->getChildrenModificationTime())
	{
		storages[sid]->childrenModificationTime = object->getChildrenModificationTime();

		StorageHash newChildrenId;

		int count = object->getPhysicChildrenCount();
		for(int i = 0; i < count; i++) // add new + cache
		{
			PhysicObject *child = object->getPhysicChildren(i);
			uid id = child->getId();
			newChildrenId[id] = i;

			if(idHash.find(id) == idHash.end())
			{
				// add new object to storage
				makeStorageForObject(child);
				makeStorageForChildren(child);

				storages[sid]->children.push_back(id);
			}
			Storage *storage = getStorage(child);
			if(storage != NULL)
				storage->cache();
		}
		int size = storages[sid]->children.size();

		for(int i = 0; i<size; i++)
		{
			uid id = storages[sid]->children[i];
			if(newChildrenId.find(id) == newChildrenId.end())
			{
				int n = idHash[id];
				deleteStorageForChildren(n);
				deleteStorageForObject(n);
				storages[sid]->children[i] = storages[sid]->children.back();
				storages[sid]->children.pop_back();

				size--;
				i--;
			}
		}
	}


	int count = object->getPhysicChildrenCount();
	for(int i = 0; i < count; i++)
	{
		PhysicObject *child = object->getPhysicChildren(i);
		
		prepare(child, globalTime, time);
	}
}


bool PhysicEngineSteel::process(steel::time globalTime, steel::time time)
{
	helperDrawLines = helper && conf->geti("helperDrawLines");
	if(helper) // draw velocity
		helper->setTime(globalTime);

	int size = objects.size();
	for(int i=0; i < size; i++)
		prepare(objects[i], globalTime, time);

	for(svector<Storage*>::iterator it = storages.begin(); it != storages.end(); it++)
	{
		PhysicStorage *objectStorage = (PhysicStorage*)(*it);
		objectStorage->velocity = ((PhysicObject*)objectStorage->object)->getVelocity().translation;
		objectStorage->force.loadZero();
	}

	for(svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		processParticle((PhysicStorageParticle*)storages[*it], globalTime, time);
	}
	

	for(svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		PhysicStorageParticle *objectStorage = (PhysicStorageParticle*)storages[*it];

		objectStorage->velocity += time*objectStorage->force/objectStorage->mass;
		objectStorage->force += objectStorage->velocity.getNormalized() * objectStorage->velocity.getNormalized();
		v3 frictionForce  = 
			-objectStorage->velocity.getNormalized() * pow(objectStorage->velocity.getLength(), objectStorage->friction_power)*objectStorage->friction_k;

		v3 newVelocity = objectStorage->velocity + time*frictionForce/objectStorage->mass;

		if((newVelocity & objectStorage->velocity) >0) // сила трения не может развернуть тело
				objectStorage->velocity = newVelocity;
		else
			objectStorage->velocity.loadZero();


		objectStorage->position += objectStorage->velocity*time;

		PhysicObject &object = *(PhysicObject*)objectStorage->object;

		ObjectPosition objectPosition = object.getPosition();
		objectPosition.setTranslation(objectStorage->position);
		object.setPosition(objectPosition);

		velocity vel;
		vel.translation = objectStorage->velocity;
		object.setVelocity(vel);
	}

	return true;
}


PhysicEngineSteel::Storage* PhysicEngineSteel::getStorageClass(Object *object)
{ 
	if(((PhysicObject*)object)->getCollisionType() == COLLISION_PARTICLE)
		return new PhysicStorageParticle(this);
	else
		return new PhysicStorage(this);
}



