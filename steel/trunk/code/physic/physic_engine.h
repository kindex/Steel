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

class PhysicEngine: public Engine
{
protected:
/*	struct Element
	{
		Vertexes	*vertex;
		Triangles	*triangle;
		coord		mass;
		v3			velocity;
//		MATRIX4X4	matrix;
		v3			position;
	};
*/
	// data, to store collected information
//	std::vector<Element> elements;
	std::vector<PhysicInterface*> objects;
	v3 g;

public:
	// Collect information about object: object shape + velocity
	virtual bool inject(PhysicInterface *object);
	// Move objects
	bool process(time speed); 
	// Clear collected information
	bool clear();

	bool init(std::string _conf);
	bool deinit() { return clear();}
};

#endif

