#include "light.h"

bool GameLight::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;

	v3 origin =  conf->getv3("origin");
	position.loadIdentity();
	position.setTranslation(origin);

	light = new Light;
	light->position.loadZero();
	light->color.set(1.0f, 1.0f, 1.0f, 1.0f);

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
		GraphInterface &gengine = *dynamic_cast<GraphInterface*>(engine);
		gengine.setPosition(position);
		gengine.addLight(light);
	}
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
