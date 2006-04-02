/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		TODO
 ************************************************************/

#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H

#include "../res/conf/conf.h"

#include "physic_interface.h"

struct PhysicElement
{
	PhysicInterface* obj;
	matrix4			matrix, parentMatrix; // abloslute
};

class PhysicEngine: public Engine
{
protected:
	std::vector<PhysicElement> objects;

public:
	// Collect information about object: object shape + velocity
	virtual bool inject(PhysicInterface *object, matrix4 matrix = matrix4());
	// Move objects
	virtual bool process(steel::time speed) = 0; 
	// Clear collected information
	virtual bool clear();

	virtual bool init(std::string _conf);
	virtual bool deinit() { return clear();}
};

#endif

