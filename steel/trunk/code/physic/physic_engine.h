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
#include "../math/matrix4x4.h"

#include "physic_interface.h"

class PhysicEngine: public Engine
{
protected:
	struct Element
	{
		PhysicInterface	*object, *parent;
		matrix44		matrix, parentMatrix; // abloslute
		Vertexes		*vertex;
		Triangles		*triangle;
		aabb			frame;
		int				collisionCount;
	};
	typedef Element* PElement;

	std::vector<Element> element;
	std::vector<PhysicInterface*> object;

public:
	// Collect information about object: object shape + velocity
	virtual bool inject(PhysicInterface *object);
	virtual bool prepare(PhysicInterface *object, matrix44 matrix = matrix44::getIdentity(), PhysicInterface *parent = NULL) = 0;
	// Move objects
	virtual bool process(steel::time globalTime, steel::time time) = 0; 
	// Clear collected information
	virtual bool clear();

	virtual bool init(std::string _conf);
	virtual bool deinit() { return clear();}
};

#endif

