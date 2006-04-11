/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine WIN API
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Часть графического движока OpenGL, которая реализует
		системно зависимые функции, такие как создание окна.
 ************************************************************/

#include "../../_cpp.h"

#ifdef OPENGL_WIN

#include "opengl_win_engine.h"
#include "../../input/input_win.h"
#include "../../common/utils.h"

#include <vector>

using namespace std;

extern HINSTANCE hInstance;

struct Window
{
	HWND handle;
	bool focused;
	InputWIN *input;
	OpenGL_WIN_Engine *engine;
};

vector<Window> window;

bool OpenGL_WIN_Engine::init(std::string _conf, InputWIN *_input)
{
	input = _input;
	return OpenGL_Engine::init(_conf);
}

void OpenGL_WIN_Engine::repair() // repair window on resize
{
	glViewport( 0, 0, conf->geti("window.width"), conf->geti("window.height"));
}

void OpenGL_WIN_Engine::swapBuffers()
{
    if (DC) SwapBuffers(DC);
}

void RedrawWindow(HWND hWnd)
{
    PAINTSTRUCT    ps;
    BeginPaint(hWnd, &ps);							// Init the paint struct
	EndPaint(hWnd, &ps);							// EndPaint, Clean up
}


LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0;
	for(vector<Window>::iterator it = window.begin(); it != window.end(); it++)
    if (hWnd == it->handle)
	{

		switch (uMsg)
		{
/* 	case WM_PAINT:										// If we need to repaint the scene
        RedrawWindow(hWnd);
		break;*/

//	case WM_KILLFOCUS:
		case WM_SIZE:
			it->engine->resize(LOWORD(lParam),HIWORD(lParam));
			break;

	    case WM_CLOSE:										// If the window is being closes
		    PostQuitMessage(0);								// Send a QUIT Message to the window
			break;

		case WM_KEYDOWN:
			if(wParam == VK_RETURN && !it->engine->return_pressed)
			{
				it->engine->fullScpeenToggle();
				it->engine->return_pressed = true;
			}

			lRet = it->input->processMessage(hWnd, uMsg, wParam, lParam);
			break;
		case WM_KEYUP:
			if(wParam == VK_RETURN)
				it->engine->return_pressed = false;

			lRet = it->input->processMessage(hWnd, uMsg, wParam, lParam);
			break;

		default:											// Return by default
			lRet = it->input->processMessage(hWnd, uMsg, wParam, lParam);
		    break;
		}
		return lRet;
    }

    // ne nashe okno
    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);

    return lRet;										// Return by default
}

void OpenGL_WIN_Engine::resize(int w, int h)
{
	conf->setup("window.width", IntToStr(w));
	conf->setup("window.height", IntToStr(h));
	repair();
}

void OpenGL_WIN_Engine::fullScpeenToggle()
{
	if(conf->geti("fullscreen"))
	{
		if(changeToWindow()) conf->setup("fullscreen", 0);
	}
	else
	{
		if(changeToFullScpeen()) conf->setup("fullscreen", 1);
	}


}

bool OpenGL_WIN_Engine::changeToWindow()
{
	bool ok = false;
	DEVMODE dmSettings;									// Device Mode variable

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS, &dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return false;
	}

	if(dmSettings.dmPelsWidth != conf->geti("save.width") || 
		dmSettings.dmPelsHeight	!= conf->geti("save.height") || 
		dmSettings.dmBitsPerPel != conf->geti("save.depth"))
	{
		dmSettings.dmBitsPerPel = conf->geti("save.depth");
		dmSettings.dmPelsWidth	= conf->geti("save.width");				// Selected Screen Width
		dmSettings.dmPelsHeight	= conf->geti("save.height");			// Selected Screen Height

		dmSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// This function actually changes the screen to full screen
		// CDS_FULLSCREEN Gets Rid Of Start Bar.
		// We always want to get a result from this function to check if we failed
		int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);

		// Check if we didn't recieved a good return message From the function
		if(result != DISP_CHANGE_SUCCESSFUL)
		{
		// Display the error message and quit the program
			MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
			ok = false;
	//		PostQuitMessage(0);
		}
		else ok = true;
	}
	else 
		ok = true;

	if(ok)
	{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(handle, &wp);
		// Adjust width and height to required

		wp.rcNormalPosition.left	=	conf->geti("window.left");
		wp.rcNormalPosition.top		=	conf->geti("window.top");
		wp.rcNormalPosition.right	=	conf->geti("window.left") + conf->geti("savewindow.width");
		wp.rcNormalPosition.bottom	=	conf->geti("window.top") + conf->geti("savewindow.height");
		
		SetWindowPlacement(handle, &wp);

		SetWindowPos(handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

//		SetWindowLong(handle, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	}
	return ok;

}

bool OpenGL_WIN_Engine::changeToFullScpeen()
{
	bool ok = false;
	DEVMODE dmSettings;									// Device Mode variable

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS, &dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return false;
	}

	conf->setup("save.depth", dmSettings.dmBitsPerPel);
	conf->setup("save.width", dmSettings.dmPelsWidth);
	conf->setup("save.height", dmSettings.dmPelsHeight);

	if(dmSettings.dmPelsWidth != conf->geti("screen.width") || 
		dmSettings.dmPelsHeight	!= conf->geti("screen.height") || 
		dmSettings.dmBitsPerPel != conf->geti("screen.depth"))
	{
		dmSettings.dmBitsPerPel = conf->geti("screen.depth");
		dmSettings.dmPelsWidth	= conf->geti("screen.width");				// Selected Screen Width
		dmSettings.dmPelsHeight	= conf->geti("screen.height");			// Selected Screen Height

		dmSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// This function actually changes the screen to full screen
		// CDS_FULLSCREEN Gets Rid Of Start Bar.
		// We always want to get a result from this function to check if we failed
		int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);

		// Check if we didn't recieved a good return message From the function
		if(result != DISP_CHANGE_SUCCESSFUL)
		{
		// Display the error message and quit the program
			MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
			ok = false;
	//		PostQuitMessage(0);
		}
		else ok = true;
	}
	else 
		ok = true;

	if(ok)
	{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(handle, &wp);
		// Adjust width and height to required
		conf->setup("savewindow.width", conf->geti("window.width"));
		conf->setup("savewindow.height", conf->geti("window.height"));

		wp.rcNormalPosition.left	=	0;
		wp.rcNormalPosition.top		=	0;
		wp.rcNormalPosition.right	=	conf->geti("screen.width");
		wp.rcNormalPosition.bottom	=	conf->geti("screen.height");
		
		SetWindowPlacement(handle, &wp);

		SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

//		SetWindowLong(handle, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	}
	return ok;
}

