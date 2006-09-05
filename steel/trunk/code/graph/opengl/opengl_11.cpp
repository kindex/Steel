/*id*********************************************************
	File: graph/opengl/opengl_11.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 1.1
		Используется glVertexPointer и BindTexture
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "gl/libext.h"

// нарисовать множество полигонов с указанным материалом / glVertexPointer
void OpenGL_Engine::DrawTriangles_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, TexCoords *coords, GraphEngine::GraphTotalInfo &total)
{
	if(triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();
			
		if(coords)	
		{ 
			glTexCoordPointer(2, GL_FLOAT, 0, &coords->data.front());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

/*		RUNTIME ERROR: glDrawElements вылезает в плохую область памяти, если передаются нормали
		if(e.normal) // TODO check array length
		{
			glVertexPointer(3, GL_FLOAT, 0, &e.normal->data.front());
			glEnableClientState(GL_NORMAL_ARRAY);
		}
*/
		glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data.front());	
		glEnableClientState(GL_VERTEX_ARRAY);

		//Draw All
		glDrawElements(GL_TRIANGLES, triangles->data.size()*3/*a,b,c*/, GL_UNSIGNED_INT, &triangles->data.front().a[0]);

		glPopAttrib();
	}
}

void OpenGL_Engine::BindTexCoords_OpenGL11(TexCoords *coords)
{
	if(coords)	
	{ 
		glTexCoordPointer(2, GL_FLOAT, 0, &coords->data.front());	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
}

void OpenGL_Engine::BindTexCoords3f_OpenGL11(TexCoords3f *coords)
{
	if(coords)
	{ 
		glTexCoordPointer(3, GL_FLOAT, 0, &coords->data.front());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


bool OpenGL_Engine::BindTexture_OpenGL11(Image *image, bool enable)
{
	if(image == NULL) return false;

	uid	id = image->getId();

	bool loaded = false;
	if(buffer.find(id) != buffer.end())
		loaded = buffer[id].loaded;

	OpenGL_Buffer &buf = buffer[id];

	if(loaded)
	{
		switch(image->getDimension())
		{
			case IMAGE_2D:
				if(enable) glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, buf.glid);
				break;
			case IMAGE_CUBE:
				if(GL_EXTENSION_TEXTURE_CUBE_MAP)
				{
					if(enable) glEnable(GL_TEXTURE_CUBE_MAP_ARB);
					glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, buf.glid);
					break;
				}
			default:
				return false; // unsupoerted texture format
		}

//		buf.lastUsedTime = time;
		buf.usedCnt++;
	}
	else
	{
		buf.loaded = false;
		glGenTextures(1, &buf.glid);

		int format;
		if(image->getFormat() == IMAGE_RGB) format = GL_RGB; 
		else
		if(image->getFormat() == IMAGE_NORMAL) format = GL_RGB; 
		else if(image->getFormat() == IMAGE_RGBA) format = GL_RGBA;
		else return false;

		int width = image->getWidth();
		int heigth = image->getHeight();

		switch(image->getDimension())
		{
			case IMAGE_2D:
				if(!GL_EXTENSION_TEXTURE_NON2POWER)
				{
					if((width & (width - 1) )!= 0)
					{
						log_msg("graph opengl res error", "Texture width is not power of 2 ("+ IntToStr(width) + ")");
						return false;
					}
					if((heigth & (heigth - 1)) != 0)
					{
						log_msg("graph opengl res error", "Texture heigth is not power of 2 ("+ IntToStr(heigth) + ")");
						return false;
					}
				}

				if(enable) glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, buf.glid);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


				glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image->getWidth(), image->getHeight(),0,
					format,  GL_UNSIGNED_BYTE , image->getBitmap());
				break;
			case IMAGE_CUBE:
				if(GL_EXTENSION_TEXTURE_CUBE_MAP)
				{
					if(enable) glEnable(GL_TEXTURE_CUBE_MAP_ARB);
					glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, buf.glid);

					int w = image->getWidth();
					int h = image->getHeight();
					int bpp = image->getBpp()/8;

					if(w*6 != h) return false; // 6 images in one

					for(int i=0; i<6; i++)
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i,	0, 
							GL_RGBA8, w, w, 0, format, GL_UNSIGNED_BYTE, image->getBitmap() + w*w*bpp*i);
					}

					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

					break;
				}
			default:
				return false;
		}

		buf.loaded = true;
		buf.loadCnt++;
//		buf.lastUsedTime = time;
		buf.usedCnt++;
	}
	return true;
}




void OpenGL_Engine::DrawWire_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total)
{
	if(triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPolygonMode (GL_FRONT, GL_LINE);

		glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data.front());	glEnableClientState(GL_VERTEX_ARRAY);

		glDrawElements(GL_TRIANGLES, triangles->data.size()*3/*A,b,c*/, GL_UNSIGNED_INT, &triangles->data.front().a[0]);

		glPopAttrib();
	}
}

void OpenGL_Engine::DrawLines_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total)
{
	if(e.vertex && e.lines && !e.lines->index.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += e.lines->index.size();
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// TODO color
		glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data.front());	glEnableClientState(GL_VERTEX_ARRAY);

		glDrawElements(GL_LINES, e.lines->index.size()*2, GL_UNSIGNED_INT, &e.lines->index.front().a[0]);

		glPopAttrib();
    }
}
