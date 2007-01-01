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


#if STEEL_OS == OS_WIN32
	#include <windows.h>
#endif

class Input;  // forward declaration, cross-use
#include "../game/game.h"
#include "../res/res_main.h"

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
	Input():
		game(NULL),
		conf(NULL),
		CaptureMouse(NULL),
		FreeMouse(NULL),
		Process(NULL)
		{}

	virtual ~Input() {}
	virtual bool init(Config* _conf);
	virtual void setGame(Game *_game) { game = _game; }

	virtual bool isPressed(std::string key);
	virtual bool isAlive() { return alive; }
	virtual void getMouseDelta(double &dx, double &dy);
	virtual void setMouseCenter(int _cx, int _cy) { cx = _cx; cy = _cy; }
	virtual bool isMouseCaptured() { return mouseCaptured; }

	void (Input::*CaptureMouse)();
	void (Input::*FreeMouse)();
	void (Input::*Process)();

#if STEEL_OS == OS_WIN32
	void UseWinAPI();

	void CaptureMouse_WinAPI();
	void FreeMouse_WinAPI();
	void Process_WinAPI();
	LRESULT CALLBACK ProcessMessage_WinAPI(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::string DecodeKey_WinAPI(MSG p);
#endif

#ifdef LIB_SDL
	void UseSDL();

	void CaptureMouse_SDL();
	void FreeMouse_SDL();
	void Process_SDL();
#endif

};

#endif // __INPUT_H__
