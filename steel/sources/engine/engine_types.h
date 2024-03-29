/*id*********************************************************
	File: engine/engine_types.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		�������� ���� ������ ��� ���������� ������.
 ************************************************************/

#ifndef ENGINE__ENGINE_TYPES_H
#define ENGINE__ENGINE_TYPES_H

#include "../steel.h"

#include "../math/vector3d.h"
#include "id_generator.h"
#include "../common/types.h"


// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	Triangle(){}
	Triangle(unsigned int _0, unsigned int _1, unsigned int _2) { set(_0, _1, _2); }
	void set(unsigned int _0, unsigned int _1, unsigned int _2) { a[0] = _0;	a[1] = _1;	a[2] = _2;	}

	unsigned int a[3];
};

// One triangle, indexes of vertexes
struct Quad // anti-clockwise direction (one side Triangle)
{
	Quad(){}
	Quad(size_t _0, size_t _1, size_t _2, size_t _3) { set(_0, _1, _2, _3); }
	void set(size_t _0, size_t _1, size_t _2, size_t _3) { a[0] = _0;	a[1] = _1;	a[2] = _2;	a[3] = _3;}

	size_t a[4];
};

// ������ ��������, ������� ����� ���� ��������� � ����������� ����������
class BufferedElement
{
public:
	BufferedElement(): changed(true) { id = objectIdGenerator.genUid(); }
	uid		getId() const { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged() const { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }

	// ���� ���� ������ ��� ������� � �������� ����, �� true
	bool	changed;
	// ���������� ������������� ��� ������� ������
	uid		id;
};

class TriangleVector : 
    public pvector<Triangle>, 
    public BufferedElement 
{
};

class QuadVector : 
    public pvector<Quad>, 
    public BufferedElement
{
};

// ������ ������ �������������
struct VertexVector : 
    public pvector<v3>, 
    public BufferedElement 
{
};

// ������ ������������� �������������
struct Faces
{	
	TriangleVector	triangles;
	QuadVector		quads;
};

class Camera
{
public:
	Camera();

    void setPosition (const v3 &_position)	{ position	= _position; }
	void setDirection(const v3& _direction)	{ direction	= _direction.getNormalized(); }
    void setUpVector (const v3& _upVector);
	void set(const v3 &_position, const v3& _direction, const v3& _upVector);

	v3	getDirection()	const { return direction; }
	v3  getPosition()	const { return position; }
	v3  getUpVector()	const { return upVector; }

private:
    v3 position; // camera eye
	v3 direction;
	v3 upVector;
};

struct TimeInfo
{
	TimeInfo() {}
	TimeInfo(IN const steel::time currentTime, 
			 IN const steel::time frameLength) :
		currentTime(currentTime),
		frameLength(frameLength)
	{}

	steel::time currentTime;
	steel::time frameLength;
};

struct ProcessInfo
{
	TimeInfo	timeInfo;
	Camera		camera;
};


#endif
