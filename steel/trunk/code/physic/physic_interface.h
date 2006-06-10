/*id*********************************************************
    Unit: PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        ������ ��� Division
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

class PhysicInterface;
typedef steel::vector<PhysicInterface*> PhysicInterfaceList;

class PhysicEngine;

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

class PhysicInterface: public Interface
{
	int storageId;
//	steel::time	currentTime; // �����, � ������� ���������� ������
	friend class PhysicEngine;
	friend class PhysicEngine3D;

public:
	virtual bool beforeInject() { return true;}
	virtual void afterRemove() {}
	// ���������� true, ���� ���� ���� �� ���������� ������� ��� �������� ��
	// ����� ������ ������� set* (mass, position, material, mesh)
	// �������� ����� ��������	
	virtual bool wasChanged() { return false; }

	virtual void setStorageId(const int _id) { storageId = _id; }
	virtual int getStorageId(void) { return storageId; }

	virtual CollisionType::CollisionType getCollisionType() = 0;

	virtual	ObjectPosition getGlobalPosition(void) = 0;
	virtual	void setPosition(ObjectPosition const &newPosition) = 0;
	virtual ProcessKind::ProcessKind getProcessKind() = 0;
	virtual	void setPositionKind(PositionKind::PositionKind const newPositionKind) = 0;

	// �������� � ���������� �����������
	virtual velocity	getVelocity() = 0;
	virtual void		setVelocity(const velocity &v) = 0;
	virtual velocity	getGlobalVelocity() = 0;

	PhysicInterface() {}
/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	virtual PhysicInterfaceList getPChildrens() = 0;

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual Vertexes*	getPVertexes() = 0; // ������ ������ (���������� ������������ getPosition() � ���� ������ �������)
	// ������ �������� ������, ������� �������� ������������ (�����)
	virtual Triangles*	getTriangles() = 0; 

	virtual	float		getGlobalScale() = 0;

	// �����
	virtual	coord	getMass() = 0;

// ���������� ������������� ������� ��� ������ ������
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) = 0;
//	virtual std::string getMaterial() = 0;

	virtual Config* getPMaterial() = 0;
	// ��� ������� ����������, ���� ������ ������ ������� ����
	virtual void	trigger(PhysicInterface *object) = 0;
};

#endif

