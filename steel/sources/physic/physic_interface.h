/*id*********************************************************
	File: physic/physic_interface.h
	Unit: physic engine
	Part of: Steel engine
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Физический джижок - интерфейс двигающегося объекта
 ************************************************************/

#ifndef PHYSIC_INTERFACE_H
#define PHYSIC_INTERFACE_H

#include "../steel.h"
#include "../engine/engine.h"
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

	inline velocity(void) {}
	inline velocity(const v3 _translation, const v3 _axis) 
	{
		translation = _translation;
		rotationAxis = _axis;
	}
};

class PhysicInterface : public BaseInterface, public ChildrenInterface, public Interface3D
{
public:
	static const InterfaceId interfaceId = 0x400;

	virtual	void			setPosition(IN const ObjectPosition&) abstract;
	virtual	ObjectPosition	getPosition() abstract;

	virtual void			setVelocity(const velocity &v) abstract;
	virtual velocity		getVelocity() abstract;
};

#endif
