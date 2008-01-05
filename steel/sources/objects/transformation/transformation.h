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
#include "../../engine/interface.h"
#include <string>
struct TimeInfo;
class Config;

class Transformation
{
public:
	Transformation();
	virtual ~Transformation() {}
	virtual bool InitFromConfig(IN Config&);
	virtual Config* getConfig() const abstract;
	virtual void process(IN const TimeInfo&) abstract;
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
