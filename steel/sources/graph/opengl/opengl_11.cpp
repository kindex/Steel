/*id*********************************************************
	File: graph/opengl/opengl_11.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 1.1
		Используется glVertexPointer и BindTexture
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "../../libs/opengl/libext.h"

namespace opengl
{

// нарисовать множество полигонов с указанным материалом / glVertexPointer
void OpenGL_Engine::DrawTriangles_OpenGL11(GraphShadow& e, const Faces& faces, const TexCoords* coords)
{
	if (e.vertexes != NULL && !(faces.triangles.empty() && faces.quads.empty()) && !e.vertexes->empty())// если есть полигоны и вершины
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		total.vertexCount += e.vertexes->size();
		total.triangleCount += faces.triangles.size();
			
		if (coords)	
		{ 
			glTexCoordPointer(2, GL_FLOAT, 0, &coords->front());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

/*		RUNTIME ERROR: glDrawElements вылезает в плохую область памяти, если передаются нормали
		if(e.normals) // TODO check array length
		{
			glVertexPointer(3, GL_FLOAT, 0, &e.normals->front());
			glEnableClientState(GL_NORMAL_ARRAY);
		}
*/
		glVertexPointer(3, GL_FLOAT, 0, &e.vertexes->front());	
		glEnableClientState(GL_VERTEX_ARRAY);

		if (!faces.triangles.empty())
		{
			total.batchCount++;
			glDrawElements(GL_TRIANGLES, faces.triangles.size()*3, GL_UNSIGNED_INT, &faces.triangles.front().a[0]);
		}
		if (!faces.quads.empty())
		{
			total.batchCount++;
			glDrawElements(GL_QUADS, faces.quads.size()*4, GL_UNSIGNED_INT, &faces.quads.front().a[0]);
		}

		glPopClientAttrib();
		glPopAttrib();
	}
}

void OpenGL_Engine::BindTexCoords_OpenGL11(const TexCoords* coords, const TextureMatrix* textureMatrix)
{
	if (coords != NULL)
	{ 
		glTexCoordPointer(2, GL_FLOAT, 0, &coords->front());	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (textureMatrix != NULL && textureMatrixLevel == 0)
	{
		glMatrixMode(GL_TEXTURE);	glLoadIdentity();
		glScalef(		textureMatrix->texCoordsScale.x,		textureMatrix->texCoordsScale.y,		textureMatrix->texCoordsScale.z); 
		glTranslatef(	textureMatrix->texCoordsTranslation.x,	textureMatrix->texCoordsTranslation.y,	textureMatrix->texCoordsTranslation.z); 
		glRotatef(		textureMatrix->texCoordsRotation/(float)(M_PI)*180.0f, 0.0f, 0.0f, 1.0f); 
		textureMatrixLevel++;
	}
}

void OpenGL_Engine::BindTexCoords3f_OpenGL11(const TexCoords3f* coords)
{
	if (coords != NULL)
	{ 
		glTexCoordPointer(3, GL_FLOAT, 0, &coords->front());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}


bool OpenGL_Engine::BindTexture_OpenGL11(Image& image, bool enable)
{
	uid	id = image.getId();

	bool loaded = false;
	if(buffer.find(id) != buffer.end())
	{
		loaded = buffer[id].loaded;
	}

	OpenGL_Buffer &buf = buffer[id];

	if(loaded)
	{
		switch(image.getDimension())
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
		if(image.getFormat() == IMAGE_RGB) format = GL_RGB; 
		else if(image.getFormat() == IMAGE_RGBA) format = GL_RGBA;
		else return false;

		int width = image.getWidth();
		int heigth = image.getHeight();

		switch(image.getDimension())
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


				glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image.getWidth(), image.getHeight(),0,
					format,  GL_UNSIGNED_BYTE , image.getBitmap());
				break;
			case IMAGE_CUBE:
				if(GL_EXTENSION_TEXTURE_CUBE_MAP)
				{
					if(enable) glEnable(GL_TEXTURE_CUBE_MAP_ARB);
					glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, buf.glid);

					int w = image.getWidth();
					int h = image.getHeight();
					int bpp = image.getBpp()/8;

					if(w*6 != h) return false; // 6 images in one

					for(int i=0; i<6; i++)
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i,	0, 
							GL_RGBA8, w, w, 0, format, GL_UNSIGNED_BYTE, image.getBitmap() + w*w*bpp*i);
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




void OpenGL_Engine::DrawWire_OpenGL11(GraphShadow& e, const Faces& faces)
{
	if (e.vertexes != NULL && !faces.triangles.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.vertexCount += e.vertexes->size();
		total.triangleCount += faces.triangles.size();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		glPolygonMode (GL_FRONT, GL_LINE);
		glVertexPointer(3, GL_FLOAT, 0, &e.vertexes->front());	glEnableClientState(GL_VERTEX_ARRAY);

		total.batchCount++;
		glDrawElements(GL_TRIANGLES, faces.triangles.size()*3/*A,b,c*/, GL_UNSIGNED_INT, &faces.triangles.front().a[0]);

		glPopClientAttrib();
		glPopAttrib();
	}

	if (e.vertexes != NULL && !faces.quads.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.vertexCount += e.vertexes->size();
		total.triangleCount += faces.quads.size();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		glPolygonMode (GL_FRONT, GL_LINE);
		glVertexPointer(3, GL_FLOAT, 0, &e.vertexes->front());	glEnableClientState(GL_VERTEX_ARRAY);

		total.batchCount++;
		glDrawElements(GL_QUADS, faces.quads.size()*4, GL_UNSIGNED_INT, &faces.quads.front().a[0]);

		glPopClientAttrib();
		glPopAttrib();
	}
}

void OpenGL_Engine::DrawLines_OpenGL11(GraphShadow &e)
{
	if (e.vertexes != NULL && e.lines != NULL && !e.lines->index.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.vertexCount += e.vertexes->size();
		total.triangleCount += e.lines->index.size();
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		// TODO color
		glVertexPointer(3, GL_FLOAT, 0, &e.vertexes->front());	glEnableClientState(GL_VERTEX_ARRAY);

		total.batchCount++;
		glDrawElements(GL_LINES, e.lines->index.size()*2, GL_UNSIGNED_INT, &e.lines->index.front().a[0]);

		glPopClientAttrib();
		glPopAttrib();
    }
}

} // namespace opengl
