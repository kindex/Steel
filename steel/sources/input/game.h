/*id*********************************************************
	File: game/game.h
	Unit: game
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
 ************************************************************/

#ifndef GAME_H
#define GAME_H

#include "../steel.h"
#include "../res/res.h"

#include "../graph/graph_engine.h"
#include "../audio/audio_engine.h"
#include "../objects/light.h"
#include "../objects/game_object_factory.h"

#include <string>
#include <map>

/*
Класс, задающий правила игры
*/

class GameInput
{
public:
	// виртуальные обработчики клавиатуры и мышки
	virtual void handleEventKeyDown(std::string key) = 0;
	virtual void handleEventKeyUp(std::string key) = 0;
	virtual void handleMouse(double dx, double dy) = 0;
};


#endif
