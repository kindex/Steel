/*id*********************************************************
	File: objects/transformator/rotation.h
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

#ifndef __OBJECTS__ROTATION_H__
#define __OBJECTS__ROTATION_H__

#include "../../steel.h"
#include "transformation.h"

class Rotation : public Transformation
{
public:
	bool InitFromConfig(IN Config&);
	void process(IN const TimeInfo&);
	Config* getConfig() const;

private:
	v3 rotationAxis;
};


#endif
