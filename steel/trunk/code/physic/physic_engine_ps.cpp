#include "physic_engine_ps.h"
using namespace std;

v3 PhysicEnginePS::calculateForce(PhysicInterface *obj1, PhysicInterface *obj2)
{
	if(obj1 == obj2) return v3(0,0,0);

	v3 res;
	res.loadZero();

	if(obj2->getCollisionType() == CollisionType::particle1)
	{
// ---------------

	v3 pos1 = obj1->getPosition().getTranslation();
	v3 pos2 = obj2->getPosition().getTranslation();

	if(helper) // draw collision
		helper->drawLine(Line(pos1, pos2-pos1), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));


	Config *material1 =  obj1->getPMaterial();
	Config *material2 =  obj2->getPMaterial();

	float dist = (pos2-pos1).getLength();
	
	float spring_r0 = material1->getf("spring_r0") + material2->getf("spring_r0");
	float spring_k = material1->getf("spring_k") + material2->getf("spring_k");

	float gravity_k = material1->getf("gravity_k") + material2->getf("gravity_k");
	float gravity_power = 0.5f*(material1->getf("gravity_power") + material2->getf("gravity_power"));
	float gravity_min_dist = 0.5f*(material1->getf("gravity_min_dist") + material2->getf("gravity_min_dist"));


	res += (pos2-pos1).getNormalized() * (dist - spring_r0)*spring_k; // пружина

	if(dist>gravity_min_dist && gravity_k>0)
	{
		res += (pos2-pos1).getNormalized() * pow(dist, gravity_power)*gravity_k; // гравитация
	}

// ---------------
	}
	PhysicInterfaceList c = obj2->getPChildrens();
	for(PhysicInterfaceList::const_iterator it = c.begin(); it != c.end(); it++)
		res += calculateForce(obj1, *it);

	return res;
}

v3 PhysicEnginePS::calculateForce(PhysicInterface *obj)
{
	v3 res;
	res.loadZero();

	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		res += calculateForce(obj, *it);

	return res;
}


bool PhysicEnginePS::process(PhysicInterface &o, steel::time globalTime, steel::time time)
{
	CollisionType::CollisionType type = o.getCollisionType();
	if(type == CollisionType::particle1)
	{
		matrix34 pos = o.getPosition();
		v3 coord = pos.getTranslation();
		velocity vel = o.getVelocity();
		float mass = o.getMass();

		int id = o.getStorageId();
		PhysicObjectStorage &objectStorage = storage[id];

		objectStorage.force.loadZero();
// -------------------
		objectStorage.force = g*mass*0;
		v3 f = calculateForce(&o);
		objectStorage.force += f;
// -------------------
	}

// move childrens
	PhysicInterfaceList c = o.getPChildrens();
	for(PhysicInterfaceList::const_iterator it = c.begin(); it != c.end(); it++)
		process(**it, globalTime, time);


	return true;
}

bool PhysicEnginePS::process(steel::time globalTime, steel::time time)
{
	if(helper) // draw velocity
		helper->setTime(globalTime);

/*	for(vector<PhysicObjectStorage>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		it->force.loadZero();
	}
*/

	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		process(**it, globalTime, time);


	for(vector<PhysicObjectStorage>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		PhysicObjectStorage &objectStorage = *it;
		PhysicInterface &object = *objectStorage.object;

		int id = object.getStorageId();

		if(object.getCollisionType() == CollisionType::particle1)
		{
			matrix34 pos = object.getPosition();
			v3 coord = pos.getTranslation();

			velocity vel = object.getVelocity();

			float mass = object.getMass();

			vel.translation += time*objectStorage.force/mass;
			coord += vel.translation*time;

			vel.translation *= 0.99f;

			pos.setTranslation(coord);
			object.setPosition(pos);
			object.setVelocity(vel);
		}
	}

	return true;
}

bool PhysicEnginePS::inject(PhysicInterface *obj)
{
	object.push_back(obj);
	makeStorage(obj);

	return true;
}

bool PhysicEnginePS::makeStorage(PhysicInterface *object)
{
	int id = storage.size();
	storage.resize(id + 1);

	object->setStorageId(id);
	storage[id].object = object;

	PhysicInterfaceList children = object->getPChildrens();
	for(PhysicInterfaceList::iterator it = children.begin(); it != children.end(); it++)
		makeStorage(*it);

	return true;
}
