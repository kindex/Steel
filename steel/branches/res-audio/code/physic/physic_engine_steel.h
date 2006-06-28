/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
    License:
        Steel Engine License
    Description:
************************************************************/

#ifndef PHYSIC_ENGINE_PS_H
#define PHYSIC_ENGINE_PS_H

#include "physic_engine.h"

#include "../common/steel_vector.h"

// hash_map include (различные файлы, так как hash_map не входит в стандарт С++)
#ifdef STEEL_COMPILER_VS8
	#include <hash_map>
#endif

#ifdef STEEL_COMPILER_DEVCPP
	#include <ext/hash_map>
#endif


class PhysicEngineSteel: public PhysicEngine
{
protected:
	bool helperDrawLines;

	struct PhysicObjectStorage
	{
		int objectId, storageId;
		PhysicInterface *object;
		CollisionType::CollisionType collisionType;
		ModificationTime modificationTime, childrenModificationTime;

		steel::svector<int> children;


		int partiecleSetId;
		v3 force;
		v3 position;
		v3 velocity;
		float mass, spring_r0, spring_k, gravity_k, gravity_power, gravity_min_dist, friction_k, friction_power;
		Config *material;
	};

	// hash_map namespace
#ifdef STEEL_COMPILER_VS8
	stdext::
#endif

#ifdef STEEL_COMPILER_DEVCPP
	__gnu_cxx::
#endif
		
	hash_map<uid, int> idHash;

	steel::svector<PhysicObjectStorage> storage;
	steel::svector<int> particleSet;

public:
	bool inject(PhysicInterface *object);
	bool makeStorage(PhysicInterface *object);
	void cacheStorage(PhysicObjectStorage &objectStorage);
	void cacheChildrenStorage(PhysicObjectStorage &objectStorage, bool force = false);

	bool process(steel::time globalTime, steel::time time);

	bool processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time);

	void prepare(PhysicInterface &object, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), PhysicInterface *parent = NULL);

	v3 calculateForceForParticle(PhysicObjectStorage &storage);
	v3 calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2);
};


#endif
