/*id*********************************************************
	File: graph/opengl/opengl_15.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 1.5

		Buffer Objects
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "gl/libext.h"

void OpenGL_Engine::CleanupDrawTriangles_OpenGL15()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

// нарисовать множество полигонов с указанным материалом / VBO
void OpenGL_Engine::DrawTriangles_OpenGL15(GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total)
{
	if(triangles && e.vertexes && !triangles->data.empty() && !e.vertexes->data.empty())// если есть полигоны и вершины
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		total.vertexCount += e.vertexes->data.size();
		total.triangleCount += triangles->data.size();
			
//		if(coords)	{ glTexCoordPointer(2, GL_FLOAT, 0, &coords->data.front());	glEnableClientState(GL_TEXTURE_COORD_ARRAY); }

		if(BindVBO(e.vertexes, GL_VERTEX_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
		{
			glVertexPointer(3, GL_FLOAT, 0, 0);
		}

		if(BindVBO(e.normals, GL_NORMAL_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
		{
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		//Draw All
		if(BindVBO(triangles, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 3))
		{
			glDrawElements(GL_TRIANGLES, triangles->data.size()*3, GL_UNSIGNED_INT, 0);
		}

		glPopClientAttrib();
		glPopAttrib();
	}
}

void OpenGL_Engine::BindTexCoords_OpenGL15(const TexCoords *coords, const TextureMatrix* textureMatrix)
{
	if(coords != NULL)	
	{ 
		if(BindVBO(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 2))
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

void OpenGL_Engine::BindTexCoords3f_OpenGL15(const TexCoords3f *coords)
{
	if(coords != NULL)
	{ 
		if(BindVBO(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
			glTexCoordPointer(3, GL_FLOAT, 0,0);
	}
}


/*void OpenGL_Engine::drawElement(GraphShadow &e)
{
	steel::vector<uid> buffersToDelete;

	if(e.triangle && e.vertexes && !e.vertexes->data.empty() && !e.triangle->data.empty())// если есть полигоны и вершины
	{
		Material *m = e.material; // получаем материал
		if(m != NULL  && conf->geti("drawFill", 1))
		{
			// загружаем нормали объекта
	
			int texCount = m->map.size();

			int curTexArb = 0;
			// идём по всем картам
			for(int i=0; i<texCount; i++)
			{
				Texture map = m->map[i]; // текущая текстура

				if(glActiveTextureARB)
				{
					glActiveTextureARB(GL_TEXTURE0_ARB + curTexArb);
					glClientActiveTextureARB(GL_TEXTURE0_ARB + curTexArb);
				}
				else if(curTexArb>0) break;

				// режим мультитекстурирования
				GLint mode = GL_REPLACE;
				if(i>0)
				{
					switch(map.mode)
					{
						case TEXTURE_BLEND_MODE_MUL: 	mode = GL_MODULATE; break;
						case TEXTURE_BLEND_MODE_ADD:  mode = GL_ADD; break; // TODO: GL_ADD_SIGNED_ARB ??
						case TEXTURE_BLEND_MODE_BLEND: mode = GL_BLEND; break;
					}
				}

				if(glActiveTextureARB)
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);

				if(conf->geti("drawBump") && !light.empty() && (i==0) && !e.blend && (map.kind == TEXTURE_FORMAT_BUMP_MAP || map.kind == TEXTURE_FORMAT_COLOR_MAP))
				{
					TexCoords *coords = e.object->getTexCoords(i);

					int j = 0;

					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

					uid bufId = objectIdGenerator.genUid();
					buffersToDelete.push_back(bufId);

					if(map.kind == TEXTURE_FORMAT_BUMP_MAP)
					{
						drawBump(e, coords, e.matrix, light[j].pos, bufId, curTexArb, map.texture);
						curTexArb +=2;
					}
					else
					{
						drawBump(e, coords, e.matrix, light[j].pos, bufId, curTexArb, zeroNormal);
						glActiveTextureARB(GL_TEXTURE2_ARB + curTexArb);
						glClientActiveTextureARB(GL_TEXTURE2_ARB + curTexArb);
						glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
						bindTexture(map.texture); // 2D texture (auto detect from Image)
						if(coords && !coords->data.empty())
						{
							if(bind(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 2))
								glTexCoordPointer(2, GL_FLOAT, 0,0);
							else
							{
								glEnable(GL_TEXTURE_COORD_ARRAY);
								glTexCoordPointer(2, GL_FLOAT, 0, &coords->data[0]);
							}
						}
						curTexArb += 3;
					}

				}
				else
				{
					if(conf->geti("drawTexture") && map.kind == TEXTURE_FORMAT_COLOR_MAP) // обычная текстура
					{	// загружем текстуру
						bindTexture(map.texture); // 2D texture (auto detect from Image)
						// загружаем тектурные координаты
						TexCoords *coords = e.object->getTexCoords(i);
						if(coords && !coords->data.empty())
						{
							if(bind(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 2))
								glTexCoordPointer(2, GL_FLOAT, 0,0);
							else
							{
								glEnable(GL_TEXTURE_COORD_ARRAY);
								glTexCoordPointer(2, GL_FLOAT, 0, &coords->data[0]);
							}
						}
					}
					

					if(map.kind == TEXTURE_FORMAT_COLOR) // цвет RGBA
						glColor4f(map.color.r, map.color.g, map.color.b, map.color.a);

					curTexArb++;
				}
			}

			if(e.blend)
			{
				glEnable(GL_BLEND);
				
				if(m->map[0].kind == TEXTURE_FORMAT_COLOR_MAP && m->map[0].texture->getBpp() == 24) // RGB
				{
					if(m->map[0].mode == TEXTURE_BLEND_MODE_ADD)	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
					if(m->map[0].mode == TEXTURE_BLEND_MODE_MUL)	glBlendFunc(GL_DST_COLOR, GL_ZERO);
				}else
//				if(m->map[0].texture->getBpp() == 32) // Alpha
				{
					if(m->map[0].mode == TEXTURE_BLEND_MODE_ADD)	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA); // ?
					if(m->map[0].mode == TEXTURE_BLEND_MODE_MUL)	glBlendFunc(GL_DST_ALPHA, GL_ZERO);
				}
//				glDepthFunc(GL_LESS);
				glDepthMask(GL_FALSE);
			}
// -------------------------------------------------------------------------------
			// загружаем и ресуем треугольники
			if(bind(e.triangle, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 3))
			{
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, 0);
			}
			else
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, &e.triangle->data.front());

// -------------------------------------------------------------------------------

			// откат настроек
			glPopAttrib();

			while(curTexArb >= 0)
			{
				glActiveTextureARB(GL_TEXTURE0_ARB + curTexArb);
				glClientActiveTextureARB(GL_TEXTURE0_ARB + curTexArb);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				curTexArb--;
			}
		}

		if(conf->geti("drawWire"))
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			if(e.material && !e.material->map.empty() &&e.material->map[0].kind == TEXTURE_FORMAT_COLOR) // цвет RGBA
					glColor4f(e.material->map[0].color.r, e.material->map[0].color.g, e.material->map[0].color.b, e.material->map[0].color.a);
			else
				glColor4f(1,1,1,1);

			// загружаем вершины объекта
			if(bind(e.vertexes, GL_VERTEX_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
			{
				glVertexPointer(3, GL_FLOAT, 0, 0);
			}
			else
			{
				glEnable(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, &e.vertexes->data[0]);
			}
			glPolygonMode(GL_FRONT, GL_LINE);  		// Draw Polygons As Wireframes
			glPolygonMode(GL_BACK, GL_LINE); 
			glDepthFunc(GL_LEQUAL); // For blending

			if(bind(e.triangle, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 3))
			{
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, 0);
			}
			else
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, &e.triangle->data[0]);

			glPolygonMode(GL_FRONT, GL_FILL);  

			glPopAttrib ();
		}
	}

	if(e.lines && e.vertexes && !e.vertexes->data.empty() && !e.lines->empty())
	{
		glColor4f(1,1,1,1);
		glBegin(GL_LINES);

		int i = 0;
		for(GLines::iterator it = e.lines->begin(); it != e.lines->end(); it++)
		{
			glVertex3fv(e.vertexes->data[it->a[0]].get3fv());
			glVertex3fv(e.vertexes->data[it->a[1]].get3fv());
		}
		glEnd();
	}

	if(conf->geti("drawNormals", 0))
		drawNormals(e);
	
	if(conf->geti("drawVertexes", 0))
		drawVertexes(e);

	if(conf->geti("drawAABB", 0))
		drawAABB(e, e.matrix);
	glPopMatrix();

	for(steel::vector<uid>::const_iterator it = buffersToDelete.begin(); it != buffersToDelete.end(); it++)
		cleanBuffer(*it);
}*/

