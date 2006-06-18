/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
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

class IdGenerator
{
protected:
	uid freeId;
public:
	IdGenerator() { freeId = 1; }
	uid genUid() { return freeId++;}
};

extern IdGenerator objectIdGenerator;

/*
Interface protottype
�� ����� ������ ����������� ��� ������� � ���������� � ������.

������ ����� ������:
* ����� ��������� (� ����������� ��� ��������� �����������). 
��������� - ������� 3�4, � ������� ���������� �����, ������� � ��������������� ����
������������ ��� �������� ��� � ���������� �����������. ������� ������� ���������� 
������� getPositionKind
*/

class Interface
{
protected:
	uid id;
public:
	Interface() { id = objectIdGenerator.genUid(); }

	virtual uid								getId()				{ return id; }
	virtual	ObjectPosition					getPosition(void) = 0;
	virtual PositionKind::PositionKind		getPositionKind(void) { return PositionKind::local; }
};

#endif

