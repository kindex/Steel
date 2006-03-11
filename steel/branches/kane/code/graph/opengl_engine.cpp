#include "opengl_engine.h"

#include <iostream>

#include "time.h"
#include "../utils.h"
#include "../common/logger.h"
#include "SDL.h"

// Override
Window *window;

void onResize(int w, int h)
{
    window->width = w; // TODO
    window->height= h; // TODO

    glViewport( 0, 0, w,h);
//    setCamera();
}

bool Window::createWin()
{
	int videoFlags;
	const SDL_VideoInfo *videoInfo;
	
	videoInfo=SDL_GetVideoInfo();
	if ( !videoInfo )
	{
		alog.out("Video query failed: %s\n",SDL_GetError());
		lastError=SE_SDL_VQUERY;
		return false;
	}
	
	videoFlags=SDL_OPENGL;
	//videoFlags|=SDL_GL_DOUBLEBUFFER;
	videoFlags|=SDL_HWPALETTE;
	//videoFlags|=SDL_RESIZABLE;
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	surface = SDL_SetVideoMode(width,height,bpp,videoFlags);
	if ( !surface )
	{
		alog.out("Video mode set failed: %s\n",SDL_GetError());
		lastError=SE_SDL_VIDEO;
		return false;
	}
	
	alog.out("Video mode has been set!\n" \
		"\tResolution: %dx%dx%d\n" \
		"\tVideo memory: %dK\n" \
		"\tHardware surface: %s\n" \
		"\tWindow manager: %s\n" \
		"\tHardware blits: %s\n" \
		"\tHardware colourkey blits: %s\n" \
		"\tHardware alpha blits: %s\n" \
		"\tSoftware blits: %s\n" \
		"\tSoftware colourkey blits: %s\n" \
		"\tSoftware alpha blits: %s\n" \
		"\tAccelerated colour fills: %s\n", \
		width,height,bpp,videoInfo->video_mem, \
		videoInfo->hw_available?"yes":"no", \
		videoInfo->wm_available?"yes":"no", \
		videoInfo->blit_hw?"yes":"no", \
		videoInfo->blit_hw_CC?"yes":"no", \
		videoInfo->blit_hw_A?"yes":"no", \
		videoInfo->blit_sw?"yes":"no", \
		videoInfo->blit_sw_CC?"yes":"no", \
		videoInfo->blit_sw_A?"yes":"no", \
		videoInfo->blit_fill?"yes":"no");
	
	
	return true;
}

bool OpenGL_Engine::init()
{
    initTime();
	
	this->window = new Window;
	::window = this->window;
	
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0 )
	{
		alog.out("SDL initialization failed: %s\n",SDL_GetError());
		lastError=SE_SDL_INIT;
		return false;
	}
	
	if ( !window->createWin() )
	{
		SDL_Quit();
		lastError=window->getError();
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	window->needupdate = true;
	
	alog.out("OpenGL engine has been initialized!\n");
	
	return true;
}

bool OpenGL_Engine::process()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glVertexPointer(3, GL_FLOAT, vertex.size() , &vertex[0]);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawElements(GL_TRIANGLES, triangle.size()*3, GL_UNSIGNED_SHORT, &triangle[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glFlush();
	SDL_GL_SwapBuffers();
	
    if ( updateFPS() )
    {
        window->caption =  window->title + " FPS = " + FloatToStr(curFPS);
		SDL_WM_SetCaption(window->caption.c_str(),"test");
    }

	window->needupdate = false;
	return true;
}

bool OpenGL_Engine::deinit()
{
	SDL_Quit();
	alog.out("OpenGL engine has been stopped!\n");
	return true;
}


Window::Window() 
{ 
	surface=NULL;

	title	= "Steel Demo"; 
	width	= 975; 
	height	= width*3/4; 
	bpp		= 32;
	needupdate = false;
}
