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
        object_id = objectIdGenerator.genUid();
    }
	virtual ~GameObject() {}

	// Object unique identifier
	virtual uid	 getObjectId()				
    {
        return object_id; 
    }

    // true if object or its children han be processed by engine
	virtual bool supportsInterface(IN OUT Engine&, IN const InterfaceId) abstract;
    // true if object wants to be added to engine
    virtual bool beforeInject(IN OUT Engine&, IN const InterfaceId) 
    { 
        return true;
    }
    virtual void afterRemove(IN OUT Engine&, IN const InterfaceId) 
    {
    }
	
	virtual bool updateInformation(IN OUT Engine&, IN const InterfaceId) abstract;
    // here object can add children to engine
	virtual void bindEngine(IN OUT Engine&, IN const InterfaceId)
    {
    }
	virtual void process(const ProcessInfo&) abstract;

	virtual bool InitFromConfig(Config&) abstract;

    virtual void traverse(Visitor& visitor, const ObjectPosition& base_position)
    {
        if (visitor.visit(this))
        {
            visitor.postvisit(this, base_position);
        }
    }

private:
	uid object_id;
};

#endif
