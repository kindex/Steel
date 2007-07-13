/*id*********************************************************
	File: engine/engine.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Bond [Andrey Bondarenko]
	License:
		Steel Engine License
	Description:
		Класс Object - прототип класса, от которого должны наследоваться 
		классы, которые должны обрабатываться классом Engine.
		Класс Engine (движок) через функцию inject() получает объекты, которые
		должны быть обработаны, но обрабатывает их только внутри process().
		Для этого объекты должны поддерживать интерфейс (interface) обмена 
		информацией с движком. От этого интерфейса наследуется Engine.
 ************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include "../steel.h"
#include "../res/res.h"
#include "../res/config/config.h"
#include "../math/vector3d.h"
#include "interface.h"

#include <string>

class Engine;
class EngineHelper;
class GameObject;

struct Shadow
{
	GameObject* object;
	GameObject* parent;
	Engine* engine;
//		int storageType; //  ??
	uid objectId; // инентификатор объекта (uid)
	int shadowIndex; // индекс этой структуры (кеша) в массиве shadow

	// время последнего изменения объекта. Если отлично от того, что возвращает Object::getModificationTime(), то надо обновить кеш.
//		ModificationTime modificationTime, childrenModificationTime;
	// список детей объекта (uid)
	pvector<uid> children;

	Shadow(Engine* aEngine): object(NULL), parent(NULL), engine(aEngine) {}
	virtual void fill(GameObject* object);
	// овновляюет место для хранения дополнительной инормации (shadow, кеш объекта) - для одного объекта
	// возвращает true, если была обнавлена вся информация
	virtual ~Shadow() {}
	virtual void setParent(GameObject* aparent) {parent = aparent; }
};

class Engine : public BaseInterface
{
public:
	Config* conf;

	virtual ~Engine() {}
//	virtual bool init(std::string _conf) = 0; // initialization

	virtual bool clear() abstract; 
//	protottype: virtual bool inject(Object *object) { return false; }; // add object to process by engine
//	virtual	bool remove(Object *object);
//	virtual bool process() = 0; // add object to process by engine
	virtual void bindHelper(EngineHelper *_helper) { helper = _helper; } 
	// TODO: убрать это. Не вписывает в идею движка

protected:
	EngineHelper* helper;

	/*
		для увеличения производительности некоторая чать информации об объектах
		хранится в Shadow. В унаследованных от Engine классах
		класс Shadow дополняется
	*/
	typedef pvector<Shadow*> ShadowPVector;
	// кеш объектов
	ShadowPVector shadows;
	
    typedef std::map<uid, int> UIdMap;
    typedef std::map<InterfaceId, UIdMap> ShadowHash;

	typedef int sid; // Shadow vector index
	// отображение идентификаторов объекта на положение в массиве shadow
	ShadowHash idHash;
	virtual int findSid(uid sid, InterfaceId iid)
	{
        UIdMap& uidmap = idHash[iid];
		UIdMap::iterator it = uidmap.find(sid);
		if (it == uidmap.end())
        {
			return -1;
        }
		else
        {
			return it->second;
        }
	}

public:
	virtual Shadow* getShadow(GameObject* object, const InterfaceId id);
	virtual Shadow* getShadow(uid id, const InterfaceId iid);

protected:
	// создаёт место для хранения дополнительной инормации (shadow, кеш объекта) - для одного объекта
    virtual bool makeShadowForObject(GameObject* object, const InterfaceId);
	virtual Shadow* shadowClassFactory(GameObject* object, const InterfaceId) abstract;
	virtual void makeShadowForObjectPost(GameObject* object, Shadow* shadow) {}
    virtual int addShadow(Shadow*, const InterfaceId);

	virtual void deleteShadowForObject(int sid, const InterfaceId id);
	virtual void deleteShadowForObjectPost(int sid) {}

	// создаёт место для хранения дополнительной инормации (shadow, кеш объекта) - для детей объекта
	virtual void deleteShadowForChildren(int sid, const InterfaceId id);
};

#endif
