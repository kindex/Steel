/*id*********************************************************
	File: engine/game_object.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		GameObject protottype
		От этого класса наследуются все игровые объекты.
 ************************************************************/
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../steel.h"

#include "interface.h"
#include "engine.h"

class GameObject
{
public:
	// дефолтовый конструктор присваюивает уникальный идентификатор.
	GameObject() { id = objectIdGenerator.genUid(); }
	virtual ~GameObject() {}

	// Object unique identifier
	virtual uid	 getId()				{ return id; }

	virtual bool isSuportingInterface(IN OUT Engine&) abstract;
	virtual bool beforeInject(IN OUT Engine&) abstract;
	virtual void afterRemove(IN OUT Engine&) abstract;
	
	virtual bool updateInformation(IN OUT Engine&) abstract;
	virtual void bindEngine(IN OUT Engine&) abstract;
	virtual void process(IN const ProcessInfo&) abstract;

	virtual bool InitFromConfig(IN Config&) abstract;

protected:
	uid id;
};

#endif
