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
protected:
	uid id;
public:
	// дефолтовый конструктор присваюивает уникальный идентификатор.
	GameObject() { id = objectIdGenerator.genUid(); }
	virtual ~GameObject() {}

	// Object unique identifier
	virtual uid	 getId()				{ return id; }

	virtual bool isSuportingInterface(InterfaceId) { return false; }
	virtual bool beforeInject(InterfaceId) { return true; }
	virtual void afterRemove(InterfaceId, Engine*) {}
	
	virtual bool updateInformation(InterfaceId, Engine*) { return false; }
	virtual void bindEngine(InterfaceId, Engine*) { }
	virtual void process(IN const ProcessInfo&) {}

	virtual bool InitFromConfig(Config* conf) {return true;}
};

#endif
