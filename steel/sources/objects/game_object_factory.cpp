/*id*********************************************************
	File: objects/game_object_factory.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Создаватель игровый объектов при загрузке из конфигов
 ************************************************************/
#include "../steel.h"
#include "game_object_factory.h"

#include "../engine/game_object.h"
#include "../res/config/config.h"

#include "light.h"
#include "combiner/combiner.h"
#include "ps/particle_system.h"
#include "../common/logger.h"
#include "../common/utils.h"


GameObjectFactory* gameObjectFactory = NULL;

GameObject* GameObjectFactory::createGameObject(IN const std::string& className) const
{
	if (className == "combiner")return new Combiner;
	if (className == "ps")		return new ParticleSystem;
	if (className == "light")	return new GameLight;

	return NULL;
}

GameObject* createGameObject(Config* conf)
{
	if(conf == NULL) return NULL;

	GameObject* obj = gameObjectFactory->createGameObject(conf->gets("class"));
	if (obj == NULL) 
	{
		error("objects", std::string("GameObject class '") + conf->gets("class") + "' not found");
		return NULL;
	}

	v3 origin = conf->getv3("origin");
	ObjectPosition pos;
	pos.loadIdentity();
	pos.setTranslation(origin);

	bool result = obj->InitFromConfig(*conf);
	if (!result)
	{
		delete obj; obj = NULL;
	}

	return obj;
}
