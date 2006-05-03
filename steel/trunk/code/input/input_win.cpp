#include "../_cpp.h"
#ifdef OPENGL_WIN
#include "input_win.h"

#include <windows.h>		// Header File For Windows
#include <winbase.h>
typedef POINT Point;									// This is a window structure that holds an X and Y

using namespace std;

void InputWIN::captureMouse()
{
	mouseCaptured = true;

	lastdx = 0;
	lastdy = 0;

	SetCursorPos(cx, cy);
	ShowCursor(false);										// Hide Mouse Pointer
}

void InputWIN::freeMouse()
{
	ShowCursor(true);			// show Mouse Pointer
	mouseCaptured = false;
}

string decodeKey(WPARAM p)
{
	string key;
	switch(p)
	{
		case VK_RETURN: key = "return"; break;
		case VK_ESCAPE: key = "escape"; break;
		case VK_SPACE: key = "space"; break;
		default:
			if(p>='A' && p<='Z')
			{
				key.resize(1);
				key[0] = p + ('a'-'A');
			}
			break;
	}
	return key;
}


LRESULT CALLBACK InputWIN::processMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void InputWIN::process()
{
	MSG msg;
	BOOL wasmsg;
	string key;
	if (wasmsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		switch(msg.message)					// If the message wasnt to quit
		{
		case WM_QUIT:
			alive = false;
			break;
		case WM_KEYDOWN:
			key = decodeKey(msg.wParam);
			if(key != "" && !keyPressed[key])
			{
				game->handleEventKeyDown(key);
				if(key == "space")
				{
					if(mouseCaptured) 
						freeMouse();
					else
						captureMouse();
				}
				keyPressed[key] = true;
			}

			break;

		case WM_KEYUP:
			key = decodeKey(msg.wParam);
			if(key != "")
				game->handleEventKeyUp(key);
			keyPressed[key] = false;
			break;

		case WM_SETFOCUS:
			focused = true;
			break;
		}
		TranslateMessage(&msg);						// Find out what the message does
		DispatchMessage(&msg);						// Execute the message
	}
	if(mouseCaptured && focused)
	{
		Point n;
		GetCursorPos(&n);
		SetCursorPos(cx, cy);
	
		lastdx += cx - n.x;
		lastdy += cy - n.y;
	}
}


#endif

