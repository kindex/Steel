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

	// кеш объекта
	struct PhysicObjectStorage
	{
		// инентификатор объекта (uid)
		uid objectId;
		int storageId; // индекс этой структуры (кеша) в массиве stroage
		PhysicObject *object; // ссылка на объект
		CollisionType collisionType;

		// время последнего изменения объекта. Если отлично от того, что возвращает PhysicObject::getModificationTime(), то надо обновить кеш.
		ModificationTime modificationTime, childrenModificationTime;

		// список детей объекта (uid)
		steel::svector<uid> children;

		// индекс в массиве particleSet
		int partiecleSetId;

		// *** Particle ***
		v3 force;
		v3 position;
		v3 velocity;
		float mass, spring_r0, spring_k, gravity_k, gravity_power, gravity_min_dist, friction_k, friction_power;
		Config *material;
	};

	// кеш объектов
	steel::svector<PhysicObjectStorage> storage;
	// множество объектов с типом обработки частица
	steel::svector<int> particleSet;

public:

	PhysicObjectStorage &getStorage(PhysicObject *object);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	void makeStorageForObject(PhysicObject *object);
	void deleteStorageForObject(int sid);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для детей объекта
	void makeStorageForChildren(PhysicObject *object);
	void deleteStorageForChildren(int sid);

	// овновляюет место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	void cacheStorageObject(PhysicObjectStorage &objectStorage);

	// рекурсивно обновить информацию об объектах и их детей
	void prepare(PhysicObject *object, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), PhysicObject *parent = NULL);

	// обработать движение всех объектов
	bool process(steel::time globalTime, steel::time time);

	// обработать движение одного объекта
	bool processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time);

	// рассчитать суммарно действующие силы для частицы
	v3 calculateForceForParticle(PhysicObjectStorage &storage);
	// рассчитать силу взаимодействия между двумя частицами
	v3 calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2);
};


#endif
