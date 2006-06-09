/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
    Licence:
        Только для Division
    Description:
************************************************************/

#ifndef PHYSIC_ENGINE_PS_H
#define PHYSIC_ENGINE_PS_H

#include "physic_engine.h"


class PhysicEnginePS: public PhysicEngine
{
public:
	bool process(steel::time globalTime, steel::time time);
	bool process(PhysicInterface &o, steel::time globalTime, steel::time time);

	bool prepare(PhysicInterface *object, matrix34 matrix = matrix34::getIdentity(), PhysicInterface *parent = NULL){ return true;}
	bool update(Element &element) { return true;}

	v3 calculateForce(PhysicInterface *object);
	v3 calculateForce(PhysicInterface *object1, PhysicInterface *object2);
};


#endif
