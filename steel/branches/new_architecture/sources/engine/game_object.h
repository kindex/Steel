/*id*********************************************************
	File: engine/object.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Предок для интерфейтов объектов, которые имеют положение
		(глобальную, локальную систему координат)
************************************************************/
#ifndef INTERFACE_H
#define INTERFACE_H

#include "../steel.h"

#include "interface.h"



/*
	GameObject protottype
	От этого класса наследуются все игровые объекты.
*/

class GameObject
{
protected:
	uid id;
public:
	// дефолтовый конструктор присваюивает уникальный идентификатор.
	GameObject(void) { id = objectIdGenerator.genUid(); }

	// Object unique identifier
	virtual uid		getId(void)				{ return id; }

	virtual bool isSuportingInterface(InterfaceId) { return false; }
	virtual bool beforeInject(InterfaceId) { return true; }
	virtual void afterRemove(InterfaceId) {}
	
	virtual Callback getCallback(InterfaceId) { return NULL; }
};

#endif
