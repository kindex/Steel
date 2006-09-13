/*id*********************************************************
	File: physic/physic_engine.h
	Unit: physic engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Абстрактный физический движок
		Обрабатывает движение объектов.
		Поддерживает ирерархию объектов, кеширует информацию об	объектах.
************************************************************/

#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H

#include "../steel.h"
#include <map>
#include "../res/conf/conf.h"
#include "physic_interface.h"

class PhysicEngine: public Engine
{
protected:
	// спиок [глобальных] объектов, котоыре были лобавлены в движок для обработки с помощью процедуры inject()
	steel::vector<PhysicObject*> objects;
	// действующая гратация на все обхекты
	v3 g;

public:
	struct TotalInfo
	{
		int collisionCount;
	} total;

public:
	virtual bool init(std::string _conf);
	virtual void deinit(void) { clear();}

	// удаляет все объекты из движка (foreach remove)
	virtual bool clear(void);

	// обрабатывает движение всех объектов
	virtual bool process(steel::time globalTime, steel::time time) = 0; 

	// Inject добавляет объект для обработки
	// Типы движений объектов: uni, custom, none
	// У uni не может быть детей
	// У custom и none не может быть детей с типом uni
	// добавляет объект и его детей в движок для обработки
	virtual bool inject(PhysicObject *object);
	virtual bool remove(PhysicObject *object);

	virtual void setGravitation(const v3 force) { g = force; }
};

#endif

