/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        ������ ��� Division
    Description:
		������ ��� ����������� ��������, ������� ����� ���������
		(����������, ��������� ������� ���������)
************************************************************/
#ifndef INTERFACE_H
#define INTERFACE_H

#include "math/vector3d.h"
#include "math/matrix4x4.h"

#include <vector>

// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];
};

typedef int	uid;// ���������� ������������� ������

// ������ ��������, ������� ����� ���� ��������� � ����������� ����������
class BufferedElement
{
public:
	// true, ���� ���� ������ ��� ������ � �������� ����
	bool	changed;
	// ���������� ������������� ��� ������� ������
	uid		id;
public:
	uid		getId() const { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged() const { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }
};

// ������ ������ �������������
struct Vertexes: public BufferedElement
{
	std::vector<v3> data;
	Vertexes(): data(0) {}
};

// ������ ������������� �������������
struct Triangles: public BufferedElement
{
	std::vector<Triangle> data;
};

/*
Interface protottype
�� ����� ������ ����������� ��� ������� � ���������� � ������.

������ ����� ������:
* ����� ��������� (� ����������� ��� ��������� �����������). 
��������� - ������� 4�4, � ������� ���������� �����, ������� � ��������������� ����
������������ ��� �������� ��� � ���������� �����������. ������� ������� ���������� 
������� getPositionKind
*/

class Interface
{
public:
	virtual	matrix44	getMatrix() = 0;
	virtual	void		setMatrix(matrix44 const &m) = 0; 

	typedef enum
	{
		none,
		local,
		global
	} PositionKind;

    virtual PositionKind	getPositionKind() = 0;
	virtual void changePositionKind(const PositionKind newKind) = 0;
};

#endif

