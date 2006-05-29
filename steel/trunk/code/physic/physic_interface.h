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
#include "../math/matrix4x4.h"
#include "../math/vector3d.h"
#include "../res/conf/conf.h"

#include <string>

class PhysicInterface;
typedef std::vector<PhysicInterface*> PhysicInterfaceList;

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
		uni,
		trigger
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
	steel::time	currentTime; // �����, � ������� ���������� ������
	friend class PhysicEngine;
	friend class PhysicEngine3D;

public:
	virtual ProcessKind::ProcessKind getProcessKind() = 0;

	PhysicInterface() {}
/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	virtual PhysicInterfaceList getPChildrens() = 0;

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual bool cleanupP() = 0; // ��� ������������� ��������
	virtual Vertexes*	getPVertexes() = 0; // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
	// ������ �������� ������, ������� �������� ������������ (�����)
	virtual Triangles*	getTriangles() = 0; 

	// �������� � ���������� �����������
	virtual velocity	getVelocity() = 0;
	virtual void	setVelocity(const velocity &v) = 0;
	virtual matrix44	getGlobalMatrix() = 0;
	virtual velocity	getGlobalVelocity() = 0;
	virtual	float		getGlobalScale() = 0;

	// �����
	virtual	coord	getMass() = 0;

// ���������� ������������� ������� ��� ������ ������
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) = 0;
	virtual	v3		getPosition() = 0;
	virtual	bool	setPosition(v3 const &v) = 0;
//	virtual std::string getMaterial() = 0;

	virtual Config* getPMaterial() = 0;
	// ��� ������� ����������, ���� ������ ������ ������� ����
	virtual void	trigger(PhysicInterface *object) = 0;
};

#endif

