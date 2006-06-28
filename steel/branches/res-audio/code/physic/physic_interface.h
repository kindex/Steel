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

#include <string>

namespace ProcessKind
{
	typedef enum
	{
		none,
		custom, // ��� ������ ���������� ��� ��������
		uni // �������� �� ������������� �������
	} ProcessKind;
};

namespace CollisionType
{
	typedef enum
	{
		none,
		polyhedra,
		trigger,
		particle1, // sphere, collide with polyhedra
		particle2, // sphere, collide with polyhedra & other particles
	} CollisionType;
};

namespace ShapeType
{
	typedef enum
	{
		none,
		poly,
		sphere
	} ShapeType;
}

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

class PhysicInterface;

typedef steel::svector<PhysicInterface *> PhysicObjectList;

class PhysicInterface: public Interface
{
//	steel::time	currentTime; // �����, � ������� ���������� ������
	friend class PhysicEngine;
	friend class PhysicEngineSteel;

public:

	// ������ �����
	/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	virtual PhysicObjectList* getPhysicChildrenList(void) { return NULL; }


	virtual bool beforeInject() { return true;}
	virtual void afterRemove() {}

	virtual CollisionType::CollisionType getCollisionType() = 0;

	virtual	void setPosition(ObjectPosition const &newPosition) = 0;
	virtual ProcessKind::ProcessKind getProcessKind() = 0;

	// �������� � ���������� �����������
	virtual velocity	getVelocity() = 0;
	virtual void		setVelocity(const velocity &v) = 0;


/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual Vertexes*	getPVertexes() = 0; // ������ ������ (���������� ������������ getPosition() � ���� ������ �������)
	// ������ �������� ������, ������� �������� ������������ (�����)
	virtual Triangles*	getTriangles() = 0; 

	// �����
	virtual	coord	getMass() = 0;

// ���������� ������������� ������� ��� ������ ������
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) {}

	virtual Config* getPMaterial() = 0;
	// ��� ������� ����������, ���� ������ ������ ������� ����
	virtual void	trigger(PhysicInterface *object) {}
};

#endif

