/*id*********************************************************
	File: engine/interface.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Базовый интерфейс, который должны поддрерживать все движки
 ************************************************************/

#ifndef ENGINE__INTERFACE_H
#define ENGINE__INTERFACE_H

#include "../steel.h"

typedef int InterfaceId;

class GameObject;

class BaseInterface
{
public:
	virtual bool setCurrentObject(GameObject*) = 0;
};

#endif
