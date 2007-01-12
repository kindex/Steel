/*id*********************************************************
	File: objects/transformator/transformator.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
	Transformation();
	virtual ~Transformation() {}
	virtual bool InitFromConfig(IN Config&);
	virtual void process(IN const ProcessInfo&) = 0;
	virtual ObjectPosition getPosition();

protected:
	float getT(IN const float time);

	ObjectPosition currentPosition;
	ObjectPosition origin;
	float speed;
	float bias;
};

Transformation* transformationFactory(const std::string& className);


#endif
