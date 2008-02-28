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
#include "ps/animator.h"
#include "ps/emitter.h"
#include "ps/ps_renderer.h"
#include "../common/logger.h"
#include "../common/utils.h"

GameObjectFactory* gameObjectFactory = NULL;

ParticleAnimator* GameObjectFactory::createParticleAnimator(IN const std::string& _class) const
{
	if (_class == "simple") return new SimpleAnimator;

	error("ps", std::string("ParticleAnimator class '") + _class + "' not found");
	return NULL;
}

ParticleEmitter* GameObjectFactory::createParticleEmitter(const std::string& _class) const
{
	if (_class == "simple") return new SimpleEmitter;

	error("ps", std::string("ParticleEmitter class '") + _class + "' not found");
	return NULL;
}

ParticleRenderer* GameObjectFactory::createParticleRenderer(const std::string& _class) const
{
	if (_class == "sprite") return new SpriteRenderer;
	if (_class == "object") return new ObjectPSRenderer;
	if (_class == "dummy")  return new DummyPSRenderer;

	error("ps", std::string("ParticleRenderer class '") + _class + "' not found");
	return NULL;
}


GameObject* GameObjectFactory::createGameObject(IN const std::string& className) const
{
	if (className == "combiner")return new Combiner;
	if (className == "ps")		return new ParticleSystem;
	if (className == "light")	return new GameLight;

	return NULL;
}

GameObject* createGameObject(Config* conf, OUT std::string* _class)
{
	if (conf == NULL)
    {
        return NULL;
    }
    if (!conf->getb("enabled", true))
    {
        return NULL;
    }

	std::string conf_class = conf->gets("class");
	if (_class != NULL)
	{
		*_class = conf_class;
	}
	GameObject* obj = gameObjectFactory->createGameObject(conf_class);
	if (obj == NULL) 
	{
		error("objects", std::string("GameObject class '") + conf_class + "' not found");
		return NULL;
	}

	v3 origin = conf->getv3("origin");
	ObjectPosition pos;
	pos.loadIdentity();
	pos.setTranslation(origin);

	bool result = obj->InitFromConfig(*conf);
	if (!result)
	{
		delete obj;
        obj = NULL;
	}

	return obj;
}
