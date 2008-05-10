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
#include "../math/sprite.h"

enum InterfaceId
{
    INTERFACE_BASE = 1 << 0,
    INTERFACE_3D = 1 << 1,
    INTERFACE_CHILDREN = 1 << 2,
    INTERFACE_GRAPH_RAW = 1 << 3, // TODO: rename
    INTERFACE_AUDIO_RAW = 1 << 4, // TODO: rename

    INTERFACE_GRAPH = INTERFACE_BASE + INTERFACE_3D + INTERFACE_CHILDREN + INTERFACE_GRAPH_RAW,
    INTERFACE_AUDIO = INTERFACE_BASE + INTERFACE_3D + INTERFACE_CHILDREN + INTERFACE_AUDIO_RAW,
};

class GameObject;

class BaseInterface
{
public:
	virtual bool setCurrentObject(GameObject*, IN const InterfaceId) abstract;
};

/*
местоположение + поворот объекта
*/
typedef matrix34 ObjectPosition;

class Interface3D
{
public:
	virtual void setPosition(const ObjectPosition&) abstract;
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
