#include "light.h"

bool GameLight::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;

	v3 origin =  conf->getv3("origin");
	position.loadIdentity();
	position.setTranslation(origin);

	light = new Light;
	light->position.loadZero();

	light->constantAttenuation = conf->getf("constantAttenuation", 0.0f);
	light->sqrtAttenuation = conf->getf("sqrtAttenuation", 0.0f);
	light->linearAttenuation = conf->getf("linearAttenuation", 1.0f);
	light->quadraticAttenuation = conf->getf("quadraticAttenuation", 0.0f);

	light->ambient = conf->getv3("ambient", v3(0.0f, 0.0f, 0.0f));
	light->diffuse = conf->getv3("diffuse", v3(1.0f, 1.0f, 1.0f));
	light->specular = conf->getv3("specular", v3(1.0f, 1.0f, 1.0f));

	light->minDistance = conf->getf("minDistance");
	light->maxDistance = conf->getf("maxDistance");
	light->k = conf->getf("k");

	bool enabled = conf->geti("enabled", 1) > 0;
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

bool GameLight::isSuportingInterface(InterfaceId id)
{
	return id == GraphInterface::interfaceId;
}

void GameLight::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId)
	{
		this->engine = dynamic_cast<GraphInterface*>(engine);
		this->engine->setPosition(position);
		if (enabled) this->engine->addLight(light);
	}
}

void GameLight::afterRemove(InterfaceId, Engine*)
{
	engine = NULL;
}


bool GameLight::updateInformation(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId)
	{
/*		matrix34 rot = matrix34::CreateRotationMatrix(0.001f, v3(0.0f, 0.0f, 1.0f));
		v3 pos = position.getTranslation();
		pos = rot * pos;
		position.setTranslation(pos);*/

		GraphInterface &gengine = *dynamic_cast<GraphInterface*>(engine);
		gengine.setPosition(position);
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
