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

#include "../../steel.h"
#include "../../_cpp.h"

#if STEEL_VIDEOSYS == OPENGL_SDL

#include "opengl_sdl_engine.h"

#ifdef STEEL_USE_GETTEXT
#include <libintl.h>
#endif

void OpenGL_SDL_Engine::swapBuffers()
{
	SDL_GL_SwapBuffers();
}

bool OpenGL_SDL_Engine::createWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		error("graph sdl",std::string(_("SDL initialization has failed: "))+SDL_GetError());
		setError(SE_SDL_INIT);
		return false;
	}

	int videoFlags;
	const SDL_VideoInfo *videoInfo;
	
	videoInfo = SDL_GetVideoInfo();
	if ( !videoInfo )
	{
		error("graph sdl",std::string(_("Video query has failed: "))+SDL_GetError());
		setError(SE_SDL_VQUERY);
		SDL_Quit();
		return false;
	}
	
	videoFlags  = 
		SDL_OPENGL | 
		SDL_GL_DOUBLEBUFFER | 
		SDL_HWPALETTE;

	if(conf->geti("fullscreen"))
		videoFlags |= SDL_FULLSCREEN;
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	if ( (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 ) == -1) ||
		(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) == -1) ||
		(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) == -1) ||
    	(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) == -1) ||
		(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32) == -1) )
		//(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32) == -1) ) СЃ СЌС‚РёРј РЅРµ СЂР°Р±РѕС‚Р°РµС‚..
	{
		error("graph sdl opengl",std::string(_("Setting the GL attribute has failed: "))+SDL_GetError());
		setError(SE_OGL_INIT);
		SDL_Quit();
		return false;
	}

	surface = SDL_SetVideoMode(conf->geti("window.width"), conf->geti("window.height"), conf->geti("screen.depth"), videoFlags);
	if ( !surface )
	{
		error("graph sdl",std::string(_("Setting the video mode has failed: "))+SDL_GetError());
		setError(SE_SDL_VIDEO);
		SDL_Quit();
		return false;
	}
	
	log_msg("graph sdl opengl", std::string(_("Video mode has been set!\n"))+ \
    	_("\tResolution: ")+ \
			IntToStr(conf->geti("window.width"))+"x"+ \
			IntToStr(conf->geti("window.height"))+"x"+ \
			IntToStr(conf->geti("screen.depth"))+"\n"+ \
    	_("\tVideo memory: ") + IntToStr(videoInfo->video_mem) + "K\n"+ \
    	_("\tHardware surface: ") + (videoInfo->hw_available?_("yes"):_("no")) + "\n"+ \
    	_("\tWindow manager: ") + (videoInfo->wm_available?_("yes"):_("no")) + "\n" + \
    	_("\tHardware blits: ") + (videoInfo->blit_hw?_("yes"):_("no")) + "\n" + \
    	_("\tHardware colourkey blits: ") + (videoInfo->blit_hw_CC?_("yes"):_("no")) + "\n" + \
    	_("\tHardware alpha blits: ") + (videoInfo->blit_hw_A?_("yes"):_("no")) + "\n" + \
    	_("\tSoftware blits: ") + (videoInfo->blit_sw?_("yes"):_("no")) + "\n" + \
    	_("\tSoftware colourkey blits: ") + (videoInfo->blit_sw_CC?_("yes"):_("no")) + "\n" + \
    	_("\tSoftware alpha blits: ") + (videoInfo->blit_sw_A?_("yes"):_("no")) + "\n" + \
    	_("\tAccelerated colour fills: ") + (videoInfo->blit_fill?_("yes"):_("no")) + "\n");
	
	return true;
}

bool OpenGL_SDL_Engine::deinit()
{
	SDL_Quit();
	return OpenGL_Engine::deinit();
}

void OpenGL_SDL_Engine::setCaption(std::string caption)
{
	SDL_WM_SetCaption(caption.c_str(),caption.c_str());
}

#endif
