/*id*********************************************************
	File: objects/game_object_factory.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Создаватель игровый объектов при загрузке из конфигов
 ************************************************************/

#ifndef __OBJECTS__GAME_OBJECT_FACTORY_H
#define __OBJECTS__GAME_OBJECT_FACTORY_H

#include "../steel.h"
#include "../engine/game_object.h"

class GameObjectFactory
{
public:
	virtual GameObject* createGameObject(IN const std::string& className) const;
};

extern GameObjectFactory* gameObjectFactory;

GameObject *createGameObject(Config *conf);


#endif
