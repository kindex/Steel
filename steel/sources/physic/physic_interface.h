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
#include "../engine/game_object.h"
#include "../math/aabb.h"
#include "../math/matrix34.h"
#include "../math/vector3d.h"

struct Velocity
{
	v3 translation; // скорость поступательного движения
	v3 rotationAxis;  // ось вращения*скорость вращения(радиан/сек)

	inline Velocity(void) {}
	inline Velocity(const v3 _translation, const v3 _axis) 
	{
		translation = _translation;
		rotationAxis = _axis;
	}
};

class PhysicParticleInterface : public ChildrenInterface
{
public:
	virtual	void		setPosition(IN const v3) abstract;
	virtual	v3			getPosition() abstract;

	virtual void		setVelocity(IN const v3) abstract;
	virtual v3			getVelocity() abstract;

	virtual	void		setConfig(Config&) abstract;
};

#endif
