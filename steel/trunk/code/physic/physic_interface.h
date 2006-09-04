/*id*********************************************************
	Unit: PhysicEngine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Физический джижок - интерфейс двигающегося объекта
 ************************************************************/

#ifndef PHYSIC_INTERFACE_H
#define PHYSIC_INTERFACE_H

#include "../engine.h"
#include "../math/aabb.h"
#include "../math/matrix34.h"
#include "../math/vector3d.h"
#include "../res/conf/conf.h"

typedef enum
{
	PROCESS_NONE,
	PROCESS_CUSTOM, // сам объект определяет своё движение
	PROCESS_UNI // движется по универсальным законам
} ProcessKind;

typedef enum
{
	COLLISION_NONE,
	COLLISION_POLYHEDRA,
	COLLISION_TRIGGER,
	COLLISION_PARTICLE, // sphere, collide with polyhedra
	COLLISION_SPHERE
} CollisionType;


struct velocity
{
	v3 translation; // скорость поступательного движения
	v3 rotationAxis;  // ось вращения*скорость вращения(радиан/сек)

	inline velocity() {}
	inline velocity(const v3 _translation, const v3 _axis) 
	{
		translation = _translation;
		rotationAxis = _axis;
	}
};

class PhysicObject;

typedef steel::svector<PhysicObject *> PhysicObjectList;

class PhysicObject: public Interface
{
//	steel::time	currentTime; // время, в котором находиться объект
	friend class PhysicEngine;
	friend class PhysicEngineSteel;

public:

// *** Common ***

	// список детей
	/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	// возвращает количество детей
	virtual int getPhysicChildrenCount(void) { return 0; } 
	// ребёнок с указанным номером
	virtual PhysicObject* getPhysicChildren(int number) { return NULL; }

	// Непосредственно перед добавлением в движок вызывается 
	virtual bool PhysicBeforeInject() { return true;}
	// После удаления из движка вызывается процедура afterRemove
	virtual void PhysicAfterRemove() {}

	// вызывается перед каждой итерацией обработки. Внутри этой процедуры объект может менять некоторые свои параметры
	virtual	void ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) {}


// *** Configuration ***


	// Форма объекта и способ проверки и обработки коллизий
	virtual CollisionType getCollisionType() { return COLLISION_NONE; }

	// Положение и поворот произвольной точки объекта в локальный координатах (точка отсчёта объекта). Именно за изменения этого параметра и отвечает физический движок.
	virtual	void setPosition(ObjectPosition const &newPosition) = 0;

	// скорость
	virtual velocity	getVelocity() = 0;
	virtual void		setVelocity(const velocity &v) = 0;
	// масса
	virtual	float	getMass() = 0;


/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
//	virtual aabb getPFrame() = 0; // AABB of object

	// *** Polyhedra ***
	virtual Vertexes*	getPVertexes() { return NULL; } // список вершин (координаты относительно getPosition() и всех матриц предков)
	// массив индексов вершин, которые образуют треугольники (грани)
	virtual Triangles*	getTriangles() { return NULL; }

	virtual Config* getPMaterial() { return NULL; }
	// эта функция вызывается, если другой объект трогает этот
	virtual void	trigger(PhysicObject *object) {}
};

#endif
