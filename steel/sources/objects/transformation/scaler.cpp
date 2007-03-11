/*id*********************************************************
	File: objects/transformator/scaler.cpp
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

#include "scaler.h"
#include "../../res/config/config.h"

bool Scaler::InitFromConfig(IN Config& conf)
{
	Transformation::InitFromConfig(conf);

	scale = conf.getv3("scale", v3(1.0f, 1.0f, 1.0f));
	if (fabs(scale.y) < EPSILON) scale.y = scale.x;
	if (fabs(scale.z) < EPSILON) scale.z = scale.z;

	return true;
}

void Scaler::process(IN const TimeInfo& info)
{
	currentPosition.setScale(scale);
}

