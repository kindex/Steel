/*id*********************************************************
	File: graph/opengl/opengl_win_engine.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Часть графического движока OpenGL, которая реализует
		системно зависимые функции, такие как создание окна.
	TODO
		fullscreen <-> window mode
		set refresh rate
		restore DC on DC lose
 ************************************************************/

#include "../../steel.h"

#if STEEL_OS == OS_WIN32

#include "opengl_engine.h"
#include "../../input/input_win.h"
#include "../../common/logger.h"
#include "../../common/utils.h"
#include <sstream>
#include "ext/libext.h"


extern HINSTANCE hInstance;

namespace opengl
{

static pvector<OpenGL_Engine*> engines;

struct WindowInformationWinAPI: public OpenGL_Engine::WindowInformation
{
	HWND handle;
	bool focused, return_pressed;

	HDC		DC;    // Window Handle, Device Context
	HGLRC	RC; // Rendering Context - for OpenGL
	DWORD	dwStyle;
	InputWin* input;

	WindowInformationWinAPI(): input(NULL) {}
};

bool OpenGL_Engine::RepairOpenGL_Window_WinAPI() // repair window on resize
{
	glViewport( 0, 0, conf->geti("window.width"), conf->geti("window.height"));
	((WindowInformationWinAPI*)windowInformation)->input->		
		setMouseCenter(	conf->geti("window.width")/2, 
						conf->geti("window.height")/2);
	return true;
}

bool OpenGL_Engine::FlushOpenGL_Window_WinAPI()
{
    if (((WindowInformationWinAPI*)windowInformation)->DC)
	{
		glFlush();
		SwapBuffers(((WindowInformationWinAPI*)windowInformation)->DC);
		return true;
	}
	else
		return false;
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
	for EACH(pvector<OpenGL_Engine*>, engines, it)
	{
		WindowInformationWinAPI &win = *((WindowInformationWinAPI*)(*it)->windowInformation);
	    if(hWnd == win.handle)
		{
			switch (uMsg)
			{
	/* 	case WM_PAINT:										// If we need to repaint the scene
			RedrawWindow(hWnd);
			break;*/

	//	case WM_KILLFOCUS:
			case WM_SIZE:
			(*it)->onResize(LOWORD(lParam),HIWORD(lParam));
			break;


			// TODO
			case WM_CLOSE:										// If the window is being closes
			    PostQuitMessage(0);								// Send a QUIT Message to the window
				break;

			case WM_KEYDOWN:
				if(wParam == VK_RETURN && !win.return_pressed)
				{
//					*it->engine->fullScpeenToggle();
					win.return_pressed = true;
				}

				lRet = win.input->ProcessMessage_WinAPI(hWnd, uMsg, wParam, lParam);
				break;
			case WM_KEYUP:
				if(wParam == VK_RETURN)
					win.return_pressed = false;

				lRet = win.input->ProcessMessage_WinAPI(hWnd, uMsg, wParam, lParam);
				break;

			default:											// Return by default
				lRet = win.input->ProcessMessage_WinAPI(hWnd, uMsg, wParam, lParam);
				break;
			}
			return lRet;
		}
	}

    // ne nashe okno
    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);

    return lRet;										// Return by default
}


/*void OpenGL_WIN_Engine::fullScpeenToggle()
{
	if(conf->geti("fullscreen"))
	{
		if(changeToWindow()) conf->setup("fullscreen", 0);
	}
	else
	{
		if(changeToFullScpeen()) conf->setup("fullscreen", 1);
	}


}*/
/*
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
}*/

bool OpenGL_Engine::CreateOpenGL_Window_WinAPI(Input* input)
{
	WNDCLASS wndclass;

	windowInformation = new WindowInformationWinAPI;

	memset(&wndclass, 0, sizeof(WNDCLASS));				// Init the size of the class
	wndclass.style = 0;
  //  CS_HREDRAW   | CS_VREDRAW	// Regular drawing capabilities
	wndclass.lpfnWndProc = WinProc;						// Pass our function pointer as the window procedure
	wndclass.hInstance = hInstance;						// Assign our hInstance
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION );	// General icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// An arrow for the cursor
//	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	// A white window
//    u nas black :)
	wndclass.lpszClassName = "steel";			// Assign the class name

	if(!RegisterClass(&wndclass)) 
	{
		log_msg("error graph opengl", std::string("Cannot register class ") + wndclass.lpszClassName);
		return false;							// Register the class
	}
	
	((WindowInformationWinAPI*)windowInformation)->dwStyle =  
          WS_OVERLAPPEDWINDOW
        | WS_CLIPSIBLINGS
        | WS_CLIPCHILDREN;

	if (conf->geti("window.fullscreen")) 						// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		((WindowInformationWinAPI*)windowInformation)->dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
