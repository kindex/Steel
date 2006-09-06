/*id*********************************************************
	File: input/input.h
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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

#include "../res/conf/conf.h"

class Input;  // forward declaration, cross-use
#include "../game/game.h"

#include <string>
#include <map>


class Input
{
protected:
	std::map<std::string, bool> keyPressed;
	double sensetivity;
	bool alive, mouseCaptured, focused;
	Game	*game;
	Config	*conf;

	int cx, cy, lastdx, lastdy;

public:
	virtual ~Input(void) {}
	bool init(std::string _conf);
	void setGame(Game *_game) { game = _game; }

	bool isPressed(std::string key);
	bool isAlive(void) { return alive; }
	virtual void captureMouse(void) = 0;
	virtual void freeMouse(void) = 0;
	virtual void process(void) = 0;
	virtual void getMouseDelta(double &dx, double &dy);
	virtual void setMouseCenter(int _cx, int _cy) { cx = _cx; cy = _cy; }
	bool isMouseCaptured(void) { return mouseCaptured; }
};

#endif // __INPUT_H__
