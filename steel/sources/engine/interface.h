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

const InterfaceId INTERFACE_BASE = 1 << 0;
const InterfaceId INTERFACE_3D = 1 << 1;
const InterfaceId INTERFACE_CHILDREN = 1 << 2;
const InterfaceId INTERFACE_GRAPH_RAW = 1 << 3; // TODO: rename
const InterfaceId INTERFACE_AUDIO_RAW = 1 << 4; // TODO: rename

const InterfaceId INTERFACE_GRAPH = INTERFACE_BASE + INTERFACE_3D + INTERFACE_CHILDREN + INTERFACE_GRAPH_RAW;
const InterfaceId INTERFACE_AUDIO = INTERFACE_BASE + INTERFACE_3D + INTERFACE_CHILDREN + INTERFACE_AUDIO_RAW;

class GameObject;

class BaseInterface
{
public:
	virtual bool setCurrentObject(GameObject*) abstract;
	virtual bool isSupportingInterface(IN const InterfaceId) abstract;
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
	virtual void setPosition(ObjectPosition) abstract;
	virtual void setPositionKind(PositionKind) abstract;
};

class ChildrenInterface
{
public:
	virtual void addChild(GameObject* child) abstract;
	virtual void deleteChild(GameObject* child) abstract;
	virtual void clearChildren() abstract;
};


#endif
