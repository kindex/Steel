/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
		* Bond [Andrey Bondarenko]
    Licence:
        ������ ��� Division
    Description:
		����������� ������ - ��������� ��������� �������
 ************************************************************/

#ifndef PHYSIC_INTERFACE_H
#define PHYSIC_INTERFACE_H

#include "../engine.h"
#include "../math/geometry.h"
#include "../math/matrix4x4.h"
#include "../math/maths.h"
#include "../math/vector3d.h"

#include <string>

class PhysicInterface;
typedef std::vector<PhysicInterface*> PhysicInterfaceList;

class PhysicInterface: public Interface
{
public:
	PhysicInterface() {}
/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	virtual PhysicInterfaceList getPChildrens() = 0;

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual bool cleanupP() = 0; // ��� ������������� ��������
	virtual Vertexes*	getPVertexes() = 0; // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
	// ������ �������� ������, ������� �������� ������������ (�����)
	virtual Triangles*	getPTriangles() = 0; 

/*������� ������������� ������� ������������ ��� ��������. 
�������� � ���� �������, ����� � �������.
getPVertexes ���������� ���������� ����� � ������� ��������� getPMatrix*/

	virtual matrix4		getPMatrix() = 0;
	virtual void		setPMatrix(matrix4 const &m) = 0;

	// ��������� (����� �������� ��������, ������� �������� � ���� ���������, ������� � ������� ����)
//	virtual v3		getPosition() = 0;
	// ���������� true, ���� ����� �������� ���������
//	virtual bool	setPosition(v3 const &v) = 0;

// ����� �� ������ ��������� ��� �������� � ��������� �� �� ���� ������� ����, ����� ����������
	virtual bool	isMovable() = 0;
	virtual bool	isRotatable() = 0;

	// �������� � ���������� �����������
	virtual v3		getVelocity() = 0;
	virtual void	setVelocity(v3 const &v) = 0;
	// �����
	virtual	coord	getMass() = 0;

// ���� ������ ����� ��������� � ������� �������, �� �� ���������� true 
// � ��������� ���������� ���� ��������
	virtual bool	getTarget(v3 &targetPoint, coord &speed) = 0;
// ���� ���� ����������, �� ���������� ��� �������
	virtual void	setTargetReached() = 0;
// ���������� ������������� ������� ��� ������ ������
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time speed) = 0;
//	virtual std::string getMaterial() = 0;
};

#endif

