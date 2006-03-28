/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс GModel является посредником между коллекцией ресурсов 
		и графическим интерфейсом
 ************************************************************/

#ifndef PHYSIC_PRIMITIVES_H
#define PHYSIC_PRIMITIVES_H

#include "physic_interface.h"
#include "../graph/graph_primitives.h"

class PDummy: public PhysicInterface
{
public:
	PhysicInterfaceList getPChildrens()	{		PhysicInterfaceList a;		return a;	}

	aabb getPFrame()	{		aabb A;		return A;	}
	bool cleanupP(){	return true;}
	Vertexes*	getPVertexes()	{			return NULL;	}
	Triangles*	getPTriangles()	{			return NULL;	}

	v3		getPosition() {return v3(); }
	bool	setPosition(v3 const &v) { return false; }

	// скорость
	v3		getVelocity() {return v3(0.0 ,0.0 ,0.0); }
	bool	setVelocity(v3 const &v){ return false; }
	// масса
	virtual	coord	getMass() {return 0; }

};


class PModel: public GModel, public PDummy
{
public:
	PhysicInterfaceList getPChildrens()
	{
		PhysicInterfaceList a;
		return a;
	}

	aabb getPFrame()	{		aabb A;		return A;	}
	bool cleanupP(){	return true;}
	Vertexes*	getPVertexes()
	{
		if(m)
			return &m->vertex;
		else
			return NULL;
	}
	Triangles*	getPTriangles()
	{
		if(m)
			return &m->triangle;
		else
			return NULL;
	}
};




#endif