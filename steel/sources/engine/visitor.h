/*id*********************************************************
	File: engine/visitor.h
	Unit: engine
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Visitor for game objects
 ************************************************************/
#ifndef OBJECT_VISITOR_H
#define OBJECT_VISITOR_H

#include "../steel.h"

class GameObject;
struct Particle;
struct matrix34;
typedef matrix34 ObjectPosition;

class Visitor
{
public:
    virtual bool visit(IN OUT GameObject*) { return true; }
    virtual void postvisit(IN OUT GameObject*, const ObjectPosition& base_position) {}
    virtual bool visit(IN OUT Particle*)  { return true; }
    virtual void postvisit(IN OUT Particle*) {}
};

#endif
