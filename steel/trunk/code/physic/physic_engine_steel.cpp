#include "physic_engine_steel.h"

v3 PhysicEngineSteel::calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2)
{
	v3 res;
	res.loadZero();

// -------------------------------- PS Forces --------------------------------

	v3 &pos1 = storage1.position;
	v3 &pos2 = storage2.position;

	if(helperDrawLines) // draw collision
		helper->drawLine(Line(pos1, pos2-pos1), 0.0f,0.0f, color4f(1.0f,0.0f,0.0f,1.0f));

	Config *material1 =  storage1.object->getPMaterial();
	Config *material2 =  storage2.object->getPMaterial();

	float dist = (pos2-pos1).getLength();
	
	float spring_r0 = storage1.spring_r0 + storage2.spring_r0;
	float spring_k = storage1.spring_k + storage2.spring_k;

	float gravity_k = storage1.gravity_k + storage2.gravity_k;
	float gravity_power = 0.5f*(storage1.gravity_power + storage2.gravity_power);
	float gravity_min_dist = 0.5f*(storage1.gravity_min_dist + storage2.gravity_min_dist);

	//res += (pos2-pos1).getNormalized() * (dist - spring_r0)*spring_k; // �������
	res += (pos2-pos1).getNormalized() * (pow((1/dist),2) - pow(1/dist,3)); // lennard-jones

	if(dist>gravity_min_dist && gravity_k != 0)
	{
		res += (pos2-pos1).getNormalized() * pow(dist, gravity_power)*gravity_k; // ����������
	}

// --------------------------------------------------------------------------
	storage2.force -= res; // ���� �������� ����� ���� ���������������

	return res;
}

v3 PhysicEngineSteel::calculateForceForParticle(PhysicObjectStorage &storage1)
{
	v3 res;
	res.loadZero();
// ��������� ������ � ���������, ����� ������� ������ �����
// ��� ����, ����� ��������� ��������� �������� (���� �������� ����� ���� ���������������)
	for(unsigned int i = storage1.partiecleSetId + 1; i < particleSet.size(); i++)
		res += calculateForceForParticle(storage1, storage[particleSet[i]]);

	return res;
}


bool PhysicEngineSteel::processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time)
{
	objectStorage.position;

// -------------------
	objectStorage.force += g*objectStorage.mass*0;
	objectStorage.force += calculateForceForParticle(objectStorage);

// -------------------

	return true;
}

void PhysicEngineSteel::prepare(PhysicInterface &object, steel::time globalTime, steel::time time, matrix34 matrix, PhysicInterface *parent)
{
	object.process(globalTime, time, globalFrameNumber);

	PhysicObjectStorage &objectStorage = storage[idHash[object.getId()]];

	cacheStorage(objectStorage);
	cacheChildrenStorage(objectStorage);

	PhysicObjectList *children = object.getPhysicChildrenList();
	if(children)
	for(unsigned int i = 0; i < children->size(); i++)
	{
		PhysicInterface *child = children->at(i);
		prepare(*child, globalTime, time);
	}
}


bool PhysicEngineSteel::process(steel::time globalTime, steel::time time)
{
	helperDrawLines = helper && conf->geti("helperDrawLines");
	if(helper) // draw velocity
		helper->setTime(globalTime);

	int size = objects.size();
	for(int i=0; i < size; i++)
		prepare(*objects[i], globalTime, time);

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

		if((newVelocity & objectStorage.velocity) >0) // ���� ������ �� ����� ���������� ����
				objectStorage.velocity = newVelocity;
		else
			objectStorage.velocity.loadZero();


		objectStorage.position += objectStorage.velocity*time;


		PhysicInterface &object = *objectStorage.object;

		ObjectPosition objectPosition = objectStorage.object->getPosition();
		objectPosition.setTranslation(objectStorage.position);
		objectStorage.object->setPosition(objectPosition);

		velocity vel;
		vel.translation = objectStorage.velocity;
		objectStorage.object->setVelocity(vel);
	}

	return true;
}

bool PhysicEngineSteel::inject(PhysicInterface *obj)
{
	if(!obj->beforeInject()) return false;
	objects.push_back(obj);
	makeStorage(obj);

	PhysicObjectStorage &objectStorage = storage[idHash[obj->getId()]];
	cacheChildrenStorage(objectStorage, true);

	return true;
}

void PhysicEngineSteel::cacheStorage(PhysicObjectStorage &objectStorage)
{
	PhysicInterface *object = objectStorage.object;

	if(objectStorage.modificationTime < object->getModificationTime())
	{
		objectStorage.modificationTime = object->getModificationTime();
		objectStorage.position = object->getPosition().getTranslation();
		objectStorage.mass = object->getMass();

		objectStorage.material = object->getPMaterial();

		if(objectStorage.collisionType == CollisionType::particle1)
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

void PhysicEngineSteel::cacheChildrenStorage(PhysicObjectStorage &objectStorage, bool force)
{
	PhysicInterface *object = objectStorage.object;

	if(force || objectStorage.childrenModificationTime < object->getChildrenModificationTime())
	{
		objectStorage.childrenModificationTime = object->getChildrenModificationTime();

		PhysicObjectList *children = object->getPhysicChildrenList();
		if(children)
		for(unsigned int i = 0; i < children->size(); i++)
		{
			PhysicInterface *child = children->at(i);
			uid id = child->getId();

			if(idHash.find(id) == idHash.end())
				makeStorage(child);
			else
				cacheStorage(storage[idHash[id]]);
		}
	}
}


bool PhysicEngineSteel::makeStorage(PhysicInterface *object)
{
	int storageId = storage.size();
	storage.resize(storageId + 1);

	uid objectId = object->getId();

	PhysicObjectStorage &objectStorage = storage[storageId];

	idHash[objectId] = storageId;

	objectStorage.object = object;
	objectStorage.storageId = storageId;
	objectStorage.objectId = objectId;
	objectStorage.collisionType = object->getCollisionType();
	objectStorage.force.loadZero();

	objectStorage.modificationTime = -1;
	objectStorage.childrenModificationTime = -1;

	cacheStorage(objectStorage);
	cacheChildrenStorage(objectStorage, true);

	if(objectStorage.collisionType == CollisionType::particle1)
	{
		particleSet.push_back(storageId);
		objectStorage.partiecleSetId = particleSet.size()-1;
	}

	return true;
}