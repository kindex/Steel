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

#include "transformation_list.h"
#include "../../engine/engine_types.h"
#include "../../res/config/config.h"

bool TransformationList::InitFromConfig(IN Config& conf)
{
	Transformation::InitFromConfig(conf);

	ConfigArray* list = conf.getArray("transformations");
	if (list != NULL)
	{
		for EACH(ConfigArray, *list, it)
		{
			Transformation *newTransformation = transformationFactory((*it)->gets("class"));
			if (newTransformation != NULL)
			{
				if (newTransformation->InitFromConfig(**it))
				{
					transformations.push_back(newTransformation);
				}
				else
				{
					delete newTransformation;
				}
			}
		}
	}

	return true;
}

Config* TransformationList::getConfig() const
{
    return NULL; // TODO:
}

void TransformationList::process(IN const TimeInfo& info)
{
	TimeInfo newInfo = info;
	newInfo.currentTime = getT(info.currentTime);

	currentPosition.loadIdentity();
	for EACH(pvector<Transformation*>, transformations, it)
	{
		(*it)->process(newInfo);
		currentPosition = (*it)->getPosition()*currentPosition;
	}
}

TransformationList::~TransformationList()
{
	for EACH(pvector<Transformation*>, transformations, it)
	{
		delete *it;
	}
}

