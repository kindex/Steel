/*id*********************************************************
	File: objects/transformator/transformator.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		3D object abstract transfotmation
 ************************************************************/

#ifndef __OBJECTS__TRANSFORMATOR_H__
#define __OBJECTS__TRANSFORMATOR_H__

#include "../../steel.h"
#include "../../engine/game_object.h"

class Transformation
{
public:
	virtual bool InitFromConfig(Config*) = 0;
	virtual void process(IN const ProcessInfo& info) = 0;
	virtual ObjectPosition getPosition() = 0;
};

Transformation* transformationFactory(const std::string& className);


#endif
