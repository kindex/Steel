/*id*********************************************************
	File: engine/interface.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Базовый интерфейс, который должны поддрерживать все движки
 ************************************************************/

#ifndef ENGINE__INTERFACE_H
#define ENGINE__INTERFACE_H

#include "../steel.h"
#include "../math/vector3d.h"
#include "../math/matrix34.h"

typedef int InterfaceId;

class GameObject;

class BaseInterface
{
public:
	virtual bool setCurrentObject(GameObject*) = 0;
};

/*
местоположение + поворот объекта
*/
typedef matrix34 ObjectPosition;

// Тип положения объекта: local – относительно родитиля, global – в глобальной системе координат.
typedef enum
{
	POSITION_NONE,
	POSITION_LOCAL,
	POSITION_GLOBAL,
	POSITION_SCREEN
} PositionKind;



class Interface3D
{
public:
	virtual void setPosition(ObjectPosition) = 0;
	virtual void setPositionKind(PositionKind) = 0;
};

class ChildrenInterface
{
public:
	virtual void addChild(GameObject* child) = 0;
	virtual void deleteChild(GameObject* child) = 0;
	virtual void clearChildren() = 0;
};


#endif
