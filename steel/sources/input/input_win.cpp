/*id*********************************************************
	File: input/input_win.cpp
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Обработчика ввода с клавиатуры и мыши, используя Windows API
 ************************************************************/

#include "../steel.h"

#if STEEL_OS == OS_WIN32

#include "input_win.h"
#include "../common/utils.h"
#include "../res/config/config.h"

#include <windows.h>		// Header File For Windows
#include <winbase.h>

using namespace std;

typedef POINT Point;									// This is a window structure that holds an X and Y

void InputWin::CaptureMouse_WinAPI()
{
	mouseCaptured = true;

	lastdx = 0;
	lastdy = 0;

	SetCursorPos(cx, cy);
	ShowCursor(false);										// Hide Mouse Pointer
}

string InputWin::DecodeKey_WinAPI(MSG p)
{
	string key;
	switch(p.message)
	{
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN: key = "mouse1"; break;
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN: key = "mouse1"; break;
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN: key = "mouse1"; break;

		case WM_KEYUP:
		case WM_KEYDOWN:
			switch(p.wParam)
			{
				case VK_RETURN: key = "return"; break;
				case VK_ESCAPE: key = "escape"; break;
				case VK_SPACE: key = "space"; break;
				case VK_PAUSE: key = "pause"; break;
				case VK_SNAPSHOT: key = "snapshot"; break;
				case VK_OEM_1: key = ";"; break;
				case VK_OEM_2: key = "/"; break;
				case VK_OEM_7: key = "'"; break;

		//		case : key = ""; break;

				default:
					WPARAM c = p.wParam;
					if(c>='0' && c<='9')
					{
						key.resize(1);
						key[0] = c;
					}
                    else if(c>='A' && c<='Z')
					{
						key.resize(1);
						key[0] = c + ('a'-'A');
					}
					else if(c>=VK_F1 && c<=VK_F24)
                    {
						key = "f" + IntToStr(c-VK_F1 + 1); break;
                    }
                    else if (c == VK_OEM_MINUS)
                    {
                        key = "-";
                    }
                    else if (c == VK_OEM_PLUS)
                    {
                        key = "+";
                    }

					break;
			}
	}
	return key;
}


LRESULT CALLBACK InputWin::ProcessMessage_WinAPI(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		focused = false;
		break;
	case WM_SETFOCUS:
		focused = true;
		if(mouseCaptured)
		{
			lastdx = 0;
			lastdy = 0;
			SetCursorPos(cx, cy);
		}
		break;
	
	default:
		return DefWindowProc (hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void InputWin::Process_WinAPI()
{
	MSG msg;
	BOOL wasmsg;
	string key;
	while (wasmsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		switch (msg.message)					// If the message wasnt to quit
		{
		case WM_QUIT:
			alive = false;
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_KEYDOWN:
			key = DecodeKey_WinAPI(msg);

			if (key != "" && !keyPressed[key])
			{
				game->handleEventKeyDown(key);
				if (key == "space")
				{
					if (mouseCaptured)
                    {
						FreeMouse_WinAPI();
                    }
					else
                    {
						CaptureMouse_WinAPI();
                    }
				}
				keyPressed[key] = true;
			}
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_KEYUP:
			key = DecodeKey_WinAPI(msg);
			if(key != "")
            {
				game->handleEventKeyUp(key);
            }
			keyPressed[key] = false;
			break;

		case WM_SETFOCUS:
			focused = true;
			break;
		}
		TranslateMessage(&msg);						// Find out what the message does
		DispatchMessage(&msg);						// Execute the message
	}
	if (mouseCaptured && focused)
	{
		Point n;
		GetCursorPos(&n);
		SetCursorPos(cx, cy);
	
		lastdx += cx - n.x;
		lastdy += cy - n.y;
	}
}

void InputWin::FreeMouse_WinAPI()
{
	ShowCursor(true);			// show Mouse Pointer
	mouseCaptured = false;
}

void InputWin::start()
{
	if (conf->geti("mouse.capture", 0))
    {
		CaptureMouse_WinAPI();
    }
}

#endif
