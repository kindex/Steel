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

bool Rotation::InitFromConfig(IN Config& conf)
{
	Transformation::InitFromConfig(conf);

	rotationAxis = conf.getv3("rotationAxis", v3(0.0f, 0.0f, 1.0f));

	return true;
}

void Rotation::process(IN const ProcessInfo& info)
{
	currentPosition.setRotationAxis(getT(info.curTime), rotationAxis);
}

