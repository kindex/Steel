/*id*********************************************************
	File: engine.h
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Bond [Andrey Bondarenko]
	License:
		Steel Engine License
	Description:
		Класс Interface - прототип класса, от которого должны наследоваться 
		классы, которые должны обрабатываться классом Engine.
		Класс Engine (движок) через функцию inject() получает объекты, которые
		должны быть обработаны, но обрабатывает их только внутри process().
		Для этого объекты должны поддерживать интервейс (interface) обмена 
		информацией с движком.
 ************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include "steel.h"
#include "res/res.h"
#include "res/conf/conf.h"
#include "math/vector3d.h"
#include "interface.h"
#include "helper.h"

#include <string>

class Engine
{
public:
	Config			*conf;

	virtual ~Engine(void) {}
	virtual bool init(std::string _conf) = 0; // initialization

	virtual bool clear(void) = 0; 
//	protottype: virtual bool inject(Interface *object) { return false; }; // add object to process by engine
//	virtual	bool remove(Interface *object);
//	virtual bool process() = 0; // add object to process by engine
	virtual void bindHelper(EngineHelper *_helper) { helper = _helper; } 
	// TODO: убрать это. Не вписывает в идею движка

protected:
	EngineHelper	*helper;

	/*
		для увеличения произвтдительности некоторая чать информации об объектах
		хранится в Storage. В унаследованных от Engine классах
		класс Storage дополняется
	*/
	struct Storage
	{
		Interface *object;
//		int storageType; //  ??
		uid objectId; // инентификатор объекта (uid)
		int storageIndex; // индекс этой структуры (кеша) в массиве storage

		// время последнего изменения объекта. Если отлично от того, что возвращает Object::getModificationTime(), то надо обновить кеш.
		ModificationTime modificationTime, childrenModificationTime;
		// список детей объекта (uid)
		svector<uid> children;

		Storage(void): object(NULL) {}
		virtual void fill(Interface *object);
		// овновляюет место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
		virtual void cache(void);
		virtual ~Storage(void) {}
	};
	// кеш объектов
	svector<Storage*> storages;
	typedef std::map <uid, int> StorageHash;
	// отображение идентификаторов объекта на положение в массиве storage
	StorageHash idHash;

	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	virtual void makeStorageForObject(Interface *object);
	virtual Storage* getStorageClass(Interface *object) = 0;
	virtual void makeStorageForObjectPost(Interface *object, Storage *storage) {}

	virtual void deleteStorageForObject(int sid);
	virtual void deleteStorageForObjectPost(int sid) {}

	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для детей объекта
	virtual void makeStorageForChildren(Interface *object)  = 0;
	virtual void deleteStorageForChildren(int sid);
};

#endif
