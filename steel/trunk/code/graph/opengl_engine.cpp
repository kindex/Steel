#include "opengl_engine.h"

#include <iostream>

#include "time.h"
#include "../utils.h"
#include "../common/logger.h"
#include "SDL.h"

bool OpenGL_Engine::process()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glVertexPointer(3, GL_FLOAT, 12 /* sizeof(v3)*/ , &vertex[0]);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawElements(GL_TRIANGLES, triangle.size(), GL_UNSIGNED_INT, &triangle[0]);
	glDisableClientState(GL_VERTEX_ARRAY);

	glFlush();
	SDL_GL_SwapBuffers();
	
    if ( updateFPS() )
    {
        window.caption =  window.title + " FPS = " + FloatToStr(curFPS);
		SDL_WM_SetCaption(window.caption.c_str(),"test");
    }

	return true;
}



bool OpenGL_Engine::createWin()
{
	int videoFlags;
	const SDL_VideoInfo *videoInfo;
	
	videoInfo = SDL_GetVideoInfo();
	if ( !videoInfo )
	{
		alog.out("Video query failed: %s\n",SDL_GetError());
		lastError = SE_SDL_VQUERY;
		return false;
	}
	
	videoFlags = SDL_OPENGL;
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
	surface = SDL_SetVideoMode(window.width, window.height, window.bpp, videoFlags);
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
		window.width, window.height, window.bpp, videoInfo->video_mem, \
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
	surface = NULL;
    initTime();
	
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0 )
	{
		alog.out("SDL initialization failed: %s\n",SDL_GetError());
		lastError=SE_SDL_INIT;
		return false;
	}
	
	if (!createWin())
	{
		SDL_Quit();
		lastError = getError();
		return false;
	}
//	glEnable(GL_DEPTH_TEST);
	
	alog.out("OpenGL engine has been initialized!\n");
	
	return true;
}

bool OpenGL_Engine::deinit()
{
	SDL_Quit();
	alog.out("OpenGL engine has been stopped!\n");
	return true;
}




void OpenGL_Engine::processCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    
//	gluPerspective( camera.fov, (float)window.width/window.height, front, back );

	gluPerspective(90.0, 1.0, 0, 1.0e+10);


    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, 
			camera.center.x, camera.center.y, camera.center.z, 
			camera.up.x, camera.up.y, camera.up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
