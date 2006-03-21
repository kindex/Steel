#include "opengl_engine.h"

#include <iostream>

#include "../../utils.h"
#include "../../common/logger.h"

#include "../../res/image/image.h"
#include "extensions.h"


void OpenGL_Engine::drawElement(DrawElement &e)
{
	glLoadMatrixf(e.matrix.entries);
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

		GLuint texture = getTexture(m->var_s->operator []("diffuse_map"));

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

		glBegin(GL_LINES);
		for(unsigned int i=0; i < e.vertex->size(); i++)
		{
			v3 s = e.vertex->operator [](i);
			v3 d = e.vertex->operator [](i) + e.normal->operator [](i)*5;

			glVertex3f(s.x, s.y, s.z);
			glVertex3f(d.x, d.y, d.z);
		}

		glEnd();
	}
}


GLuint OpenGL_Engine::getTexture(std::string imageName)
{
	if(registredTextures.find(imageName) != registredTextures.end())
		return registredTextures[imageName];

	Res::res_kind kind;

	res->add(Res::image, imageName);


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




bool OpenGL_Engine::process()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for(vector<DrawElement>::iterator it = elements.begin(); it != elements.end(); it++)
		drawElement((*it));

	glFlush();
	swapBuffers();

	return true;
}



bool OpenGL_Engine::init()
{
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

	SetUpARB_multitexture();
	makeNormalisationCubeMap();

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


void OpenGL_Engine::makeNormalisationCubeMap()
{
	glGenTextures(1, &normalisationCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	GenerateNormalisationCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
