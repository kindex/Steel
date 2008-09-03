/*id*********************************************************
	File: res/config/config_collection.cpp
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

#include "../../steel.h"

#include "config_collection.h"
#include "../text/text_file.h"
#include "../config/config_parser.h"

using namespace std;

Config* ResCollectionConfig::addForce(const std::string& name, bool pop)
{
	std::string baseDirectory;
	std::string newName;
	splitPath(name, baseDirectory, newName);
	resStack.push(baseDirectory);
	std::string fullResName = createPath(baseDirectory, newName);

	if (failedResourcesCache.find(fullResName) != failedResourcesCache.end())
	{
//		log_msg("res " + id, "Ignoring " + fullResName);
		if (pop) resStack.pop();
		return NULL;
	}

//	log_msg("res " + id, "Loading " + fullResName);

	Config* obj = NULL;
	TextFile* file = new TextFile();
	if (!file->init(newName, baseDirectory))
	{
		delete file;
		file = NULL;
	}

	if (file != NULL)
	{
		ConfigParser* parser = new ConfigParser();
		obj = parser->Parse(file);
		if (obj != NULL)
		{
			obj->setFilePath(fullResName);
		}
		delete parser;
	}
	else
	{
        if (getErrorMode())
        {
		    log_msg("res error " + id, "Not found " + fullResName);
        }
	}

	if (obj != NULL)
	{
		ResStorage newResStorage;
		newResStorage.id = obj->getId();
		newResStorage.name = fullResName;
		newResStorage.links = 1;
		newResStorage.object = obj;

		data.push_back(newResStorage);

		index[fullResName] = data.size() - 1;
		resIndex[obj] = data.size() - 1;

//		log_msg("res " + id, "OK " + fullResName);
		if(pop) resStack.pop();

		return obj;
	}
	else
    {
        if (getErrorMode())
        {
		    log_msg("res error " + id, "Failed " + baseDirectory + "/" + newName);
        }
        if (pop)
        {
            resStack.pop();
        }

		failedResourcesCache.insert(fullResName);
		return NULL;
	}
}
