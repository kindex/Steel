/*id*********************************************************
	File: graph/opengl/opengl_15.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 1.5

		Buffer Objects
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "ext/libext.h"
#include "../material.h"

namespace opengl
{

void OpenGL_Engine::CleanupDrawTriangles_OpenGL15()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

// нарисовать множество полигонов с указанным материалом / VBO
void OpenGL_Engine::DrawTriangles_OpenGL15(GraphShadow& e, const Faces& faces, const TexCoords* coords)
{
	if (e.vertexes && !(faces.triangles.empty() && faces.quads.empty()) && !e.vertexes->empty())// если есть полигоны и вершины
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		total.vertexCount += e.vertexes->size();
		total.faceCount += faces.triangles.size();
			
//		if(coords)	{ glTexCoordPointer(2, GL_FLOAT, 0, &coords->data.front());	glEnableClientState(GL_TEXTURE_COORD_ARRAY); }

		if (BindVBO(e.vertexes, GL_VERTEX_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
		{
			glVertexPointer(3, GL_FLOAT, 0, 0);
		}

		if (BindVBO(e.normals, GL_NORMAL_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
		{
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		//Draw All
		if (!faces.triangles.empty())
		{
			total.batchCount++;
			if (BindVBO(&faces.triangles, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 3))
			{
				glDrawElements(GL_TRIANGLES, faces.triangles.size()*3, GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawElements(GL_TRIANGLES, faces.triangles.size()*3/*a,b,c*/, GL_UNSIGNED_INT, &faces.triangles.front().a[0]);
			}
		}

		if (!faces.quads.empty())
		{
			total.batchCount++;
			if (BindVBO(&faces.quads, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 4))
			{
				glDrawElements(GL_QUADS, faces.quads.size()*4, GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawElements(GL_QUADS, faces.quads.size()*4, GL_UNSIGNED_INT, &faces.quads.front().a[0]);
			}
		}

		glPopClientAttrib();
		glPopAttrib();
	}
}

void OpenGL_Engine::BindTexCoords_OpenGL15(const TexCoords* coords, const TextureMatrix* textureMatrix)
{
	if (coords != NULL)	
	{ 
		if (BindVBO(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 2))
		{
			glTexCoordPointer(2, GL_FLOAT, 0,0);

			if (textureMatrix != NULL && textureMatrixLevel == 0)
			{
				glMatrixMode(GL_TEXTURE);	glLoadIdentity();
				glScalef(		textureMatrix->texCoordsScale.x,		textureMatrix->texCoordsScale.y,		textureMatrix->texCoordsScale.z); 
				glTranslatef(	textureMatrix->texCoordsTranslation.x,	textureMatrix->texCoordsTranslation.y,	textureMatrix->texCoordsTranslation.z); 
				glRotatef(		textureMatrix->texCoordsRotation/(float)(M_PI)*180.0f, 0.0f, 0.0f, 1.0f); 
				textureMatrixLevel++;
			}
		}
	}
}

void OpenGL_Engine::BindTexCoords3f_OpenGL15(const TexCoords3f* coords)
{
	if(coords != NULL)
	{ 
		if (BindVBO(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
		{
			glTexCoordPointer(3, GL_FLOAT, 0,0);
		}
	}
}

void OpenGL_Engine::cleanBuffer(uid bufId)
{
	if(GL_EXTENSION_VBO)
	{
		glDeleteBuffersARB(1, &buffer[bufId].glid);
	}

	buffer.erase(bufId);
}

template<class Class> 
bool OpenGL_Engine::BindVBO(Class* v, int mode, int mode2, int elCnt)
{
	if (v == NULL || v->empty()) return false;

	if (GL_EXTENSION_VBO) // Vertex Buffer Object supportd
	{
		uid	id = v->getId();
		if (id == 0)
		{
			if (mode != 0) 
			{
				glEnableClientState(mode);
			}

			glBindBufferARB(mode2, 0);
			return false;
		}

		bool loaded = false;
		if(buffer.find(id) != buffer.end())
		{
			loaded = buffer[id].loaded;
		}

		OpenGL_Buffer &buf = buffer[id];

		if (loaded && buf.size != v->size())
		{
			glDeleteBuffersARB(1, &buf.glid);
			buf.loaded = false;
			loaded = false;
		}

		if (loaded)
		{
			glBindBufferARB(mode2, buf.glid);
			if (v->changed)
			{
				glBufferSubDataARB(mode2, 0, elCnt*sizeof(float)*v->size(), &v->front());
				buf.loadCnt++;
			}

			if (mode)
			{
				glEnableClientState(mode);
			}

			buf.lastUsedTime = info.timeInfo.currentTime;
			buf.usedCnt++;
//			buf.temp = false;
		}
		else
		{
			glGenBuffersARB(1, &buf.glid);

			if(mode > 0) glEnableClientState ( mode );
			glBindBufferARB(mode2, buf.glid);

			GLenum usage = GL_STATIC_DRAW;
			if (v->changed)
			{
				usage = GL_STREAM_DRAW;
			}

			glBufferDataARB(mode2, elCnt*sizeof(float)*v->size(), &v->front(), usage);
			buf.size = v->size();
	
			buf.loaded = true;
			buf.loadCnt++;
			if (mode2 == GL_ARRAY_BUFFER_ARB)
			{
				buf.kind = OpenGL_Buffer::array;
			}
			if (mode2 == GL_ELEMENT_ARRAY_BUFFER_ARB)
			{
				buf.kind = OpenGL_Buffer::index;
			}

//			buf.temp = false;
			buf.lastUsedTime = info.timeInfo.currentTime;
			buf.usedCnt++;
		}
		return true;
	}  
	else
	{
		return false;
	}
}

} // namespace opengl
