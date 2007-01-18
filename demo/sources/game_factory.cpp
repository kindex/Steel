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
#include "game_factory.h"
#include "game_free_scene.h"
#include "19_ps/19_game.h"

Game* GameFactory::createGame(IN const std::string& className) const
{
	if(className == "free_scene")	return new GameFreeScene;
	if(className == "19_ps")		return new Game19;

	return NULL;
}

