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
		Основные типы данных для интерфейса движка.
 ************************************************************/

#ifndef ENGINE__ENGINE_TYPES_H
#define ENGINE__ENGINE_TYPES_H

#include "../steel.h"

#include "../math/vector3d.h"
#include "id_generator.h"


// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	Triangle(){}
	Triangle(unsigned int _0, unsigned int _1, unsigned int _2) { set(_0, _1, _2); }

	void set(unsigned int _0, unsigned int _1, unsigned int _2) 
	{ 
		a[0] = _0;
		a[1] = _1;
		a[2] = _2;
	}

	unsigned int a[3];


};
typedef pvector<Triangle> TriangleVector;

// Массив значений, который может быть кеширован в графическом ускорителе
class BufferedElement
{
public:
	// true, если этот массив был изменён с прошлого раза
	bool	changed;
	// уникальный идентификатор для массива данных
	uid		id;
public:
	BufferedElement(): changed(true) { id = objectIdGenerator.genUid(); }
	uid		getId() const { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged() const { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }
};

// массив вершин многогранника
struct Vertexes: public BufferedElement
{
	v3Vector data;

	Vertexes(): BufferedElement(), data(0) {}
};

// массив треугольников многогранника
struct Triangles: public BufferedElement
{
	TriangleVector data;

	Triangles(): BufferedElement(), data(0) {}
};

class Camera
{
public:
	Camera();

    void setPosition (const v3 &_position)	{ position	= _position; }
	void setDirection(const v3& _direction)	{ direction	= _direction.getNormalized(); }
    void setUpVector (const v3& _upVector)	{ upVector	= _upVector.getNormalized(); }
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
	steel::time currentTime;
	steel::time frameLength;
};

struct ProcessInfo
{
	TimeInfo	timeInfo;
	Camera		camera;
};


#endif
