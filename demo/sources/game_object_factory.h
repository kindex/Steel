/*id*********************************************************
	File: demo_game_object_factory.h
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Позволяет движку использовать классы, объявленные в Demo проекте.
 ************************************************************/

#ifndef DEMO__GAME_OBJECT_FACTORY_H__
#define DEMO__GAME_OBJECT_FACTORY_H__

#include <steel.h>
#include <engine/game_object.h>
#include <objects/game_object_factory.h>

class DemoGameObjectFactory : public GameObjectFactory
{
public:
	GameObject* createGameObject(IN const std::string& className) const;
};


#endif
