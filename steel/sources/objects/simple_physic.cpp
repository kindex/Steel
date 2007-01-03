/*id*********************************************************
	File: objects/simple_physic.cpp
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Физический аудио-объект.
 ************************************************************/

#include "simple_physic.h"
#include "game_object_factory.h"
#include "../graph/graph_interface.h"
#include "../audio/audio_interface.h"

SimplePhysic::SimplePhysic():
	type(SIMPLE_PHYSIC_NONE),
	object(NULL)
{}

bool SimplePhysic::InitFromConfig(Config* conf)
{
	if (conf == NULL)
	{
		return false;
	}

	std::string subclass = conf->gets("subclass");

	type = SIMPLE_PHYSIC_NONE;

	if (subclass == "rotation")
	{
		type = SIMPLE_PHYSIC_ROTATION;
		rotationAxis = conf->getv3("rotationAxis", v3(0.0f, 0.0f, 1.0f));
	} else
	if (subclass == "translation")
	{
		type = SIMPLE_PHYSIC_TRANSLATION;
	} else
	if (subclass == "scale")
	{
		type = SIMPLE_PHYSIC_SCALE;
	} else
	if (subclass == "path")
	{
		type = SIMPLE_PHYSIC_PATH;
	}
	if (subclass == "none")
	{
		type = SIMPLE_PHYSIC_NONE;
	}
	else
		error("movator", "undefined subclass " + subclass);

	object = createGameObject(conf->find("object"));
	speed = conf->getf("speed", 1.0f);

	startPosition.loadIdentity();
	startPosition.setTranslation(conf->getv3("origin"));

	return true;
}

void SimplePhysic::process(IN const ProcessInfo& info)
{
	ObjectPosition transformation;
	switch (type)
	{
	case SIMPLE_PHYSIC_ROTATION:
		transformation.setRotationAxis(info.curTime*speed, rotationAxis);
		break;
	default:
		transformation.loadIdentity();
		break;
	}
	currentPosition = transformation * startPosition;

	if (object != NULL)
	{
		object->process(info);
	}
}


void SimplePhysic::bindEngine(InterfaceId id, Engine* engine)
{
	if (object != NULL)
	{
		if (object->isSuportingInterface(id))
		{
			ChildrenInterface* childrenEngine = dynamic_cast<ChildrenInterface*>(engine);
			childrenEngine->addChild(object);
		} 
	}
}


bool SimplePhysic::isSuportingInterface(InterfaceId id)
{
	return true;
}


bool SimplePhysic::updateInformation(InterfaceId id, Engine* engine)
{
	if (object != NULL)
	{
		Interface3D* gengine = dynamic_cast<Interface3D*>(engine);
		gengine->setPosition(currentPosition);
		return true;
	}
	else
		return false;
}
