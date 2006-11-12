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
	Input(void):
		game(NULL),
		conf(NULL),
		CaptureMouse(NULL),
		FreeMouse(NULL),
		Process(NULL)
		{}

	virtual ~Input(void) {}
	virtual bool init(Config* _conf);
	virtual void setGame(Game *_game) { game = _game; }

	virtual bool isPressed(std::string key);
	virtual bool isAlive(void) { return alive; }
	virtual void getMouseDelta(double &dx, double &dy);
	virtual void setMouseCenter(int _cx, int _cy) { cx = _cx; cy = _cy; }
	virtual bool isMouseCaptured(void) { return mouseCaptured; }

	void (Input::*CaptureMouse)(void);
	void (Input::*FreeMouse)(void);
	void (Input::*Process)(void);

#if STEEL_OS == OS_WIN32
	void UseWinAPI(void);

	void CaptureMouse_WinAPI(void);
	void FreeMouse_WinAPI(void);
	void Process_WinAPI(void);
	LRESULT CALLBACK ProcessMessage_WinAPI(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::string DecodeKey_WinAPI(MSG p);
#endif

#ifdef LIB_SDL
	void UseSDL(void);

	void CaptureMouse_SDL(void);
	void FreeMouse_SDL(void);
	void Process_SDL(void);
#endif

};

#endif // __INPUT_H__
