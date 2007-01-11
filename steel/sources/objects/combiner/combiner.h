/*id*********************************************************
	File: objects/combiner/combiner.h
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

#ifndef __OBJECTS__COMBINER__COMBINER_H
#define __OBJECTS__COMBINER__COMBINER_H

#include "../../steel.h"
#include "../../engine/game_object.h"
#include "../../graph/graph_interface.h"
#include "graph_object.h"
#include "audio_object.h"
#include "../transformation/transformation.h"

class Combiner: public GameObject
{
public:
	Combiner();
	bool isSuportingInterface(InterfaceId id);
	void bindEngine(InterfaceId id, Engine* engine);
	bool updateInformation(InterfaceId id, Engine* engine);
	bool InitFromConfig(Config *conf);
	void process(IN const ProcessInfo& info);

protected:
	GraphObject*    graph;
	AudioObject*    audio;
	Transformation* transformation;
	ObjectPosition  origin;
	ObjectPosition  position;
	PositionKind    positionKind;

	pvector<GameObject*> objects;
};

#endif
