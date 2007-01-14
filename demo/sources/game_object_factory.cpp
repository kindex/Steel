/*id*********************************************************
	File: demo_game_object_factory.h
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Позволяет движку использовать классы, объявленные в Demo проекте.
 ************************************************************/
#include <steel.h>
#include "game_object_factory.h"
#include "objects/sphere.h"
#include "18_weapon/kar98k.h"

GameObject* DemoGameObjectFactory::createGameObject(IN const std::string& className) const
{
	if(className == "sphere")	return new Sphere;
	if(className == "kar98k")	return new Kar98k;

	return GameObjectFactory::createGameObject(className);
}

