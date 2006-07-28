/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Функции для рендерига объектов на OpenGL 1.1.
		Используется glVertexPointer и BindTexture
 ************************************************************/
#include "opengl_10.h"
#include "gl/libext.h"

struct OpenGL_ImageBuffer
{
	GLuint	glid;
	int		usedCnt, loadCnt;
	steel::time lastUsedTime;
	bool	loaded;
};

std::map<uid, OpenGL_ImageBuffer> imageBuffer;

bool bindTexture_OpenGL11(Image *image)
{
	if(image == NULL) return false;

	uid	id = image->getId();

	bool loaded = false;
	if(imageBuffer.find(id) != imageBuffer.end())
		loaded = imageBuffer[id].loaded;

	OpenGL_ImageBuffer &buf = imageBuffer[id];

	if(loaded)
	{
		switch(image->getKind())
		{
			case IMAGE_2D:
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, buf.glid);
				break;
			case IMAGE_CUBE:
				glEnable(GL_TEXTURE_CUBE_MAP_ARB);
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, buf.glid);
				break;
		}

//		buf.lastUsedTime = time;
		buf.usedCnt++;
	}
	else
	{
		glGenTextures(1, &buf.glid);

		int format;
		if(image->getFormat() == IMAGE_RGB) format = GL_RGB; 
		else
		if(image->getFormat() == IMAGE_NORMAL) format = GL_RGB; 
		else if(image->getFormat() == IMAGE_RGBA) format = GL_RGBA;
		else return false;

		switch(image->getKind())
		{
			case IMAGE_2D:
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, buf.glid);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


				glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image->getWidth(), image->getHeight(),0,
					format,  GL_UNSIGNED_BYTE , image->getBitmap());
				break;
			case IMAGE_CUBE: // TODO check extension
				glEnable(GL_TEXTURE_CUBE_MAP_ARB);
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

		buf.loaded = true;
		buf.loadCnt++;
//		buf.lastUsedTime = time;
		buf.usedCnt++;
	}
	return true;
}


void DrawFill_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total)
{
	if(material && triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();

        TexCoords *coords = e.object->getTexCoords(0);
		Texture map = material->map[0]; // текущая текстура

		bindTexture_OpenGL11(map.texture);
					
		glTexCoordPointer(2, GL_FLOAT, 0, &coords->data.front());	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data.front());	glEnableClientState(GL_VERTEX_ARRAY);

		//Draw All
		glDrawElements(GL_TRIANGLES, triangles->data.size()*3/*A,b,c*/, GL_UNSIGNED_INT, &triangles->data.front().a[0]);

		glPopAttrib();
	}
}

void DrawWire_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total)
{
	if(triangles && e.vertex && !triangles->data.empty() && !e.vertex->data.empty())// если есть полигоны и вершины
	{
		total.object++;
		total.vertex += e.vertex->data.size();
		total.triangle += triangles->data.size();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPolygonMode (GL_FRONT, GL_LINE);

		glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data.front());	glEnableClientState(GL_VERTEX_ARRAY);

		//Draw All
		glDrawElements(GL_TRIANGLES, triangles->data.size()*3/*A,b,c*/, GL_UNSIGNED_INT, &triangles->data.front().a[0]);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPopAttrib();
	}
}

void DrawLines_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total)
{
	// TODO
}
