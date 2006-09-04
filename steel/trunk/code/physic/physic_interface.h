/*id*********************************************************
	Unit: PhysicEngine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		���������� ������ - ��������� ������������ �������
 ************************************************************/

#ifndef PHYSIC_INTERFACE_H
#define PHYSIC_INTERFACE_H

#include "../engine.h"
#include "../math/aabb.h"
#include "../math/matrix34.h"
#include "../math/vector3d.h"
#include "../res/conf/conf.h"

typedef enum
{
	PROCESS_NONE,
	PROCESS_CUSTOM, // ��� ������ ���������� ��� ��������
	PROCESS_UNI // �������� �� ������������� �������
} ProcessKind;

typedef enum
{
	COLLISION_NONE,
	COLLISION_POLYHEDRA,
	COLLISION_TRIGGER,
	COLLISION_PARTICLE, // sphere, collide with polyhedra
	COLLISION_SPHERE
} CollisionType;


struct velocity
{
	v3 translation; // �������� ��������������� ��������
	v3 rotationAxis;  // ��� ��������*�������� ��������(������/���)

	inline velocity() {}
	inline velocity(const v3 _translation, const v3 _axis) 
	{
		translation = _translation;
		rotationAxis = _axis;
	}
};

class PhysicObject;

typedef steel::svector<PhysicObject *> PhysicObjectList;

class PhysicObject: public Interface
{
//	steel::time	currentTime; // �����, � ������� ���������� ������
	friend class PhysicEngine;
	friend class PhysicEngineSteel;

public:

// *** Common ***

	// ������ �����
	/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	// ���������� ���������� �����
	virtual int getPhysicChildrenCount(void) { return 0; } 
	// ������ � ��������� �������
	virtual PhysicObject* getPhysicChildren(int number) { return NULL; }

	// ��������������� ����� ����������� � ������ ���������� 
	virtual bool PhysicBeforeInject() { return true;}
	// ����� �������� �� ������ ���������� ��������� afterRemove
	virtual void PhysicAfterRemove() {}

	// ���������� ����� ������ ��������� ���������. ������ ���� ��������� ������ ����� ������ ��������� ���� ���������
	virtual	void ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) {}


// *** Configuration ***


	// ����� ������� � ������ �������� � ��������� ��������
	virtual CollisionType getCollisionType() { return COLLISION_NONE; }

	// ��������� � ������� ������������ ����� ������� � ��������� ����������� (����� ������� �������). ������ �� ��������� ����� ��������� � �������� ���������� ������.
	virtual	void setPosition(ObjectPosition const &newPosition) = 0;

	// ��������
	virtual velocity	getVelocity() = 0;
	virtual void		setVelocity(const velocity &v) = 0;
	// �����
	virtual	float	getMass() = 0;


/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
//	virtual aabb getPFrame() = 0; // AABB of object

	// *** Polyhedra ***
	virtual Vertexes*	getPVertexes() { return NULL; } // ������ ������ (���������� ������������ getPosition() � ���� ������ �������)
	// ������ �������� ������, ������� �������� ������������ (�����)
	virtual Triangles*	getTriangles() { return NULL; }

	virtual Config* getPMaterial() { return NULL; }
	// ��� ������� ����������, ���� ������ ������ ������� ����
	virtual void	trigger(PhysicObject *object) {}
};

#endif
