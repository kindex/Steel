/*id*********************************************************
	File: objects/combiner/combiner.h
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
	bool isSuportingInterface(IN OUT Engine&);
	bool beforeInject(IN OUT Engine&){ return true; }
	void afterRemove(IN OUT Engine&){}
	bool updateInformation(IN OUT Engine&);
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
	GameObject* getObject(size_t index) { return objects[index]; }

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
