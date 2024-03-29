/*id*********************************************************
	File: res/config/config_collection.h
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Коллекция конфигов
 ************************************************************/

#ifndef __RES__CONFIG_COLLECTION_H
#define __RES__CONFIG_COLLECTION_H

#include "../../steel.h"
#include "../res_collection.h"
class Config;

class ResCollectionConfig : public ResCollection<Config>
{
protected:
	Config* addForce(const std::string& name, bool pop);
};

extern ResCollectionConfig resConfig;

#endif
