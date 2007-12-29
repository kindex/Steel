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

#ifndef __INPUT_WIN_H__
#define __INPUT_WIN_H__

/* Посредник между классом Game и драйверами, которые опрашивают
устройства ввода. Хранит события, нажатые кнопки
*/
#include "../steel.h"

#if STEEL_OS == OS_WIN32
#define NOMINMAX
#include <windows.h>
#include "input.h"

class Config;

class InputWin : public Input
{
public:
    void start();
	void CaptureMouse_WinAPI();
	void FreeMouse_WinAPI();
	void Process_WinAPI();

	LRESULT CALLBACK ProcessMessage_WinAPI(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::string DecodeKey_WinAPI(MSG p);
};
#endif // OS_WIN32
#endif // __INPUT_WIN_H__
