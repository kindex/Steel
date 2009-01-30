/*id*********************************************************
	File: demo_game_object_factory.h
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Позволяет движку использовать классы, объявленные в Demo проекте.
 ************************************************************/
#include <steel.h>
#include "game_factory.h"
#include "game_free_scene.h"
#include "19_ps/19_game.h"
#include "23_ageia_tech/23_ageia_tech.h"


Game* GameFactory::createGame(IN const std::string& className) const
{
	if (className == "demo")	    return new GameDemo;
	if (className == "ps")			return new GamePS;
	if (className == "ageia")		return new GameAgeiatech;

	return NULL;
}