//		ShowCursor(FALSE);								// Hide the cursor
	}
	else if (((WindowInformationWinAPI*)windowInformation)->dwStyle)									// Assign styles to the window depending on the choice
    {
		((WindowInformationWinAPI*)windowInformation)->dwStyle =
              WS_OVERLAPPEDWINDOW 
            | WS_CLIPSIBLINGS  
            | WS_CLIPCHILDREN;

        if (conf->getb("window.maximized", true))
        {
            ((WindowInformationWinAPI*)windowInformation)->dwStyle |= WS_MAXIMIZE;
        }
    }

//	g_hInstance = w.hInstance;							// Assign our global hInstance to the window's hInstance

	//AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	((WindowInformationWinAPI*)windowInformation)->handle = 
        CreateWindow("steel", 
                     "Steel Engine", 
                     ((WindowInformationWinAPI*)windowInformation)->dwStyle, 
		             conf->geti("window.left"),
                     conf->geti("window.top"),
					 conf->geti("window.width"),
                     conf->geti("window.height"),
					 NULL,
                     NULL,
                     hInstance,
                     NULL);

	if (!((WindowInformationWinAPI*)windowInformation)->handle)
    {
        error("opengl", "Cannot get window handle");
        return false;								// If we could get a handle, return NULL
    }

	engines.push_back(this);

	((WindowInformationWinAPI*)windowInformation)->focused	= false;
	((WindowInformationWinAPI*)windowInformation)->input	= static_cast<InputWin*>(input);
	input->setMouseCenter(conf->geti("window.width")/2, conf->geti("window.height")/2);

	ShowWindow(((WindowInformationWinAPI*)windowInformation)->handle, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(((WindowInformationWinAPI*)windowInformation)->handle);									// Draw the window

  	SetFocus(((WindowInformationWinAPI*)windowInformation)->handle);										// Sets Keyboard Focus To The Window

    if(((WindowInformationWinAPI*)windowInformation)->handle) ((WindowInformationWinAPI*)windowInformation)->DC = GetDC(((WindowInformationWinAPI*)windowInformation)->handle);

    PIXELFORMATDESCRIPTOR  pfd;
    PIXELFORMATDESCRIPTOR* ppfd;
    int pixelformat;

    ppfd = &pfd;

    memset(ppfd, 0, sizeof(pfd));

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags =
        PFD_DRAW_TO_WINDOW
      | PFD_SUPPORT_OPENGL
      | PFD_GENERIC_ACCELERATED
      | PFD_GENERIC_FORMAT;

	if (conf->geti("swapBuffers", 1))
    {
	    ppfd->dwFlags |= PFD_DOUBLEBUFFER;
    }

    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_RGBA;
    ppfd->cColorBits = conf->geti("screen.depth");
    ppfd->cDepthBits = conf->geti("screen.depth");
	ppfd->cStencilBits = conf->getb("shadows") ? 32 : 0;

    if ((pixelformat = ChoosePixelFormat(((WindowInformationWinAPI*)windowInformation)->DC, ppfd)) == 0)
    {        
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return false;
    }
    if (SetPixelFormat(((WindowInformationWinAPI*)windowInformation)->DC, pixelformat, ppfd) == FALSE)
    {        
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return false;
    }

    ((WindowInformationWinAPI*)windowInformation)->RC = wglCreateContext(((WindowInformationWinAPI*)windowInformation)->DC);
    wglMakeCurrent(((WindowInformationWinAPI*)windowInformation)->DC, ((WindowInformationWinAPI*)windowInformation)->RC);

	//if(conf->geti("fullscreen"))
	  // 	changeToFullScpeen();							// Go to full screen

    std::stringstream s;
	s << "Video mode has been set! (" 
		<< conf->geti("window.width") << "x"
		<< conf->geti("window.height") << "x"
		<< conf->geti("screen.depth");
	if (conf->getb("window.maximized", true))
	{
		s << ",maximized";
	}
	if (conf->geti("window.fullscreen"))
	{
		s << ",fullscreen";
	}
	s << ")";

	log_msg("opengl graph", s.str());

	((WindowInformationWinAPI*)windowInformation)->return_pressed = false;

    static_cast<InputWin*>(input)->start();

	return true;
}


bool OpenGL_Engine::setCaptionOpenGL_Window_WinAPI(const std::string& caption)
{
	SetWindowText(((WindowInformationWinAPI*)windowInformation)->handle, caption.c_str() );
	return true;
}

bool OpenGL_Engine::DeleteOpenGL_Window_WinAPI() // repair window on resize
{
	// TODO
	return true;
}

} // namespace opengl


#endif
