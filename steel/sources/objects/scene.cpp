/*id*********************************************************
	File: objects/scene.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Загрузка уровня из *.scene файла.
 ************************************************************/

#include "../steel.h"
#include <string>

#include "game_object_factory.h"
#include "scene.h"


using namespace std;

bool Scene::InitFromConfig(Config *conf)
{
	ConfigArray *objectsConfig = conf->getArray("objects");
	if(objectsConfig == NULL)
		return false;
	
	const ConfigArray &objectsArray =  *static_cast<ConfigArray*>(objectsConfig);
	for EACH_CONST(ConfigArray, objectsArray, it)
	{
		GameObject *newObject = createGameObject(*it);
		if (newObject != NULL)
		{
			objects.push_back(newObject);
		}
	}

	return true;
}

bool Scene::isSuportingInterface(InterfaceId id)
{
	for EACH(pvector<GameObject*>, objects, it)
		if((*it)->isSuportingInterface(id)) return true;

	return false;
}

void Scene::bindEngine(InterfaceId id, Engine* egnine)
{
	dynamic_cast<ChildrenInterface*>(egnine)->clearChildren();

	ChildrenInterface &gEngine = *dynamic_cast<ChildrenInterface*>(egnine);
	for EACH(pvector<GameObject*>, objects, it)
	{
		if ((*it)->isSuportingInterface(id)) 
		{
			gEngine.addChild(*it);
		}
	}
}

void Scene::process(IN const ProcessInfo& info)
{
	for EACH(pvector<GameObject*>, objects, it)
	{
		(*it)->process(info);
	}
}
