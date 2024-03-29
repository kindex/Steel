/*id*********************************************************
	File: objects/transformator/rotation.cpp
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		3D object rotation
 ************************************************************/

#include "rotation.h"
#include "../../engine/engine_types.h"
#include "../../res/config/config.h"

bool Rotation::InitFromConfig(IN Config& conf)
{
	Transformation::InitFromConfig(conf);

	rotationAxis = conf.getv3("rotationAxis", v3(0.0f, 0.0f, 1.0f));

	return true;
}

Config* Rotation::getConfig() const
{
    return NULL; // TODO:
}

void Rotation::process(IN const TimeInfo& info)
{
	currentPosition.setRotationAxis(getT(info.currentTime), rotationAxis);
}

