#include "physic_engine_ps.h"

v3 PhysicEnginePS::calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2)
{
	if(storage1.object == storage2.object) return v3(0,0,0);

	v3 res;
	res.loadZero();

// -------------------------------- PS Forces --------------------------------

	v3 &pos1 = storage1.position;
	v3 &pos2 = storage2.position;

	if(helperDrawLines) // draw collision
		helper->drawLine(Line(pos1, pos2-pos1), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));

	Config *material1 =  storage1.object->getPMaterial();
	Config *material2 =  storage2.object->getPMaterial();

	float dist = (pos2-pos1).getLength();
	
	float spring_r0 = storage1.spring_r0 + storage2.spring_r0;
	float spring_k = storage1.spring_k + storage2.spring_k;

	float gravity_k = storage1.gravity_k + storage2.gravity_k;
	float gravity_power = 0.5f*(storage1.gravity_power + storage2.gravity_power);
	float gravity_min_dist = 0.5f*(storage1.gravity_min_dist + storage2.gravity_min_dist);

	//res += (pos2-pos1).getNormalized() * (dist - spring_r0)*spring_k; // пружина
	res += (pos2-pos1).getNormalized() * (pow((1/dist),2) - pow(1/dist,3)); // lennard-jones

	if(dist>gravity_min_dist && gravity_k != 0)
	{
		res += (pos2-pos1).getNormalized() * pow(dist, gravity_power)*gravity_k; // гравитация
	}

// --------------------------------------------------------------------------

	return res;
}

v3 PhysicEnginePS::calculateForceForParticle(PhysicObjectStorage &storage1)
{
	v3 res;
	res.loadZero();

	for(steel::svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
		res += calculateForceForParticle(storage1, storage[*it]);

	return res;
}


bool PhysicEnginePS::processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time)
{
	objectStorage.position;

// -------------------
	objectStorage.force = g*objectStorage.mass*0;
	objectStorage.force += calculateForceForParticle(objectStorage);
// -------------------

	return true;
}

bool PhysicEnginePS::process(steel::time globalTime, steel::time time)
{
	helperDrawLines = helper && conf->geti("helperDrawLines");
	if(helper) // draw velocity
		helper->setTime(globalTime);

	for(steel::svector<PhysicObjectStorage>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		PhysicObjectStorage &objectStorage = *it;

		objectStorage.velocity = objectStorage.object->getGlobalVelocity().translation;
		if(objectStorage.object->wasChanged())
			cacheStorage(objectStorage);
	}



	for(steel::svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		processParticle(storage[*it], globalTime, time);
	}
		

	for(steel::svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		PhysicObjectStorage &objectStorage = storage[*it];

		objectStorage.velocity += time*objectStorage.force/objectStorage.mass;
		objectStorage.position += objectStorage.velocity*time;
		objectStorage.velocity *= 0.99f;

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

bool PhysicEnginePS::inject(PhysicInterface *obj)
{
	if(!obj->beforeInject()) return false;
	object.push_back(obj);
	makeStorage(obj);

	return true;
}

void PhysicEnginePS::cacheStorage(PhysicObjectStorage &objectStorage)
{
	PhysicInterface *object = objectStorage.object;

	objectStorage.position = object->getGlobalPosition().getTranslation();
	objectStorage.mass = object->getMass();

	objectStorage.material = object->getPMaterial();

	if(objectStorage.collisionType == CollisionType::particle1)
	{
		objectStorage.spring_r0			= objectStorage.material->getf("spring_r0");
		objectStorage.spring_k			= objectStorage.material->getf("spring_k");
		objectStorage.gravity_k			= objectStorage.material->getf("gravity_k");
		objectStorage.gravity_power		= objectStorage.material->getf("gravity_power");
		objectStorage.gravity_min_dist	= objectStorage.material->getf("gravity_min_dist");
	}
}

bool PhysicEnginePS::makeStorage(PhysicInterface *object)
{
	int id = storage.size();
	storage.resize(id + 1);

	object->setStorageId(id);
	PhysicObjectStorage &objectStorage = storage[id];

	objectStorage.object = object;
	objectStorage.collisionType = object->getCollisionType();
	objectStorage.force.loadZero();

	cacheStorage(objectStorage);

	if(objectStorage.collisionType == CollisionType::particle1)
		particleSet.push_back(id);

	PhysicInterfaceList children = object->getPChildrens();
	for(PhysicInterfaceList::iterator it = children.begin(); it != children.end(); it++)
		makeStorage(*it);

	return true;
}
