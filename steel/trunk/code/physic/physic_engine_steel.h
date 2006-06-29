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
/*
#ifdef STEEL_COMPILER_VS8
	#include <hash_map>
#endif

#ifdef STEEL_COMPILER_DEVCPP
	#include <ext/hash_map>
#endif
*/
#include <map>


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

	/*
	// hash_map namespace
#ifdef STEEL_COMPILER_VS8
	stdext::
#endif

#ifdef STEEL_COMPILER_DEVCPP
	__gnu_cxx::
#endif
	hash_map <uid, int>
		*/
	typedef std::map <uid, int> StorageHash;
	StorageHash idHash;

	steel::svector<PhysicObjectStorage> storage;
	steel::svector<int> particleSet;

public:
	// добавляет объект и его детей в движок для обработки
	bool inject(PhysicInterface *object);

	PhysicObjectStorage &getStorage(PhysicInterface *object);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	void makeStorageForObject(PhysicInterface *object);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта и его детей
	void makeStorageForChildren(PhysicInterface *object);

	// овновляюет место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	void cacheStorageObject(PhysicObjectStorage &objectStorage);

	// рекурсивно обновить информацию об объектах и их детей
	void prepare(PhysicInterface *object, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), PhysicInterface *parent = NULL);

	bool process(steel::time globalTime, steel::time time);


	bool processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time);

	v3 calculateForceForParticle(PhysicObjectStorage &storage);
	v3 calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2);
};


#endif
