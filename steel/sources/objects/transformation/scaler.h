/*id*********************************************************
	File: objects/transformator/scaler.h
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

#ifndef __OBJECTS__SCALER_H__
#define __OBJECTS__SCALER_H__

#include "../../steel.h"
#include "transformation.h"

class Scaler : public Transformation
{
public:
	bool InitFromConfig(IN Config&);
	Config* getConfig() const;
	void process(IN const TimeInfo&);

private:
	v3 scale;
};


#endif
