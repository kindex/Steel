/*id*********************************************************
	File: objects/transformator/transformator.cpp
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
#include "transformation.h"
#include "transformation_list.h"
#include "rotation.h"
#include "movement.h"
#include "scaler.h"
#include "../../common/logger.h"
#include "../../common/utils.h"
#include "../../res/config/config.h"

Transformation* transformationFactory(const std::string& className)
{
	if (className == "rotation") return new Rotation;
	if (className == "movement") return new Movement;
	if (className == "scaler") return new Scaler;
	if (className == "transformation_list") return new TransformationList;

	error("objects", std::string("Transformation class '") + className + "' not found");
	return NULL;
}

bool Transformation::InitFromConfig(IN Config& conf)
{
	speed = conf.getf("speed", 1.0f);
	bias = conf.getf("bias", 0.0f);
	origin.setTranslation(conf.getv3("origin"));

	return true;
}

// function parameter (time, speed, bias)
float Transformation::getT(IN const float time)
{
	return time*speed + bias;
}

Transformation::Transformation():
	currentPosition(matrix34::getIdentity()),
	origin(matrix34::getIdentity())
{}

ObjectPosition Transformation::getPosition()
{
	return currentPosition*origin;
}
