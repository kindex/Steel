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
#include "math/matrix34.h"

#include "common/steel_vector.h"

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
	steel::vector<v3> data;
	Vertexes() {}
};

// ������ ������������� �������������
struct Triangles: public BufferedElement
{
	steel::vector<Triangle> data;
};

/*
�������������� + ������� �������
*/
typedef matrix34 ObjectPosition;

namespace PositionKind
{
	typedef enum
	{
		none,
		local,
		global
	} PositionKind;
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
	virtual	ObjectPosition getPosition(void) = 0;
	virtual PositionKind::PositionKind getPositionKind(void) = 0;
};

#endif

