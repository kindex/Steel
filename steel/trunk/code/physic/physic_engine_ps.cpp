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

	float dist = (pos2-pos1).getLength();
	
	float r0 = 0.6f;

	res += (pos2-pos1).getNormalized() * (dist - r0);

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

		v3 f;
		f.loadZero();
// -------------------
		f = g*mass*0;

		f += calculateForce(&o);

// -------------------
		vel.translation += time*f/mass;

		coord += vel.translation*time;

		pos.setTranslation(coord);
		o.setPosition(pos);
		o.setVelocity(vel);
	}

// move childrens
	PhysicInterfaceList c = o.getPChildrens();
	for(PhysicInterfaceList::const_iterator it = c.begin(); it != c.end(); it++)
		process(**it, globalTime, time);


	return true;
}

bool PhysicEnginePS::process(steel::time globalTime, steel::time time)
{
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		process(**it, globalTime, time);

	return true;
}
