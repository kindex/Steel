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
	struct Collision
	{
		v3	normal; // плоскость, в которой произошла коллизия
		v3	point; // точка коллизии
		float	time; // время (от начала карда) в процентах [0..1]
		Element *b; // учатник коллизии
	};


	v3 g;
//	std::map<std::string, int> tag;
public:
	bool process(steel::time globalTime, steel::time time);
	bool init(std::string _conf);
	bool prepare(PhysicInterface *object, matrix44 matrix = matrix44::getIdentity(), PhysicInterface *parent = NULL);
	bool clear();

	bool collisionDetection(Element &el, v3 distance, PElement &second);
	bool checkCollision(Element &a, v3 distance, Element &b, Collision &collision);
};


#endif
