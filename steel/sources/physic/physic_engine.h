/*id*********************************************************
	File: physic/physic_engine.h
	Unit: physic engine
	Part of: Steel engine
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Абстрактный физический движок
		Обрабатывает движение объектов.
		Поддерживает ирерархию объектов, кеширует информацию об	объектах.
************************************************************/

#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H

#include "../steel.h"
#include <map>
#include "../res/conf/conf.h"
#include "physic_interface.h"

class PhysicEngine: public Engine, public PhysicInterface
{
public:
	virtual bool init(Config* _conf);

	// обрабатывает движение всех объектов
	virtual void process(IN const ProcessInfo&);
	virtual bool clear(); 

	virtual bool inject(PhysicObject *object);
	virtual bool remove(PhysicObject *object);

	virtual Shadow* getShadowClass(GameObject *object) = 0;
};

#endif

