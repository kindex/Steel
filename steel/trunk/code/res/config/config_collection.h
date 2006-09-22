/*id*********************************************************
	File: res/config/config_collection.cpp
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Коллекция конфигов
 ************************************************************/

#ifndef __RES__CONFIG_COLLECTION_H
#define __RES__CONFIG_COLLECTION_H

#include "../../steel.h"

#include "config.h"
#include "../res.h"

class ResCollectionConfig: public ResCollection<Config>
{
protected:
	Config* addForce(std::string name, bool pop);
};


#endif
