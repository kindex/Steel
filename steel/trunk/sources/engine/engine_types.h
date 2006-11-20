/*id*********************************************************
	File: engine/engine_types.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		�������� ���� ������ ��� ���������� ������.
 ************************************************************/

#ifndef ENGINE__ENGINE_TYPES_H
#define ENGINE__ENGINE_TYPES_H

#include "../steel.h"

#include "../math/vector3d.h"
#include "../common/steel_vector.h"
#include "id_generator.h"


// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];

	void set(unsigned int _0, unsigned int _1, unsigned int _2) 
	{ 
		a[0] = _0;
		a[1] = _1;
		a[2] = _2;
	}
};

// ������ ��������, ������� ����� ���� ��������� � ����������� ����������
class BufferedElement
{
public:
	// true, ���� ���� ������ ��� ������ � �������� ����
	bool	changed;
	// ���������� ������������� ��� ������� ������
	uid		id;
public:
	BufferedElement(void): changed(true) { id = objectIdGenerator.genUid(); }
	uid		getId(void) const { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged(void) const { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }
};

// ������ ������ �������������
struct Vertexes: public BufferedElement
{
	steel::vector<v3> data;

	Vertexes(): BufferedElement(), data(0) {}
};

// ������ ������������� �������������
struct Triangles: public BufferedElement
{
	steel::vector<Triangle> data;

	Triangles(void): BufferedElement() {}
};


#endif
