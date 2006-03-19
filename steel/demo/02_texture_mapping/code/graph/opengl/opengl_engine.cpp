#include "opengl_engine.h"

#include <iostream>

#include "../time.h"
#include "../../utils.h"
#include "../../common/logger.h"

#include "../../res/image/image.h"


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
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -5.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 5.0f, -5.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 5.0f,  5.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f,  5.0f, 0.0f);
    glEnd();

//	glFlush();
	swapBuffers();

    if ( updateFPS() )
    {
        window.caption =  window.title + " FPS = " + FloatToStr(curFPS);
		setCaption(window.caption);
    }

	return true;
}



bool OpenGL_Engine::init()
{
    initTime();
	
	if (!createWindow())
	{
		lastError = getError();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.01, 1.0e+6);

	glDepthFunc(GL_LESS); 
	glCullFace(GL_BACK);

//    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	alog.out("OpenGL engine has been initialized!\n");

	setCaption("Steel Engine");

	return true;
}

bool OpenGL_Engine::deinit()
{
	alog.out("OpenGL engine has been stopped!\n");
	return true;
}




void OpenGL_Engine::processCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    
//	gluPerspective( camera.fov, (float)window.width/window.height, front, back );

	gluPerspective(90.0, 1.0, 0.01, 1.0e+6);


    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, 
			camera.center.x, camera.center.y, camera.center.z, 
			camera.up.x, camera.up.y, camera.up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
