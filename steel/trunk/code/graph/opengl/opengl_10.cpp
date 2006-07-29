/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Функции для рендерига объектов на OpenGL 1.0
 ************************************************************/

#include "opengl_engine.h"

bool OpenGL_Engine::BindTexture_OpenGL10(Image *image)
{
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format;
	if(image->getFormat() == IMAGE_RGB) format = GL_RGB; 
	else
	if(image->getFormat() == IMAGE_NORMAL) format = GL_RGB; 
	else if(image->getFormat() == IMAGE_RGBA) format = GL_RGBA;
	else
		assert(false, "Unsupported image format");

	int width = image->getWidth();
	int heigth = image->getHeight();
	if((width & (width - 1) )!= 0)
		log_msg("graph opengl res error", "Texture width is not power of 2 ("+ IntToStr(width) + ")");
	if((heigth & (heigth - 1)) != 0)
		log_msg("graph opengl res error", "Texture heigth is not power of 2 ("+ IntToStr(heigth) + ")");

    glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image->getWidth(), image->getHeight(),0,
				format,  GL_UNSIGNED_BYTE , image->getBitmap());
	return true;
}

void OpenGL_Engine::DrawFill_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total)
{
	if(material && triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();

        TexCoords *coords = e.object->getTexCoords(0);
		Texture map = material->map[0]; // текущая текстура
	 
		(this->*BindTexture)(map.texture);

        glBegin(GL_TRIANGLES);
         
        for(unsigned int i=0; i<triangles->data.size(); i++)
        {
            glTexCoord2fv(&coords->data[ triangles->data[i].a[0] ].x);              glVertex3fv(&e.vertex->data[ triangles->data[i].a[0] ].x);
            glTexCoord2fv(&coords->data[ triangles->data[i].a[1] ].x);              glVertex3fv(&e.vertex->data[ triangles->data[i].a[1] ].x);
            glTexCoord2fv(&coords->data[ triangles->data[i].a[2] ].x);              glVertex3fv(&e.vertex->data[ triangles->data[i].a[2] ].x);
        }
         
        glEnd();
    }
}


void OpenGL_Engine::DrawWire_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total)
{
	if(triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();
         
        for(unsigned int i=0; i<triangles->data.size(); i++)
        {
	        glBegin(GL_LINE_LOOP);
            glVertex3fv(&e.vertex->data[ triangles->data[i].a[0] ].x);
            glVertex3fv(&e.vertex->data[ triangles->data[i].a[1] ].x);
            glVertex3fv(&e.vertex->data[ triangles->data[i].a[2] ].x);
		    glEnd();
        }
    }
}


void OpenGL_Engine::DrawLines_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total)
{
	if(e.vertex && e.lines && !e.lines->index.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += e.lines->index.size();
		
		glBegin(GL_LINES);
        for(unsigned int i=0; i < e.lines->index.size(); i++)
        {
			// TODO color
            glVertex3fv(&e.vertex->data[ e.lines->index[i].a[0] ].x);
            glVertex3fv(&e.vertex->data[ e.lines->index[i].a[1] ].x);
        }
        glEnd();
    }
}
