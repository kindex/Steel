/*id*********************************************************
	File: input/input.h
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Абстрактный класс обработчика ввода с клавиатуры и мыши
 ************************************************************/

#ifndef __INPUT_H__
#define __INPUT_H__

/* Посредник между классом Game и драйверами, которые опрашивают
устройства ввода. Хранит события, нажатые кнопки
*/
#include "../steel.h"

#include "game_input.h"

#include <string>
#include <map>

class Config;

class Input
{
protected:
	std::map<std::string, bool> keyPressed;
	double sensetivity;
	bool alive, mouseCaptured, focused;
	GameInput*	game;
	Config*		conf;

	int cx, cy, lastdx, lastdy;

public:
	Input():
		game(NULL),
		conf(NULL)
		{}

	bool init(Config* _conf);
	void setGame(GameInput* _game) { game = _game; }

	bool isPressed(std::string key);
	bool isAlive() { return alive; }
	void getMouseDelta(double &dx, double &dy);
	void setMouseCenter(int _cx, int _cy) { cx = _cx; cy = _cy; }
	bool isMouseCaptured() { return mouseCaptured; }

#ifdef LIB_SDL
	void UseSDL();

	void CaptureMouse_SDL();
	void FreeMouse_SDL();
	void Process_SDL();
#endif

};

#endif // __INPUT_H__
