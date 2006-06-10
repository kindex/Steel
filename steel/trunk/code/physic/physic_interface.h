/*id*********************************************************
    Unit: PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
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

class PhysicInterface;
typedef steel::vector<PhysicInterface*> PhysicInterfaceList;

class PhysicEngine;

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

class PhysicInterface: public Interface
{
	int storageId;
//	steel::time	currentTime; // время, в котором находиться объект
	friend class PhysicEngine;
	friend class PhysicEngine3D;

public:
	virtual bool beforeInject() { return true;}
	virtual void afterRemove() {}
	// возвращает true, если хоть один из параметров изменил своё значение не
	// через вызовы функций set* (mass, position, material, mesh)
	// скорость может меняться	
	virtual bool wasChanged() { return false; }

	virtual void setStorageId(const int _id) { storageId = _id; }
	virtual int getStorageId(void) { return storageId; }

	virtual CollisionType::CollisionType getCollisionType() = 0;

	virtual	ObjectPosition getGlobalPosition(void) = 0;
	virtual	void setPosition(ObjectPosition const &newPosition) = 0;
	virtual ProcessKind::ProcessKind getProcessKind() = 0;
	virtual	void setPositionKind(PositionKind::PositionKind const newPositionKind) = 0;

	// скорость в глобальных коодринатах
	virtual velocity	getVelocity() = 0;
	virtual void		setVelocity(const velocity &v) = 0;
	virtual velocity	getGlobalVelocity() = 0;

	PhysicInterface() {}
/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual PhysicInterfaceList getPChildrens() = 0;

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual Vertexes*	getPVertexes() = 0; // список вершин (координаты относительно getPosition() и всех матриц предков)
	// массив индексов вершин, которые образуют треугольники (грани)
	virtual Triangles*	getTriangles() = 0; 

	virtual	float		getGlobalScale() = 0;

	// масса
	virtual	coord	getMass() = 0;

// уникальный идентификатор объекта или пустая строка
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) = 0;
//	virtual std::string getMaterial() = 0;

	virtual Config* getPMaterial() = 0;
	// эта функция вызывается, еслу другой объект трогает этот
	virtual void	trigger(PhysicInterface *object) = 0;
};

#endif