void OpenGL_Engine::cleanBuffer(uid bufId)
{
	if(GL_EXTENSION_VBO)
		glDeleteBuffersARB(1, &buffer[bufId].glid);

	buffer.erase(bufId);
}

template<class Class> bool OpenGL_Engine::BindVBO(Class *v, int mode, int mode2, int elCnt)
{
	if(v == NULL || v->data.empty()) return false;

	if(GL_EXTENSION_VBO) // Vertex Buffer Object supportd
	{
		uid	id = v->getId();
		if(id == 0) 
		{
			if(mode) glEnableClientState(mode);
			glBindBufferARB(mode2, 0);
			return false;
		}

		bool loaded = false;
		if(buffer.find(id) != buffer.end())
			loaded = buffer[id].loaded;

		OpenGL_Buffer &buf = buffer[id];

		if(loaded && buf.size != v->data.size())
		{
			glDeleteBuffersARB(1, &buf.glid);
			buf.loaded = false;
			loaded = false;
		}

		if(loaded)
		{
			glBindBufferARB(mode2, buf.glid);
			if(v->changed)
			{
				glBufferSubDataARB(mode2, 0, elCnt*sizeof(float)*v->data.size(), &v->data.front());
				buf.loadCnt++;
			}

			if(mode)glEnableClientState(mode);

			buf.lastUsedTime = time;
			buf.usedCnt++;
//			buf.temp = false;
		}
		else
		{
			glGenBuffersARB(1, &buf.glid);

			if(mode > 0) glEnableClientState ( mode );
			glBindBufferARB(mode2, buf.glid);

			GLenum usage = GL_STATIC_DRAW;
			if(v->changed)
				usage = GL_STREAM_DRAW;

			glBufferDataARB(mode2, elCnt*sizeof(float)*v->data.size(), &v->data.front(), usage);
			buf.size = v->data.size();
	
			buf.loaded = true;
			buf.loadCnt++;
			if(mode2 == GL_ARRAY_BUFFER_ARB)
				buf.kind = OpenGL_Buffer::array;
			if(mode2 == GL_ELEMENT_ARRAY_BUFFER_ARB)
				buf.kind = OpenGL_Buffer::index;

//			buf.temp = false;
			buf.lastUsedTime = time;
			buf.usedCnt++;
		}
		return true;
	}  
	else
		return false;
}
