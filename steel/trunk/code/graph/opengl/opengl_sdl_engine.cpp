/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine SDL
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		Часть графического движока OpenGL, которая реализует
		системно зависимые функции, такие как создание окна.
 ************************************************************/

#include "../../_cpp.h"

#ifdef STEEL_OPENGL_SDL

#include "opengl_sdl_engine.h"

void OpenGL_SDL_Engine::swapBuffers()
{
	SDL_GL_SwapBuffers();
}

bool OpenGL_SDL_Engine::createWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		steel::log.out("SDL initialization failed: %s\n",SDL_GetError());
		lastError = SE_SDL_INIT;
		return false;
	}

	int videoFlags;
	const SDL_VideoInfo *videoInfo;
	
	videoInfo = SDL_GetVideoInfo();
	if ( !videoInfo )
	{
		steel::log.out("Video query failed: %s\n",SDL_GetError());
		lastError = SE_SDL_VQUERY;
		SDL_Quit();
		return false;
	}
	
	videoFlags  = 
		SDL_OPENGL | 
		SDL_GL_DOUBLEBUFFER | 
		SDL_HWPALETTE;

	if(conf->geti("fullscreen"))
	videoFlags |= SDL_FULLSCREEN;

//	videoFlags |= SDL_OPENGLBLIT;
//	videoFlags|=SDL_RESIZABLE;
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,   1 );
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8 ); // min 8bit red
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8 ); // min 8bit green
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8 ); // min 8bit blue	
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	surface = SDL_SetVideoMode(conf->geti("window.width"), conf->geti("window.height"), conf->geti("screen.depth"), videoFlags);
	if ( !surface )
	{
		steel::log.out("Video mode set failed: %s\n",SDL_GetError());
		lastError = SE_SDL_VIDEO;
		SDL_Quit();
		return false;
	}
	
    	steel::log.out("Video mode has been set!\n" \
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
    		conf->geti("window.width"), conf->geti("window.height"), conf->geti("screen.depth"), videoInfo->video_mem, \
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

bool OpenGL_SDL_Engine::deinit()
{
	SDL_Quit();
	return OpenGL_Engine::deinit();
}

void OpenGL_SDL_Engine::setCaption(std::string caption)
{
	SDL_WM_SetCaption(caption.c_str(),"test");
}

#endif
