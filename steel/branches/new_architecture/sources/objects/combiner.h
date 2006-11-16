/*id*********************************************************
	File: objects/combiner.h
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

#ifndef __OBJECTS__COMBINER_H
#define __OBJECTS__COMBINER_H

#include "../steel.h"
#include "../engine/game_object.h"
#include "../graph/graph_interface.h"

class GraphObject: public GameObject
{
public:
	GraphObject(void);

	bool isSuportingInterface(InterfaceId id) { return id == GraphInterface::interfaceId; }
	void bindEngine(InterfaceId, Engine*);
	bool InitFromConfig(Config *conf);
protected:
	Vertexes *vertexes;
	Normals  *normals;
	FaceMaterials *faces;
	TexCoords *texCoords;
};

class Combiner: public GameObject
{
protected:
	GraphObject *graph;
	ObjectPosition position;
	PositionKind positionKind;
public:
	Combiner(void): graph(NULL) {}

	bool isSuportingInterface(InterfaceId id) 
	{ 
		return id == GraphInterface::interfaceId; 
	}
	
	void bindEngine(InterfaceId id, Engine* engine)
	{
		if(id == GraphInterface::interfaceId && graph != NULL)
		{
			dynamic_cast<GraphInterface*>(engine)->setPositionKind(positionKind);
			graph->bindEngine(id, engine);
		}
	}

	bool updateInformation(InterfaceId id, Engine* engine)
	{
		if(id == GraphInterface::interfaceId && graph != NULL)
		{
			dynamic_cast<GraphInterface*>(engine)->setCurrentObject(this);
			dynamic_cast<GraphInterface*>(engine)->setPosition(position);
			return graph->updateInformation(id, engine);
		}
		return false;
	}
	bool InitFromConfig(Config *conf);
};

#endif
