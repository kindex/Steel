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

PhysicEngineSteel::PhysicStorage* PhysicEngineSteel::getStorage(PhysicObject *object)
{
	uid id = object->getId();
	assert(idHash.find(id) != idHash.end(), "Object not found in physic storage");

	return (PhysicStorage*)storages[idHash[id]];
}



void PhysicEngineSteel::PhysicStorage::fill(Interface *object)
{
	Storage::fill(object);

	collisionType = ((PhysicObject*)object)->getCollisionType();
	force.loadZero();
}

void PhysicEngineSteel::makeStorageForObjectPost(Interface *object, Storage *storage)
{
	if(((PhysicStorage*)storage)->collisionType == COLLISION_PARTICLE)
	{
		((PhysicStorage*)storage)->partiecleSetId = particleSet.size();
		particleSet.push_back(storage->storageIndex);
	}
}

void PhysicEngineSteel::deleteStorageForObjectPost(int sid)
{
	if(((PhysicStorage*)storages[sid])->collisionType == COLLISION_PARTICLE)
	{
		((PhysicStorage*)storages[particleSet.back()])->partiecleSetId = 
			((PhysicStorage*)storages[sid])->partiecleSetId;
		particleSet[((PhysicStorage*)storages[sid])->partiecleSetId] = particleSet.back();
		particleSet.pop_back();
	}
}




void PhysicEngineSteel::makeStorageForChildren(Interface *object)
{
	int count = ((PhysicObject*)object)->getPhysicChildrenCount();
	for(int i = 0; i < count; i++)
	{
		PhysicObject *child = ((PhysicObject*)object)->getPhysicChildren(i);
		makeStorageForObject(child);
		makeStorageForChildren(child);
	}
}

void PhysicEngineSteel::PhysicStorage::cache()
{
	Storage::cache();
	PhysicObject *pobject = (PhysicObject*)object;

	if(modificationTime < pobject->getModificationTime())
	{
		position = pobject->getPosition().getTranslation();
		mass = pobject->getMass();

		material = pobject->getPMaterial();

		if(collisionType == COLLISION_PARTICLE)
		{
			spring_r0			= material->getf("spring_r0");
			spring_k			= material->getf("spring_k");
			gravity_k			= material->getf("gravity_k");
			gravity_power		= material->getf("gravity_power");
			gravity_min_dist	= material->getf("gravity_min_dist");
			friction_k			= material->getf("friction_k");
			friction_power		= material->getf("friction_power");
		}
	}
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
			getStorage(child)->cache();
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
		processParticle(*(PhysicStorage*)storages[*it], globalTime, time);
	}
	

	for(svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		PhysicStorage &objectStorage = *(PhysicStorage*)storages[*it];

		objectStorage.velocity += time*objectStorage.force/objectStorage.mass;
		objectStorage.force += objectStorage.velocity.getNormalized() * objectStorage.velocity.getNormalized();
		v3 frictionForce  = 
			-objectStorage.velocity.getNormalized() * pow(objectStorage.velocity.getLength(), objectStorage.friction_power)*objectStorage.friction_k;

		v3 newVelocity = objectStorage.velocity + time*frictionForce/objectStorage.mass;

		if((newVelocity & objectStorage.velocity) >0) // сила трения не может развернуть тело
				objectStorage.velocity = newVelocity;
		else
			objectStorage.velocity.loadZero();


		objectStorage.position += objectStorage.velocity*time;

		PhysicObject &object = *(PhysicObject*)objectStorage.object;

		ObjectPosition objectPosition = object.getPosition();
		objectPosition.setTranslation(objectStorage.position);
		object.setPosition(objectPosition);

		velocity vel;
		vel.translation = objectStorage.velocity;
		object.setVelocity(vel);
	}

	return true;
}

