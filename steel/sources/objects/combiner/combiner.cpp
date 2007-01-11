/*id*********************************************************
	File: objects/combiner/combiner.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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

using namespace std;

Combiner::Combiner(): 
	graph(NULL), 
	audio(NULL),
	transformation(NULL)
{}


bool Combiner::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;

	origin.loadIdentity();
	origin.setTranslation(conf->getv3("origin"));
	position.loadIdentity();
	positionKind = conf->gets("position_kind", "local") == "local" ? POSITION_LOCAL : POSITION_GLOBAL;

	Config *graphConf = conf->find("graph");
	if(graphConf != NULL)
	{
		string graphClass = graphConf->gets("class");
		graph = findGraphObject(graphClass);
		if(graph != NULL)
		{
			bool ok =  graph->InitFromConfig(graphConf);
			if(!ok)
			{
				delete graph; 
				graph = NULL; 
			}
		}
	}

	Config *audioConf = conf->find("audio");
	if(audioConf != NULL)
	{
		string audioClass = audioConf->gets("class");
		audio = findAudioObject(audioClass);
		if(audio != NULL)
		{
			bool ok =  audio->InitFromConfig(audioConf);
			if(!ok)
			{
				delete audio; 
				audio = NULL; 
			}
		}
	}

	Config* transformationConf = conf->find("transformation");
	if (transformationConf != NULL)
	{
		string transformationClass = transformationConf->gets("class");
		transformation = transformationFactory(transformationClass);
		if (transformation != NULL)
		{
			bool ok = transformation->InitFromConfig(transformationConf);
			if (!ok)
			{
				delete transformation;
				transformation = NULL;
			}
			else
			{
				position = transformation->getPosition();
			}
		}
	}

	ConfigArray* objectsConfig = conf->getArray("objects");
	if(objectsConfig != NULL)
	{
		const ConfigArray &objectsArray =  *static_cast<ConfigArray*>(objectsConfig);
		for EACH_CONST(ConfigArray, objectsArray, it)
		{
			GameObject *newObject = createGameObject(*it);
			if (newObject != NULL)
			{
				objects.push_back(newObject);
			}
		}
	}

	return true;
}

bool Combiner::isSuportingInterface(InterfaceId id) 
{ 
	if (id == GraphInterface::interfaceId ||
		id == AudioInterface::interfaceId)
	{
		return true;
	}
	for EACH(pvector<GameObject*>, objects, it)
	{
		if((*it)->isSuportingInterface(id)) return true;
	}
	return false;
}

void Combiner::bindEngine(InterfaceId id, Engine* engine)
{
	dynamic_cast<BaseInterface*>(engine)->setCurrentObject(this);

	if(id == GraphInterface::interfaceId && graph != NULL)
	{
		dynamic_cast<GraphInterface*>(engine)->setPositionKind(positionKind);
		dynamic_cast<GraphInterface*>(engine)->setPosition(position*origin);
		graph->bindEngine(id, engine);
	}
	if(id == AudioInterface::interfaceId && audio != NULL)
	{
//		dynamic_cast<AudioInterface*>(engine)->setPositionKind(positionKind);
//		dynamic_cast<AudioInterface*>(engine)->setPosition(position);
		audio->bindEngine(id, engine);
	}

	ChildrenInterface &cEngine = *dynamic_cast<ChildrenInterface*>(engine);
	cEngine.clearChildren();

	for EACH(pvector<GameObject*>, objects, it)
	{
		if ((*it)->isSuportingInterface(id)) 
		{
			cEngine.addChild(*it);
		}
	}
}

bool Combiner::updateInformation(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId && graph != NULL)
	{
		dynamic_cast<GraphInterface*>(engine)->setPosition(position*origin);
		return graph->updateInformation(id, engine);
	}
	// test audio update
	if (id == AudioInterface::interfaceId && audio != NULL)
	{
		dynamic_cast<AudioInterface*>(engine)->setPosition(position*origin);
		return audio->updateInformation(id, engine);
	}
	return false;
}

void Combiner::process(IN const ProcessInfo& info)
{
	if (transformation != NULL)
	{
		transformation->process(info);
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
