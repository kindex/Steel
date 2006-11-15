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
#include "combiner.h"

using namespace std;

GameObject* findClass(const string &_class)
{
	if(_class == "sphere") return new Sphere;
	if(_class == "combiner") return new Combiner;

	return NULL;
}

GameObject	*createGameObject(Config *conf)
{
	if(conf == NULL) return NULL;
	string _class = conf->gets("class");

	GameObject *obj = findClass(_class);
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
	Config *objectsConfig = conf->find("objects");
	if(objectsConfig == NULL || objectsConfig->getType() != CONFIG_VALUE_ARRAY)
		return false;
	
	const ConfigArray &objectsArray =  *static_cast<ConfigArray*>(objectsConfig);
	for(ConfigArray::iterator it = objectsArray.begin(); it != objectsArray.end(); it++ )
	{
		GameObject *newObject = createGameObject(*it);
		objects.push_back(newObject);
	}

	return true;
}
