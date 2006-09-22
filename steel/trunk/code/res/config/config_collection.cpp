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

#include "../../steel.h"

#include "config_collection.h"
#include "../text/text_file.h"

using namespace std;

Config* ResCollectionConfig::addForce(std::string name, bool pop)
{
	std::string baseDirectory;
	splitPath(name, baseDirectory, name);

	resStack.push(baseDirectory);

	log_msg("res " + id, "Loading " + baseDirectory + "/" + name);

	std::string fullResName = baseDirectory + "/" + name;

	Config *obj = NULL;
	char *file = readfile(fullResName);
	int filei = 0;
	if(file)
	{
		obj = ParseConfig(file, filei);
		SkipSpaces(file, filei);
		if(file[filei] != '\0') 
			parse_error("config parser", string("Unxpected symbol '") + file[filei] + "'");
	}
	else
		log_msg("res error " + id, "Not found " + fullResName);

	if(obj)
	{
		ResStorage newResStorage;
		newResStorage.id = obj->getId();
		newResStorage.name = fullResName;
		newResStorage.links = 1;
		newResStorage.object = obj;

		data.push_back(newResStorage);

		index[fullResName] = data.size() - 1;
		resIndex[obj] = data.size() - 1;

		log_msg("res " + id, "OK " + fullResName);
		if(pop) resStack.pop();

		return obj;
	}
	else
	{
		log_msg("res error " + id, "Failed " + baseDirectory + "/" + name);
		if(pop) resStack.pop();

		return NULL;
	}
}
