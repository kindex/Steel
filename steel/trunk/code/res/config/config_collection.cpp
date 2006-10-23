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
#include "../config/config_parser.h"

using namespace std;

Config* ResCollectionConfig::addForce(std::string name, bool pop)
{
	std::string baseDirectory;
	splitPath(name, baseDirectory, name);
	resStack.push(baseDirectory);
	std::string fullResName = createPath(baseDirectory, name);

	log_msg("res " + id, "Loading " + fullResName);

	Config *obj = NULL;
	TextFile *file = new TextFile();
	if(!file->init(name, baseDirectory)) delete file, file = NULL;
	if(file != NULL)
	{
		ConfigParser* parser = new ConfigParser();
		obj = parser->Parse(file);
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
