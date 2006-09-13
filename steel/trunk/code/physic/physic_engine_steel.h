/*id*********************************************************
	File: physic/physic_engine_steel.h
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

#ifndef PHYSIC_ENGINE_PS_H
#define PHYSIC_ENGINE_PS_H

#include "../steel.h"
#include "physic_engine.h"

#include "../common/steel_vector.h"

// hash_map include (различные файлы, так как hash_map не входит в стандарт С++)
/*
#if STEEL_COMPILER== ... VS8
	#include <hash_map>
#endif

#ifdef STEEL_COMPILER==...DEVCPP
	#include <ext/hash_map>
#endif
*/
#include <map>


class PhysicEngineSteel: public PhysicEngine
{
protected:
	bool helperDrawLines;

	// кеш объекта
	struct PhysicStorage: public Storage
	{
		// инентификатор объекта (uid)
		uid objectId;
//		PhysicObject *object; // ссылка на объект
		CollisionType collisionType;

		// время последнего изменения объекта. Если отлично от того, что возвращает PhysicObject::getModificationTime(), то надо обновить кеш.
		ModificationTime modificationTime, childrenModificationTime;

		// индекс в массиве particleSet
		int partiecleSetId;
		float mass;
		Config *material;
		v3 position;
		v3 velocity;
		v3 force;

		void fill(Interface *object);
		void cache(void);
		PhysicStorage(void):material(NULL) {}
	};

	struct PhysicStorageParticle: public PhysicStorage
	{
		// *** Particle ***
		float spring_r0, spring_k, gravity_k, gravity_power, gravity_min_dist, friction_k, friction_power;

		void cache(void);
	};

	// множество объектов с типом обработки частица
	steel::svector<int> particleSet;

public:
	PhysicStorage *getStorage(PhysicObject *object);
	PhysicStorage* getStorage(uid id);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	Storage* getStorageClass(Interface *object);
	
	void makeStorageForObjectPost(Interface *object, Storage *storage);
	void deleteStorageForObjectPost(int sid);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для детей объекта
	void makeStorageForChildren(Interface *object);

	// рекурсивно обновить информацию об объектах и их детей
	void prepare(PhysicObject *object, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), PhysicObject *parent = NULL);

	// обработать движение всех объектов
	bool process(steel::time globalTime, steel::time time);

// ************* PARTICLE *************
	// обработать движение одного объекта
	bool processParticle(PhysicStorageParticle *objectStorage, steel::time globalTime, steel::time time);

	// рассчитать суммарно действующие силы для частицы
	v3 calculateForceForParticle(PhysicStorageParticle *storage);
	// рассчитать силу взаимодействия между двумя частицами
	v3 calculateForceForParticle(PhysicStorageParticle *storage1, PhysicStorageParticle *storage2);
};


#endif
