/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Предок для интерфейтов объектов, которые имеют положение
		(глобальную, локальную систему координат)
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

typedef int	uid;// уникальный идентификатор модели

// Массив значений, который может быть кеширован в графическом ускорителе
class BufferedElement
{
public:
	// true, если этот массив был изменён с прошлого раза
	bool	changed;
	// уникальный идентификатор для массива данных
	uid		id;
public:
	uid		getId() const { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged() const { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }
};

// массив вершин многогранника
struct Vertexes: public BufferedElement
{
	std::vector<v3> data;
	Vertexes(): data(0) {}
};

// массив треугольников многогранника
struct Triangles: public BufferedElement
{
	std::vector<Triangle> data;
};

/*
Interface protottype
От этого класса наследуются все объекты и интерфейсы в движке.

Объект этого класса:
* имеет положение (в голобадьных или локальных координатах). 
Положение - матрица 4х4, в которой определяет сдвиг, поморот и масштабирование тела
относительно его родителя или в глобальный коорлинатах. Систему отсчёта определяет 
функция getPositionKind
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

