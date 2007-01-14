/*id*********************************************************
	File: game_factory.h
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

#ifndef DEMO__GAME__FACTORY_H__
#define DEMO__GAME__FACTORY_H__

#include <steel.h>
#include "game.h"

class GameFactory
{
public:
	Game* createGame(IN const std::string& className) const;
};


#endif