bool OpenGL_WIN_Engine::createWindow()
{
	HWND hWnd;
	WNDCLASS wndclass;

	memset(&wndclass, 0, sizeof(WNDCLASS));				// Init the size of the class
	wndclass.style = 0;
  //  CS_HREDRAW   | CS_VREDRAW	// Regular drawing capabilities
	wndclass.lpfnWndProc = WinProc;						// Pass our function pointer as the window procedure
	wndclass.hInstance = hInstance;						// Assign our hInstance
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION );	// General icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// An arrow for the cursor
//	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	// A white window
//    u nas black :)
	wndclass.lpszClassName = "SteelWindow";			// Assign the class name

	if(!RegisterClass(&wndclass)) 
	{
		alog.msg("error graph opengl", std::string("Cannot register class ") + wndclass.lpszClassName);
		return false;							// Register the class
	}
	
	dwStyle =  WS_OVERLAPPEDWINDOW |
        WS_CLIPSIBLINGS  |        WS_CLIPCHILDREN;

	if(conf->geti("fullscreen")) 						// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
//		ShowCursor(FALSE);								// Hide the cursor
	}
	else if(dwStyle)									// Assign styles to the window depending on the choice
		dwStyle =
        WS_OVERLAPPEDWINDOW |
        WS_CLIPSIBLINGS  |        WS_CLIPCHILDREN
        ;

//	g_hInstance = w.hInstance;							// Assign our global hInstance to the window's hInstance

	//AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	hWnd = CreateWindow("SteelWindow", "Steel Engine", dwStyle, 
		conf->geti("window.left"), conf->geti("window.top"),
						conf->geti("window.width"), conf->geti("window.height"),
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return false;								// If we could get a handle, return NULL

	window.resize(window.size() + 1);
	window[window.size() - 1].handle	= hWnd;
	window[window.size() - 1].focused	= false;
	window[window.size() - 1].input		= input;
	window[window.size() - 1].engine	= this;

	input->setMouseCenter(conf->geti("window.width")/2, conf->geti("window.height")/2);

	handle = hWnd;

	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(hWnd);									// Draw the window

  	SetFocus(hWnd);										// Sets Keyboard Focus To The Window

    if (handle) DC = GetDC(handle);

	    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    memset(ppfd,0,sizeof(pfd));

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags =
        PFD_DRAW_TO_WINDOW
      | PFD_SUPPORT_OPENGL
      | PFD_GENERIC_ACCELERATED
      | PFD_GENERIC_FORMAT
      | PFD_DOUBLEBUFFER
      ;

    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_RGBA;
    ppfd->cColorBits = conf->geti("screen.depth");
    ppfd->cDepthBits = conf->geti("screen.depth");

    if ( (pixelformat = ChoosePixelFormat(DC, ppfd)) == 0 )    {        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);        return FALSE;    }
    if (SetPixelFormat(DC, pixelformat, ppfd) == FALSE)    {        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);        return FALSE;    }

    RC = wglCreateContext(DC);
    wglMakeCurrent(DC, RC);

	if(conf->geti("fullscreen"))
			   	changeToFullScpeen();							// Go to full screen

	alog.out("Video mode has been set!\n" \
		"\tWindow size: %dx%dx%d\n" ,
		conf->geti("window.width"), conf->geti("window.height"), conf->geti("screen.depth") );

	return_pressed = false;

	return true;
}

bool OpenGL_WIN_Engine::deinit()
{
	return OpenGL_Engine::deinit();
}

void OpenGL_WIN_Engine::setCaption(std::string caption)
{
	SetWindowText(handle, caption.c_str() );
}

#endif
