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

#ifndef _GAME_INPUT_H_
#define _GAME_INPUT_H_

#include "../steel.h"
#include <string>

class GameInput
{
public:
    virtual ~GameInput() {}
	// виртуальные обработчики клавиатуры и мышки
	virtual void handleEventKeyDown(const std::string& key) abstract;
	virtual void handleEventKeyUp(const std::string& key) abstract;
	virtual void handleMouse(double dx, double dy) abstract;
};


#endif
