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

using namespace std;

bool Combiner::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;

	position.loadIdentity();
	position.setTranslation(conf->getv3("origin"));
	positionKind = conf->gets("position_kind", "global") == "local"?POSITION_LOCAL:POSITION_GLOBAL;

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

	return true;
}

bool Combiner::isSuportingInterface(InterfaceId id) 
{ 
	return 
		id == GraphInterface::interfaceId ||
		id == AudioInterface::interfaceId;
}

void Combiner::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId && graph != NULL)
	{
		dynamic_cast<GraphInterface*>(engine)->setPositionKind(positionKind);
		dynamic_cast<GraphInterface*>(engine)->setPosition(position);
		graph->bindEngine(id, engine);
	}
	if(id == AudioInterface::interfaceId && audio != NULL)
	{
//		dynamic_cast<AudioInterface*>(engine)->setPositionKind(positionKind);
//		dynamic_cast<AudioInterface*>(engine)->setPosition(position);
		audio->bindEngine(id, engine);
	}
}

bool Combiner::updateInformation(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId && graph != NULL)
	{
		dynamic_cast<GraphInterface*>(engine)->setPosition(position);
		return graph->updateInformation(id, engine);
	}
	// test audio update
	if (id == AudioInterface::interfaceId && audio != NULL)
	{
		dynamic_cast<AudioInterface*>(engine)->setPosition(position);
		return audio->updateInformation(id, engine);
	}
	return false;
}

void Combiner::process(ProcessInfo &info)
{
	matrix34 rot = matrix34::CreateRotationMatrix(0.01f, v3(1.0f, 1.0f, 1.0f));

	//if (position.getTranslation().x > 10)
	//	position.setTranslation(v3(position.getTranslation().x - 0.01f, 0.0f, 0.0f));
	
	//if (position.getTranslation().x < 0)
	//	position.setTranslation(v3(position.getTranslation().x + 0.01f, 0.0f, 0.0f));

//	position.setTranslation(v3(position.getTranslation().x - 3*sin(position.getTranslation().x), 0.0f, 0.0f));	// - "double" effect !!!

//	position.setTranslation(v3(- 0.1f * sin(info.curTime), 0.0f, 0.0f));
	//position.setTranslation(v3(position.getTranslation().x - 0.1f * sin(info.curTime), position.getTranslation().y - 0.1f * sin(info.curTime/4), 0.0f));

//	position = position * rot;
}
