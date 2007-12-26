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
class GraphObject;
class AudioObject;
class Transformation;

class Combiner: public GameObject
{
public:
	Combiner();
	bool supportsInterface(IN OUT Engine&, IN const InterfaceId);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
	void bindEngine(IN OUT Engine&, IN const InterfaceId);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
	GameObject* getObject(size_t index) { return objects[index]; }
    void traverse(Visitor&, const ObjectPosition& base_position);
	void addObject(GameObject*);

protected:
    const ObjectPosition& getLocalPosition();

	GraphObject*    graph;
	AudioObject*    audio;
	Transformation* transformation;
	ObjectPosition  origin;
	ObjectPosition  position;
	PositionKind    positionKind;
    bool            polyhedraPhysicSameAsGraph;

	pvector<GameObject*> objects;
};

GraphObject* loadGraphObject(Config& conf, const std::string& configVariable);
AudioObject* loadAudioObject(Config& conf, const std::string& configVariable);
Transformation* loadTransformation(Config& conf, const std::string& configVariable);

#endif
