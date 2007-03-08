/*id*********************************************************
	File: objects/light.cpp
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Источник освещения.
 ************************************************************/
#include "../steel.h"
#include "light.h"
#include "../res/res_main.h"

bool GameLight::InitFromConfig(Config& conf)
{
	v3 origin =  conf.getv3("origin");
	position.loadIdentity();
	position.setTranslation(origin);

	light = new Light;
	light->position.loadZero();

	light->constantAttenuation = conf.getf("constantAttenuation", 0.0f);
	light->sqrtAttenuation = conf.getf("sqrtAttenuation", 0.0f);
	light->linearAttenuation = conf.getf("linearAttenuation", 1.0f);
	light->quadraticAttenuation = conf.getf("quadraticAttenuation", 0.0f);

	light->ambient = conf.getv3("ambient", v3(0.0f, 0.0f, 0.0f));
	light->diffuse = conf.getv3("diffuse", v3(1.0f, 1.0f, 1.0f));
	light->specular = conf.getv3("specular", v3(1.0f, 1.0f, 1.0f));

	light->minDistance = conf.getf("minDistance", 1.0f);
	light->maxDistance = conf.getf("maxDistance", 10.0f);
	light->k = conf.getf("k", 1.0f);
	light->up = conf.getv3("up", v3(0.0f, 0.0f, 1.0f));
	light->direction = conf.getv3("direction", v3(1.0f, 0.0f, 0.0f));

	light->castShadows = conf.getb("castShadows", true);

	light->cubeMap = resImage.add(conf.getPath("cube_map"));

	bool enabled = conf.getb("enabled", true);
	if (enabled)
	{
		enable();
	}
	else
	{
		disable();
	}

	return true;
}


void GameLight::bindEngine(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
		this->engine = dynamic_cast<GraphInterface*>(&engine);
		this->engine->setPosition(position);
		if (enabled && light != NULL)
		{
			this->engine->addLight(light);
		}
	}
}

void GameLight::afterRemove(Engine&)
{
	engine = NULL;
}


bool GameLight::updateInformation(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
		dynamic_cast<GraphInterface*>(&engine)->setPosition(position);
		return true;
	}
	return false;
}

void GameLight::enable()
{
	if (!enabled)
	{
		if (engine != NULL) engine->addLight(light);
		enabled = true;
	}
}

void GameLight::disable()
{
	if (enabled)
	{
		if (engine != NULL) engine->removeLight(light->id);
		enabled = false;
	}
}

void GameLight::toggleEnable()
{
	if (enabled) 
	{
		disable();
	}
	else 
	{
		enable();
	}
}

void GameLight::setPosition(const v3& _position, 
							const v3& _direction,
							const v3& _up)
{
	position.setTranslation(_position);
	light->direction = _direction;
	light->up = _up;
}
