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

#include "opengl_win_engine.h"

extern HINSTANCE hInstance;
HWND handle;

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


LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0;
    if (hWnd == handle)
    switch (uMsg)
	{
     case WM_SIZE:										// If the window is resized
 	case WM_PAINT:										// If we need to repaint the scene
        RedrawWindow(hWnd);
		break;

    case WM_CLOSE:										// If the window is being closes
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break;

    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
        break;
    }
    else // ne nashe okno
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);

    return lRet;										// Return by default
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
//	   	ChangeToFullScreen(s,w);							// Go to full screen
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
		conf->geti("left"), conf->geti("top"),
						conf->geti("width"), conf->geti("height"),
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return false;								// If we could get a handle, return NULL
	::handle = hWnd;
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
    ppfd->cColorBits = conf->geti("depth");
    ppfd->cDepthBits = conf->geti("depth");

    if ( (pixelformat = ChoosePixelFormat(DC, ppfd)) == 0 )    {        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);        return FALSE;    }
    if (SetPixelFormat(DC, pixelformat, ppfd) == FALSE)    {        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);        return FALSE;    }

    RC = wglCreateContext(DC);
    wglMakeCurrent(DC, RC);

	alog.out("Video mode has been set!\n" \
		"\tResolution: %dx%dx%d\n" ,
		conf->geti("width"), conf->geti("height"), conf->geti("depth") );



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
