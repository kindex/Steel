#include "../_cpp.h"
#ifdef OPENGL_WIN
#include "input_win.h"

#include <windows.h>		// Header File For Windows
#include <winbase.h>
typedef POINT Point;									// This is a window structure that holds an X and Y

using namespace std;

void InputWIN::captureMouse(int _cx, int _cy)
{
	mouseCaptured = true;

	cx = _cx;
	cy = _cy;
	lastdx = 0;
	lastdy = 0;

	SetCursorPos(cx, cy);
}

string decodeKey(WPARAM p)
{
	string key;
	switch(p)
	{
		case VK_ESCAPE: key = "escape"; break;
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
			if(key != "")
				game->handleEventKeyDown(key);
			break;

		case WM_KEYUP:
			key = decodeKey(msg.wParam);
			if(key != "")
				game->handleEventKeyUp(key);

			break;
		}
		TranslateMessage(&msg);						// Find out what the message does
		DispatchMessage(&msg);						// Execute the message
	}

	Point n;
	GetCursorPos(&n);
	SetCursorPos(cx, cy);
	
	lastdx += cx - n.x;
	lastdy += cy - n.y;
	if(lastdx != 0)
	{
		lastdx = lastdx +1;
	}
}

void InputWIN::getMouseDelta(int &dx, int &dy)
{
	dx = lastdx; lastdx = 0;
	dy = lastdy; lastdy = 0;
}

#endif

