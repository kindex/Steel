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

#include "../engine/game_object.h"
#include "../res/config/config.h"

#include "scene.h"
#include "sphere.h"
#include "light.h"
#include "combiner/combiner.h"
#include "ps/particle_system.h"

using namespace std;

GameObject* findGameObject(const string &_class)
{
	if(_class == "scene") return new Scene;
	if(_class == "sphere") return new Sphere;
	if(_class == "combiner") return new Combiner;
	if(_class == "ps") return new ParticleSystem;
	if(_class == "light") return new GameLight;

	error("objects", string("GameObject class '") + _class + "' not found");
	return NULL;
}

GameObject	*createGameObject(Config *conf)
{
	if(conf == NULL) return NULL;
	string _class = conf->gets("class");

	GameObject *obj = findGameObject(_class);
	if(obj == NULL) return NULL;

	v3 origin =  conf->getv3("origin");
	ObjectPosition pos;
	pos.loadIdentity();
	pos.setTranslation(origin);
//	obj->setPosition(pos);

	bool result = obj->InitFromConfig(conf);
	if(!result)
	{
		delete obj; obj = NULL;
	}

	return obj;
}


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

void Scene::process(ProcessInfo &info)
{
	for EACH(pvector<GameObject*>, objects, it)
	{
		(*it)->process(info);
	}
}
