/*id*********************************************************
	File: input/game_input.h
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Интерфейс game-input
 ************************************************************/

#ifndef GAME_H
#define GAME_H

#include "../steel.h"

#include <string>

/*
Класс, задающий правила игры
*/

class GameInput
{
public:
	// виртуальные обработчики клавиатуры и мышки
	virtual void handleEventKeyDown(const std::string& key) abstract;
	virtual void handleEventKeyUp(const std::string& key) abstract;
	virtual void handleMouse(double dx, double dy) abstract;
};


#endif
