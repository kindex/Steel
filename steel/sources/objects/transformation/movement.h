/*id*********************************************************
	File: objects/transformator/movement.h
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

#ifndef __OBJECTS__MOVEMENT_H__
#define __OBJECTS__MOVEMENT_H__

#include "../../steel.h"
#include "transformation.h"

class Movement : public Transformation
{
public:
	bool InitFromConfig(IN Config&);
	void process(IN const TimeInfo&);

private:
	v3 direction;

	enum MovementType
	{
		MOVEMENT_FORWARD,
		MOVEMENT_SIN,
		MOVEMENT_SHUTTLE,
		MOVEMENT_FRAC
	};
	MovementType type;
};


#endif
