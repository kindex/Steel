#include "opengl_engine.h"
#include "steel.h"

#include "time.h"
#include "../utils.h"
#include <iostream>
#ifdef __linux
#include "../common/logger.h"
#include "SDL.h"
#endif

// Override
Window *window;

void onResize(int w, int h)
{
    window->width = w; // TODO
    window->height= h; // TODO

    glViewport( 0, 0, w,h);
//    setCamera();
}

#ifndef __linux
LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0;
    if (hWnd == window->handle)
    switch (uMsg)
	{
     case WM_SIZE:										// If the window is resized
//		if(!screen.fullscreen)								// Do this only if we are in window mode
		{
            onResize(LOWORD(lParam),HIWORD(lParam));
			window->needupdate = true;
//			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
        //    videoSwapBuffers();
//            RedrawWindow(hWnd);
		}
        break;

	case WM_PAINT:										// If we need to repaint the scene
		window->needupdate = true;
//        RedrawWindow(hWnd);
		break;

	case WM_LBUTTONDOWN:								// If we hit the left mouse button
		break;

	case WM_RBUTTONDOWN:								// If we hit the right mouse button
		break;
	case WM_KEYDOWN:
//        processKeyboard(wParam);
		switch(wParam) 
		{								// Check if we hit a key
			case VK_ESCAPE:								// If we hit the escape key
				PostQuitMessage(0);						// Send a QUIT message to the window
				break;
		}
		break;

    case WM_CLOSE:										// If the window is being closes
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break;

    default:											// Return by default
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    else // ne nashe okno
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
    return lRet;
//	return DefWindowProc (hWnd, uMsg, wParam, lParam);
}

HWND CreateMyWindow(Window &w)
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

	if(!RegisterClass(&wndclass)) return false;							// Register the class

/*	if(s.fullscreen && !w.dwStyle) 						// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		w.dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	   	ChangeToFullScreen(s,w);							// Go to full screen
//		ShowCursor(FALSE);								// Hide the cursor
	}
	else if(!w.dwStyle)									// Assign styles to the window depending on the choice*/
		w.dwStyle =
        WS_OVERLAPPEDWINDOW |
        WS_CLIPSIBLINGS  |        WS_CLIPCHILDREN
        ;

//	g_hInstance = w.hInstance;							// Assign our global hInstance to the window's hInstance

//	AdjustWindowRect( &rWindow, w.dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	hWnd = CreateWindow("SteelWindow", w.caption.c_str(), w.dwStyle, w.left, w.top,
						w.width, w.height,
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;								// If we could get a handle, return NULL

	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(hWnd);									// Draw the window

  	SetFocus(hWnd);										// Sets Keyboard Focus To The Window

	return hWnd;
}
#endif	// __linux

bool Window::createWin()
{
//    handle = opengl->opengl->Handle; DOWNGRADE
	#ifdef __linux
	int videoFlags;
	const SDL_VideoInfo *videoInfo;
	
	videoInfo=SDL_GetVideoInfo();
	if ( !videoInfo )
	{
		alog.out("Video query failed: %s\n",SDL_GetError());
		SDL_Quit();
		return false;
	}
	
	videoFlags=SDL_OPENGL;
	videoFlags|=SDL_GL_DOUBLEBUFFER;
	videoFlags|=SDL_HWPALETTE;
	videoFlags|=SDL_RESIZABLE;
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	handle = SDL_SetVideoMode(width,height,bpp,videoFlags);
	if ( !handle )
	{
		alog.out("Video mode set failed: %s\n",SDL_GetError());
		SDL_Quit();
		return false;
	}
	return true;
	
	#else	// __linux
    handle = CreateMyWindow(*this);

    if (handle) 
		DC = GetDC(handle);
	if(DC)
		return true;
	else
		return false;
	#endif	// __linux
}

/*bool glInitExtensions(HDC &DC)
{
//    wglGetExtensionsStringARB=    (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
//    if (wglGetExtensionsStringARB == NULL) return false;

    SetUpARB_multitexture();
//    checkExt(DC,"GL_ARB_multitexture");
    bumpSupported = glActiveTextureARB && glClientActiveTextureARB;
    cubeMap = checkExt(DC,"GL_EXT_texture_cube_map");
    if (!cubeMap) console.writeln(0,"WARNING: GL_EXT_texture_cube_map not supported!");

    return true;

//	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
// 	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

//    return true;
}*/

#ifdef __linux
bool glInit( void )
#else
bool glInit(HDC &DC, HGLRC &RC)
#endif
{
	#ifndef __linux
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

    ppfd->cColorBits = window->bpp;
    ppfd->cDepthBits = window->bpp;

    if ( (pixelformat = ChoosePixelFormat(DC, ppfd)) == 0 )    {        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);        return FALSE;    }
    if (SetPixelFormat(DC, pixelformat, ppfd) == FALSE)    {        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);        return FALSE;    }

    RC = wglCreateContext(DC);
    wglMakeCurrent(DC, RC);
	#endif
	
    glEnable(GL_DEPTH_TEST);

 //   glInitExtensions(DC);

    return true;
}



bool OpenGL_Engine::init()
{
    initTime();
	
	this->window = new Window;
	::window = this->window;
	
	#ifdef __linux
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0 )
	{
		alog.out("SDL initialization failed: %s\n",SDL_GetError());
		return false;
	}
	
	if ( !window->createWin() )
		return false;
	if ( !glInit() )
	{
		SDL_Quit();
		return false;
	}
	
	#else	// __linux
	if(!window->createWin())return false;
	if(!glInit(window->DC, window->RC)) return false;
	#endif	// __linux
	
	window->needupdate = true;
	return true;
		//&& glInitExtensions(window->DC);
}

/*void OpenGL_Engine::clearWindow()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}*/


bool OpenGL_Engine::process()
{
	//static int c=0;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -1.0f);
		glVertex2f(0.0f, +1.0f);
		glVertex2f(0.0f, +1.0f);

		glVertex2f(+0.5f,-1.0f);
		glVertex2f(-0.275f, -0.1f);
		glVertex2f(+0.275f, -0.1f);
	glEnd();

	//std::cout << c++ << std::endl;
	
	glVertexPointer(3, GL_FLOAT, sizeof(v3), &vertex[0]);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawElements(GL_TRIANGLES, triangle.size(), GL_UNSIGNED_SHORT, &triangle[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	

// 
//    DrawScene();
    updateTime();
	
    getfps();
    if (fpsupdated)
    {
        window->caption =  window->title + "FPS = " + FloatToStr(getfps());
		SDL_WM_SetCaption(window->caption.c_str(),"test");
        //SetWindowText(window->handle, window->caption.c_str() );
    }


	glFlush();
	#ifdef __linux
	//SDL_Flip(window->handle);
	#endif
	//SwapBuffers(window->DC);

	window->needupdate = false;
	return true;
}

bool OpenGL_Engine::deinit()
{
	return true;
}


Window::Window() 
{ 
	#ifdef __linux
	handle=NULL;
	#else
	DC = 0; 	RC = 0; 	handle = 0; 
	#endif

	title	= "Steel Demo"; 
	width	= 975; 
	height	= width*3/4; 
	left	= 0; 
	top		= 0; 
	bpp		= 32;
	needupdate = false;
}
