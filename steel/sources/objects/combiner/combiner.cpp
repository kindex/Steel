/*id*********************************************************
	File: objects/combiner/combiner.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Компоновщик графических и физических объектов.
		Позволяет в одном объекте (combiner) объединять свойства
		одного физического объекта [и одного графического].
 ************************************************************/

#include "../../steel.h"
#include "combiner.h"
#include "../game_object_factory.h"
#include "../../audio/audio_interface.h"
#include "graph_object.h"
#include "audio_object.h"
#include "../transformation/transformation.h"

using namespace std;

Combiner::Combiner(): 
	graph(NULL), 
	audio(NULL),
	transformation(NULL)
{}


bool Combiner::InitFromConfig(Config& conf)
{
	origin.loadIdentity();
	origin.setScale(conf.getv3("scale", v3(1,1,1)));
	origin.setTranslation(conf.getv3("origin"));

	position.loadIdentity();
	positionKind = conf.gets("position_kind", "local") == "local" ? POSITION_LOCAL : POSITION_GLOBAL;

	Config* graphConf = conf.find("graph");
	if (graphConf != NULL)
	{
		string graphClass = graphConf->gets("class");
		graph = graphObjectFactory(graphClass);
		if (graph != NULL)
		{
			bool ok = graph->InitFromConfig(*graphConf);
			if (!ok)
			{
				delete graph; 
				graph = NULL; 
			}
		}
	}

	Config* audioConf = conf.find("audio");
	if(audioConf != NULL)
	{
		string audioClass = audioConf->gets("class");
		audio = audioObjectFactory(audioClass);
		if(audio != NULL)
		{
			bool ok =  audio->InitFromConfig(*audioConf);
			if(!ok)
			{
				delete audio; 
				audio = NULL; 
			}
		}
	}

	Config* transformationConf = conf.find("transformation");
	if (transformationConf != NULL)
	{
		string transformationClass = transformationConf->gets("class");
		transformation = transformationFactory(transformationClass);
		if (transformation != NULL)
		{
			bool ok = transformation->InitFromConfig(*transformationConf);
			if (!ok)
			{
				delete transformation;
				transformation = NULL;
			}
			else
			{
				TimeInfo info(0.0f, 0.0f);
				transformation->process(info);
				position = transformation->getPosition();
			}
		}
	}

	ConfigArray* objectsConfig = conf.getArray("objects");
	if (objectsConfig != NULL)
	{
		const ConfigArray& objectsArray = *static_cast<ConfigArray*>(objectsConfig);
		for EACH_CONST(ConfigArray, objectsArray, it)
		{
			GameObject* newObject = createGameObject(*it);
			if (newObject != NULL)
			{
				objects.push_back(newObject);
			}
		}
	}

	return true;
}

bool Combiner::isSuportingInterface(Engine& engine) 
{ 
	if (engine.isSupportingInterface(INTERFACE_GRAPH) ||
		engine.isSupportingInterface(INTERFACE_AUDIO))
	{
		return true;
	}
	for EACH(pvector<GameObject*>, objects, it)
	{
		if ((*it)->isSuportingInterface(engine))
		{
			return true;
		}
	}
	return false;
}

void Combiner::bindEngine(Engine& engine)
{
	engine.setCurrentObject(this);
// TODO:
	if (engine.isSupportingInterface(INTERFACE_GRAPH) && graph != NULL)
	{
		dynamic_cast<GraphInterface*>(&engine)->setPositionKind(positionKind);
		dynamic_cast<GraphInterface*>(&engine)->setPosition(position*origin);
		graph->bindEngine(engine);
	}
	if (engine.isSupportingInterface(INTERFACE_AUDIO) && audio != NULL)
	{
//		dynamic_cast<AudioInterface*>(engine)->setPositionKind(positionKind);
//		dynamic_cast<AudioInterface*>(engine)->setPosition(position);
		audio->bindEngine(engine);
	}

	ChildrenInterface &cEngine = *dynamic_cast<ChildrenInterface*>(&engine);
	cEngine.clearChildren();

	for EACH(pvector<GameObject*>, objects, it)
	{
		if ((*it)->isSuportingInterface(engine)) 
		{
			cEngine.addChild(*it);
		}
	}
}

bool Combiner::updateInformation(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
		dynamic_cast<GraphInterface*>(&engine)->setPosition(position*origin);
		if (graph != NULL)
		{
			return graph->updateInformation(engine);
		}
	}
	// test audio update
	if (engine.isSupportingInterface(INTERFACE_AUDIO))
	{
		dynamic_cast<AudioInterface*>(&engine)->setPosition(position*origin);
		if (audio != NULL)
		{
			return audio->updateInformation(engine);
		}
	}
	return false;
}

void Combiner::process(IN const ProcessInfo& info)
{
	if (transformation != NULL)
	{
		transformation->process(info.timeInfo);
		position = transformation->getPosition();
	}
	if (graph != NULL)
	{
		graph->process(info);
	}
	if (audio != NULL)
	{
		audio->process(info);
	}
	for EACH(pvector<GameObject*>, objects, it)
	{
		(*it)->process(info);
	}
}
