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
#include "id_generator.h"
#include "visitor.h"

class Engine;
class Config;
struct ProcessInfo;

class GameObject
{
public:
	// дефолтовый конструктор присваюивает уникальный идентификатор.
	GameObject()
    {
        id = objectIdGenerator.genUid();
    }
	virtual ~GameObject() {}

	// Object unique identifier
	virtual uid	 getId()				
    {
        return id; 
    }

	virtual bool supportsInterface(IN OUT Engine&, IN const InterfaceId) abstract;
    virtual bool beforeInject(IN OUT Engine&, IN const InterfaceId) 
    { 
        return true;
    }
    virtual void afterRemove(IN OUT Engine&, IN const InterfaceId) 
    {
    }
	
	virtual bool updateInformation(IN OUT Engine&, IN const InterfaceId) abstract;
	virtual void bindEngine(IN OUT Engine&, IN const InterfaceId)
    {
    }
	virtual void process(const ProcessInfo&) abstract;

	virtual bool InitFromConfig(Config&) abstract;

    virtual void traverse(Visitor& visitor)
    {
        visitor.postvisit(this);
    }

protected:
	uid id;
};

#endif
