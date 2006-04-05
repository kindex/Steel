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

#ifndef PHYSIC_ENGINE_3D_H
#define PHYSIC_ENGINE_3D_H

#include "physic_engine.h"

#include <map>
#include <string>

class PhysicEngine3D: public PhysicEngine
{
protected:
	v3 g;
	std::map<std::string, int> tag;
public:
	bool process(steel::time speed);
	bool init(std::string _conf);
	bool inject(PhysicInterface *object, matrix4 matrix = matrix4(), PhysicInterface *parent = NULL);
	bool clear();
};


#endif
