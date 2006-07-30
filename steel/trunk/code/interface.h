/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Предок для интерфейтов объектов, которые имеют положение
		(глобальную, локальную систему координат)
************************************************************/
#ifndef INTERFACE_H
#define INTERFACE_H

#include "math/vector3d.h"
#include "math/matrix34.h"

#include "common/steel_vector.h"
#include "steel.h"

// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];
};

typedef unsigned int	uid;// уникальный идентификатор модели

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
	steel::vector<v3> data;
	Vertexes() {}
};

// массив треугольников многогранника
struct Triangles: public BufferedElement
{
	steel::vector<Triangle> data;
};

/*
местоположение + поворот объекта
*/
typedef matrix34 ObjectPosition;

// Тип положения объекта: local – относительно родитиля, global – в глобальной системе координат.
typedef enum
{
	POSITION_NONE,
	POSITION_LOCAL,
	POSITION_GLOBAL
} PositionKind;

class IdGenerator
{
protected:
	uid freeId;
public:
	IdGenerator() { freeId = 1; }
	uid genUid();
};

extern IdGenerator objectIdGenerator;

/*
Interface protottype
От этого класса наследуются все объекты и интерфейсы в движке.

Объект этого класса:
* имеет положение (в голобадьных или локальных координатах). 
Положение - матрица 3х4, в которой определяет сдвиг, поморот и масштабирование тела
относительно его родителя или в глобальный коорлинатах. Систему отсчёта определяет 
функция getPositionKind
*/
class Interface;

typedef steel::svector<Interface *> ObjectList;

typedef int ModificationTime;

extern ModificationTime globalFrameNumber;

class Interface: public steelAbstract
{
protected:
	uid id;
	ModificationTime modificationTime;
public:
	// дефолтовый конструктор присваюивает уникальный идентификатор.
	Interface() { id = objectIdGenerator.genUid(); modificationTime = 0; }

	// Object unique identifier
	virtual uid								getId()				{ return id; }
	// Местоположение и поворот произвольной точки объекта в локальных или глобальных координатах (точка отсчёта объекта).
	virtual	ObjectPosition					getPosition(void) = 0;
	// Система координат: локальная относительно родителя или глобальная
	virtual PositionKind		getPositionKind(void) { return POSITION_LOCAL; }

	// когда менялся список детей
	virtual ModificationTime getChildrenModificationTime(void) { return modificationTime; }

	// когда менялось состяние объектов (то, что с warn в design document)
	// когда хоть один из параметров изменил своё значение
	// скорость может меняться	
	virtual ModificationTime getModificationTime(void) { return modificationTime; }
};

#endif
