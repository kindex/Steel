/*id*********************************************************
	File: objects/transformator/movement.cpp
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		3D object movement
 ************************************************************/

#include "movement.h"
#include <cmath>

bool Movement::InitFromConfig(IN Config& conf)
{
	Transformation::InitFromConfig(conf);

	direction = conf.getv3("direction", v3(1.0f, 0.0f, 0.0f));
	std::string typeS = conf.gets("type", "forward");
	if (typeS == "forward")
	{
		type = MOVEMENT_FORWARD;
	} 
	else if (typeS == "sin")
	{
		type = MOVEMENT_SIN;
	} 
	else if (typeS == "shuttle")
	{
		type = MOVEMENT_SHUTTLE;
	} 
	else if (typeS == "frac")
	{
		type = MOVEMENT_FRAC;
	}
	else
	{
		error("object", "Unknown movement type '" + typeS + "'");
		type = MOVEMENT_FORWARD;
	}


	return true;
}

void Movement::process(IN const TimeInfo& info)
{
	float k = getT(info.currentTime);
	switch (type)
	{
	case MOVEMENT_SIN: k = sin(k); break;
//	case MOVEMENT_FRAC: k = frac(k); TODO:
	}
	currentPosition.setTranslation(k*direction);
}

