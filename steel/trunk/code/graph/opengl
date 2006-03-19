#include "opengl_engine.h"

#include <iostream>

#include "time.h"
#include "../utils.h"
#include "../common/logger.h"
#include "SDL.h"

#include "../res/image/image.h"


GLuint OpenGL_Engine::getTexture(std::string imageName)
{
	if(registredTextures.find(imageName) != registredTextures.end())
		return registredTextures[imageName];

	Res::res_kind kind;
	Image *a = (Image*)res->get(imageName, kind);
	if(kind != Res::image)
	{
		alog.out("Cannot find image %s", imageName.c_str());
		return -1;
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //{ all of the above can be used }

//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, a->width, a->height,0,
		GL_RGB,  GL_UNSIGNED_BYTE , a->bitmap);

	registredTextures[imageName] = id;
	alog.out("Bind texture %s", imageName.c_str());

	return id;
}


void OpenGL_Engine::drawElement(DrawElement &e)
{
	if(e.triangle && e.vertex)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 12 /* sizeof(v3)*/ , &e.vertex->front());

		Res::res_kind kind;
		Material *m = (Material*)res->get(e.material, kind);
		if(kind != Res::material)
		{
			alog.out("Cannot find material '%s'", e.material.c_str());
			return;
		}

		GLuint texture = getTexture(m->diffuse);

		if(e.mapcoord && texture>=0)
		{
		    glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, texture);
			glTexCoordPointer(2, GL_FLOAT, 2*4, &e.mapcoord->front());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glDrawElements(GL_TRIANGLES, e.triangle->size()*3, GL_UNSIGNED_INT, &(e.triangle->front()));
		glDisableClientState(GL_VERTEX_ARRAY);
		if(e.mapcoord)
		{
		    glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
}


bool OpenGL_Engine::process()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for(vector<DrawElement>::iterator it = elements.begin(); it != elements.end(); it++)
		drawElement((*it));

   glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -5.0f, -20.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f, -5.0f, -20.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  5.0f, -20.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f,  5.0f, -20.0f);
    glEnd();

//	glFlush();
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
	
	videoFlags  = 
		SDL_OPENGL | 
		SDL_GL_DOUBLEBUFFER | 
		SDL_HWPALETTE;
//	videoFlags |= SDL_OPENGLBLIT;
//	videoFlags|=SDL_RESIZABLE;
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,   1 );
/*	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8 ); // min 8bit red
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8 ); // min 8bit green
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8 ); // min 8bit blue	
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);*/



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
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)<0)
	{
		alog.out("SDL initialization failed: %s\n",SDL_GetError());
		lastError = SE_SDL_INIT;
		return false;
	}
	
	if (!createWin())
	{
		SDL_Quit();
		lastError = getError();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 
	glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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
