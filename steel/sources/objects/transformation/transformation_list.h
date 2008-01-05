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

#ifndef __OBJECTS__TRANSFORMATION_LIST_H__
#define __OBJECTS__TRANSFORMATION_LIST_H__

#include "../../steel.h"
#include "transformation.h"
#include "../../common/pvector.h"

class TransformationList : public Transformation
{
public:
	~TransformationList();
	bool InitFromConfig(IN Config&);
	Config* getConfig() const;
	void process(IN const TimeInfo&);

private:
	pvector<Transformation*> transformations;
};


#endif
