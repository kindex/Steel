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

#include "opengl_10.h"

void DrawFill_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total)
{
	if(material && triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();

        TexCoords *coords = e.object->getTexCoords(0);
		Texture map = material->map[0]; // текущая текстура
	 
		glEnable(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int format;
		if(map.texture->getFormat() == IMAGE_RGB) format = GL_RGB; 
		else
		if(map.texture->getFormat() == IMAGE_NORMAL) format = GL_RGB; 
		else if(map.texture->getFormat() == IMAGE_RGBA) format = GL_RGBA;
		else
			assert(false, "Unsupported image format");


        glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, map.texture->getWidth(), map.texture->getHeight(),0,
					format,  GL_UNSIGNED_BYTE , map.texture->getBitmap());
					
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


void DrawWire_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total)
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


void DrawLines_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total)
{
	if(e.vertex && e.lines && !e.lines->empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += e.lines->size();
		
		glBegin(GL_LINES);
        for(unsigned int i=0; i < e.lines->size(); i++)
        {
            glVertex3fv(&e.vertex->data[ e.lines->at(i).a[0] ].x);
            glVertex3fv(&e.vertex->data[ e.lines->at(i).a[1] ].x);
        }
        glEnd();
    }
}
