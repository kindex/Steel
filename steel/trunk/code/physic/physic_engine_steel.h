/*id*********************************************************
	Unit: 3D PhysicEngine Steel
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Steel ������ ����������� ������. 
		������������ �������� ������, ������������� ��� � �����.
		������������ ��������� ��������, �������� ���������� ��	��������.
************************************************************/

#ifndef PHYSIC_ENGINE_PS_H
#define PHYSIC_ENGINE_PS_H

#include "physic_engine.h"

#include "../common/steel_vector.h"

// hash_map include (��������� �����, ��� ��� hash_map �� ������ � �������� �++)
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

	// ��� �������
	struct PhysicObjectStorage
	{
		// ������������� ������� (uid)
		uid objectId;
		int storageId; // ������ ���� ��������� (����) � ������� stroage
		PhysicObject *object; // ������ �� ������
		CollisionType collisionType;

		// ����� ���������� ��������� �������. ���� ������� �� ����, ��� ���������� PhysicObject::getModificationTime(), �� ���� �������� ���.
		ModificationTime modificationTime, childrenModificationTime;

		// ������ ����� ������� (uid)
		steel::svector<uid> children;

		// ������ � ������� particleSet
		int partiecleSetId;

		// *** Particle ***
		v3 force;
		v3 position;
		v3 velocity;
		float mass, spring_r0, spring_k, gravity_k, gravity_power, gravity_min_dist, friction_k, friction_power;
		Config *material;
	};

	// ��� ��������
	steel::svector<PhysicObjectStorage> storage;
	// ��������� �������� � ����� ��������� �������
	steel::svector<int> particleSet;

public:

	PhysicObjectStorage &getStorage(PhysicObject *object);
	// ������ ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ������ �������
	void makeStorageForObject(PhysicObject *object);
	void deleteStorageForObject(int sid);
	// ������ ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ����� �������
	void makeStorageForChildren(PhysicObject *object);
	void deleteStorageForChildren(int sid);

	// ���������� ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ������ �������
	void cacheStorageObject(PhysicObjectStorage &objectStorage);

	// ���������� �������� ���������� �� �������� � �� �����
	void prepare(PhysicObject *object, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), PhysicObject *parent = NULL);

	// ���������� �������� ���� ��������
	bool process(steel::time globalTime, steel::time time);

	// ���������� �������� ������ �������
	bool processParticle(PhysicObjectStorage &objectStorage, steel::time globalTime, steel::time time);

	// ���������� �������� ����������� ���� ��� �������
	v3 calculateForceForParticle(PhysicObjectStorage &storage);
	// ���������� ���� �������������� ����� ����� ���������
	v3 calculateForceForParticle(PhysicObjectStorage &storage1, PhysicObjectStorage &storage2);
};


#endif
