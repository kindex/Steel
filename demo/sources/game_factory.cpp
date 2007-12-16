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
#include <steel.h>
#include "game_factory.h"
#include "game_free_scene.h"
#include "19_ps/19_game.h"
#include "20_labyrinth/20_labyrinth.h"

Game* GameFactory::createGame(IN const std::string& className) const
{
	if (className == "free_scene")	return new GameFreeScene;
	if (className == "ps")			return new GamePS;
	if (className == "labyrinth")	return new GameLabyrinth;

	return NULL;
}

