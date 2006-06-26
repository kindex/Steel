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

#include <string>

namespace ProcessKind
{
	typedef enum
	{
		none,
		custom, // сам объект определяет своё движение
		uni // движется по универсальным законам
	} ProcessKind;
};

namespace CollisionType
{
	typedef enum
	{
		none,
		polyhedra,
		trigger,
		particle1, // sphere, collide with polyhedra
		particle2, // sphere, collide with polyhedra & other particles
	} CollisionType;
};

namespace ShapeType
{
	typedef enum
	{
		none,
		poly,
		sphere
	} ShapeType;
}

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

class PhysicInterface;

typedef steel::svector<PhysicInterface *> PhysicObjectList;

class PhysicInterface: public Interface
{
//	steel::time	currentTime; // время, в котором находиться объект
	friend class PhysicEngine;
	friend class PhysicEngineSteel;

public:

	// список детей
	/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual PhysicObjectList* getPhysicChildrenList(void) { return NULL; }


	virtual bool beforeInject() { return true;}
	virtual void afterRemove() {}

	virtual CollisionType::CollisionType getCollisionType() = 0;

	virtual	void setPosition(ObjectPosition const &newPosition) = 0;
	virtual ProcessKind::ProcessKind getProcessKind() = 0;

	// скорость в глобальных коодринатах
	virtual velocity	getVelocity() = 0;
	virtual void		setVelocity(const velocity &v) = 0;


/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual Vertexes*	getPVertexes() = 0; // список вершин (координаты относительно getPosition() и всех матриц предков)
	// массив индексов вершин, которые образуют треугольники (грани)
	virtual Triangles*	getTriangles() = 0; 

	// масса
	virtual	coord	getMass() = 0;

// уникальный идентификатор объекта или пустая строка
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) {}

	virtual Config* getPMaterial() = 0;
	// эта функция вызывается, еслу другой объект трогает этот
	virtual void	trigger(PhysicInterface *object) {}
};

#endif

