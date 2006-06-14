/*id*********************************************************
    Unit: PhysicEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		TODO
 ************************************************************/

#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H

#include "../res/conf/conf.h"

#include "physic_interface.h"

class PhysicEngine: public Engine
{
protected:
	steel::vector<PhysicInterface*> object;
	v3 g;

public:
	struct TotalInfo
	{
		int collisionCount;
	} total;

public:
	// Inject добавляет объект для обработки
	// Типы движений объектов: uni, custom, none
	// У uni не может быть детей
	// У custom и none не может быть детей с типом uni
	virtual bool inject(PhysicInterface *object);
	virtual bool remove(PhysicInterface *object);

	virtual bool prepare(PhysicInterface *object, matrix34 matrix = matrix34::getIdentity(), PhysicInterface *parent = NULL) = 0;

	// Move objects
	virtual bool process(steel::time globalTime, steel::time time) = 0; 
	// Clear collected information
	virtual bool clear();

	virtual bool init(std::string _conf);
	virtual bool deinit() { return clear();}

	virtual void setGravitation(const v3 force) { g = force; }

};

#endif

