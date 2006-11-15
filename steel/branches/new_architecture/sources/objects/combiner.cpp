/*id*********************************************************
	File: objects/combiner.cpp
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
		одного физического объекта и одного графического.
 ************************************************************/

#include "../steel.h"
#include "combiner.h"

bool GraphObject::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	ConfigArray *vertexesConfing = conf->getArray("vertexes");
	ConfigArray *trianglesConfig = conf->getArray("triangles");
	ConfigArray *mapCoordsConfing = conf->getArray("mapCoords");


	return true;
}

bool GraphObject::updateInformation(InterfaceId id, Engine* engine)
{
	return true;
}

bool Combiner::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;

	position.loadIdentity();
	position.setTranslation(conf->getv3("origin"));
	positionKind = conf->gets("position_kind", "global") == "local"?POSITION_LOCAL:POSITION_GLOBAL;

	graph = new GraphObject;

	bool ok =  graph->InitFromConfig(conf->find("graph"));
	if(!ok)
	{
		delete graph; 
		graph = NULL; 
		return false;
	}

	return true;
}

