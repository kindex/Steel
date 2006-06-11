/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
    Licence:
        Только для Division
    Description:
************************************************************/

#ifndef PHYSIC_ENGINE_PS_H
#define PHYSIC_ENGINE_PS_H

#include "physic_engine.h"

#include "../common/steel_vector.h"


class PhysicEnginePS: public PhysicEngine
{
protected:
	bool helperDrawLines;

	struct PhysicObjectStorage
	{
		int id, pSetId;
		PhysicInterface *object;
		CollisionType::CollisionType collisionType;
		v3 force;
		v3 position;
		v3 velocity;
		float mass, spring_r0, spring_k, gravity_k, gravity_power, gravity_min_dist, friction_k, friction_power;
		Config *material;
	};


	steel::svector<PhysicObjectStorage> storage;
	steel::svector<int> particleSet;

public:
	bool inject(PhysicInterface *object);
	bool makeStorage(PhysicInterface *object);
	void cacheStorage(PhysicObjectStorage &objectStorage);

	bool process(steel::time globalTime, steel::time time);

	bool processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time);

	bool prepare(PhysicInterface *object, matrix34 matrix = matrix34::getIdentity(), PhysicInterface *parent = NULL){ return true;}

	v3 calculateForceForParticle(PhysicObjectStorage &storage);
	v3 calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2);
};


#endif
