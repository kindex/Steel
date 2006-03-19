#ifndef __GRAPH_OPENGL_ENGINE_H
#define __GRAPH_OPENGL_ENGINE_H

#include <string>
#include <SDL.h>			// Header File For SDL

#include "graph_engine.h"
#ifdef __linux
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#else
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#endif

class OpenGL_Engine: public GraphEngine
{
protected:
	map<std::string, GLuint> registredTextures;

	SDL_Surface *surface;	// SDL surface
    bool createWin();
    void create();
    void repair();


public:
	void processCamera();
	bool init();
	bool process();
	bool deinit();

	void drawElement(DrawElement &e);
	GLuint getTexture(std::string imageName);
};



#endif
