/*id*********************************************************
    Unit: 3D PhysicEngine Steel
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
#include "physic_engine_steel.h"


/////////////////////////////////// PHYSIC CORE ////////////////////////////////////

PhysicEngineSteel::PhysicObjectStorage &PhysicEngineSteel::getStorage(PhysicObject *object)
{
	uid id = object->getId();
	assert(idHash.find(id) != idHash.end(), "Object not found in physic storage");

	return storage[idHash[id]];
}



void PhysicEngineSteel::makeStorageForObject(PhysicObject *object)
{
	uid objectId = object->getId();
	if(idHash.find(objectId) != idHash.end())
	{
		log_msg("error physic", "Duplicate object " + IntToStr(objectId) + " in storage");
		return;
	}

	int storageId = storage.size();
	storage.resize(storageId + 1);

	PhysicObjectStorage &objectStorage = storage[storageId];

	idHash[objectId] = storageId;

	objectStorage.object = object;
	objectStorage.storageId = storageId;
	objectStorage.objectId = objectId;
	objectStorage.collisionType = object->getCollisionType();
	objectStorage.force.loadZero();

	objectStorage.modificationTime = -1;
	objectStorage.childrenModificationTime = -1;

	if(objectStorage.collisionType == CollisionType::particle)
	{
		particleSet.push_back(storageId);
		objectStorage.partiecleSetId = particleSet.size()-1;
	}
}

void PhysicEngineSteel::deleteStorageForObject(int sid)
{
	if(storage[sid].collisionType == CollisionType::particle)
	{
		storage[particleSet.back()].partiecleSetId = storage[sid].partiecleSetId;
		particleSet[storage[sid].partiecleSetId] = particleSet.back();
		particleSet.pop_back();
	}
	idHash.erase(storage[sid].objectId);
	storage[sid] = storage.back();
	idHash[storage[sid].objectId] = sid;
	storage.pop_back();
}

void PhysicEngineSteel::deleteStorageForChildren(int sid)
{
	int count = storage[sid].children.size();
	for(int i = 0; i < count; i++)
	{
		int n = idHash[storage[sid].children[i]];
		deleteStorageForChildren(n);
		deleteStorageForObject(n);
	}
}


void PhysicEngineSteel::makeStorageForChildren(PhysicObject *object)
{
	int count = object->getPhysicChildrenCount();
	for(int i = 0; i < count; i++)
	{
		PhysicObject *child = object->getPhysicChildren(i);
		makeStorageForObject(child);
		makeStorageForChildren(child);
	}
}

void PhysicEngineSteel::cacheStorageObject(PhysicObjectStorage &objectStorage)
{
	PhysicObject *object = objectStorage.object;

	if(objectStorage.modificationTime < object->getModificationTime())
	{
		objectStorage.modificationTime = object->getModificationTime();
		objectStorage.position = object->getPosition().getTranslation();
		objectStorage.mass = object->getMass();

		objectStorage.material = object->getPMaterial();

		if(objectStorage.collisionType == CollisionType::particle)
		{
			objectStorage.spring_r0			= objectStorage.material->getf("spring_r0");
			objectStorage.spring_k			= objectStorage.material->getf("spring_k");
			objectStorage.gravity_k			= objectStorage.material->getf("gravity_k");
			objectStorage.gravity_power		= objectStorage.material->getf("gravity_power");
			objectStorage.gravity_min_dist	= objectStorage.material->getf("gravity_min_dist");
			objectStorage.friction_k		= objectStorage.material->getf("friction_k");
			objectStorage.friction_power	= objectStorage.material->getf("friction_power");
		}
	}
}


void PhysicEngineSteel::prepare(PhysicObject *object, steel::time globalTime, steel::time time, matrix34 matrix, PhysicObject *parent)
{
	object->ProcessPhysic(globalTime, time, globalFrameNumber);

	int sid = idHash[object->getId()];
		
	cacheStorageObject(storage[sid]);

	if(storage[sid].childrenModificationTime < object->getChildrenModificationTime())
	{
		storage[sid].childrenModificationTime = object->getChildrenModificationTime();

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

				storage[sid].children.push_back(id);
			}
			cacheStorageObject(getStorage(child));
		}
		int size = storage[sid].children.size();

		for(int i = 0; i<size; i++)
		{
			uid id = storage[sid].children[i];
			if(newChildrenId.find(id) == newChildrenId.end())
			{
				int n = idHash[id];
				deleteStorageForChildren(n);
				deleteStorageForObject(n);
				storage[sid].children[i] = storage[sid].children.back();
				storage[sid].children.pop_back();
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

	for(steel::svector<PhysicObjectStorage>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		PhysicObjectStorage &objectStorage = *it;
		objectStorage.velocity = objectStorage.object->getVelocity().translation;
		objectStorage.force.loadZero();
	}

	for(steel::svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		processParticle(storage[*it], globalTime, time);
	}
	

	for(steel::svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		PhysicObjectStorage &objectStorage = storage[*it];

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


		PhysicObject &object = *objectStorage.object;

		ObjectPosition objectPosition = objectStorage.object->getPosition();
		objectPosition.setTranslation(objectStorage.position);
		objectStorage.object->setPosition(objectPosition);

		velocity vel;
		vel.translation = objectStorage.velocity;
		objectStorage.object->setVelocity(vel);
	}

	return true;
}

