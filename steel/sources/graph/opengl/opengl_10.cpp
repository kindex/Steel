/*id*********************************************************
	File: graph/opengl/opengl_10.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 1.0
		glBegin - glEnd
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "../../libs/opengl/libext.h"

bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL10(OpenGL_Engine::GraphShadow& e, const Faces& triangles, MaterialStd& material)
{
	total.objectCount++;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	if(BindTexture != NULL && material.diffuse_map.image != NULL) 
	{
		(this->*BindTexture)(*material.diffuse_map.image, true);
	}
	else
	{
		glColor4fv(material.color.getfv());
	}

	const TexCoords* texCoords = NULL;

	if(material.diffuse_map.image != NULL)
	{
		texCoords = e.getTexCoords(material.diffuse_map);
	}

	if(DrawTriangles != NULL) (this->*DrawTriangles)(e, triangles, texCoords);
	if(CleanupDrawTriangles != NULL) (this->*CleanupDrawTriangles)();

	unbindTexCoords();

	glPopClientAttrib();
   	glPopAttrib();
	return true;
}


// нарисовать множество полигонов с указанным материалом / Blend
/*void OpenGL_Engine::DrawFill_OpenGL10(OpenGL_Engine::GraphShadow &e, const Faces *triangles, Material *material)
{
	if(material != NULL)
	{
		total.objectCount++;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		int texCount = material.getTextureCount();

		TextureBlendMode inheritedMode = TEXTURE_BLEND_MODE_NONE, currentMode;

		glDepthFunc(GL_LEQUAL);
		for(int i=0; i<texCount; i++)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
			const Texture *texture = material.getTexture(i); // текущая текстура

			if(inheritedMode == TEXTURE_BLEND_MODE_NONE)
				currentMode = texture->mode;
			else
			{
				currentMode = inheritedMode;
				inheritedMode = TEXTURE_BLEND_MODE_NONE;
			}

			// skip texture
			if(texture->getTextureFormat() != TEXTURE_FORMAT_COLOR_MAP &&
			   texture->getTextureFormat() != TEXTURE_FORMAT_COLOR)
			{
				inheritedMode = currentMode;
				continue;
			}

			// режим мультитекстурирования
			switch(currentMode)
			{
				case TEXTURE_BLEND_MODE_MUL:	
						glEnable(GL_BLEND);
						glBlendFunc(GL_DST_COLOR, GL_ZERO);
						break;
				case TEXTURE_BLEND_MODE_ADD:
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE, GL_ONE);
						break;
//					case TEXTURE_BLEND_MODE_BLEND:	
//							glBlendFunc(GL_ONE, GL_ONE); break;

				case TEXTURE_BLEND_MODE_REPLACE: 
				default:
					glDisable(GL_BLEND);
					break;
			}

			if(texture->format == TEXTURE_FORMAT_COLOR_MAP) 
				if(BindTexture) (this->*BindTexture)(static_cast<const TextureColorMap*>(texture)->diffuse_map, true);

			if(texture->format == TEXTURE_FORMAT_COLOR) 
				glColor4fv(static_cast<const TextureColor*>(texture)->color.getfv());

			if(!lights.empty())
			{
				glEnable(GL_LIGHTING);
				for(unsigned int i = 0; i<e.lights.size() && i<GL_MAX_LIGHTS; i++)
				{
					glEnable(GL_LIGHT0 + i);
					float pos[4];
					glPushMatrix();
					glLoadIdentity();
					pos[0] = e.lights[i]->position.x;
					pos[1] = e.lights[i]->position.x;
					pos[2] = e.lights[i]->position.x;
					pos[3] = 1.0f;
					glLightfv(GL_LIGHT0 + i, GL_POSITION, (float*)pos);
					glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, e.lights[i]->light->color.getfv());
					glPopMatrix();
				}
			}

// -> (*).  ->*  *(->)
			if(DrawTriangles) (this->*DrawTriangles)(e, triangles, e.texCoords[i],  total);
			glPopClientAttrib();
			glPopAttrib();
		}
		glPopClientAttrib();
	   	glPopAttrib();
	}
}
*/
// нарисовать множество полигонов с указанным материалом
void OpenGL_Engine::DrawTriangles_OpenGL10(OpenGL_Engine::GraphShadow& e, const Faces& faces, const TexCoords* coords)
{
	if (e.vertexes && !faces.triangles.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.vertexCount += e.vertexes->size();
		total.triangleCount += faces.triangles.size();

		glBegin(GL_TRIANGLES);
		 
		for EACH_CONST(TriangleVector, faces.triangles, it)
		{
			if(coords != NULL) glTexCoord2fv(&coords->at( it->a[0] ).x);	glVertex3fv(&e.vertexes->at(it->a[0] ).x);
			if(coords != NULL) glTexCoord2fv(&coords->at( it->a[1] ).x);	glVertex3fv(&e.vertexes->at(it->a[1] ).x);
			if(coords != NULL) glTexCoord2fv(&coords->at( it->a[2] ).x);	glVertex3fv(&e.vertexes->at(it->a[2] ).x);
		}
	 
		glEnd();
	}
}

// установить текущуу текстуру
bool OpenGL_Engine::BindTexture_OpenGL10(Image& image, bool enable)
{
	if(enable) glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format;
	if(image.getFormat() == IMAGE_RGB) format = GL_RGB; 
	else if(image.getFormat() == IMAGE_RGBA) format = GL_RGBA;
	else
		assert(false, "Unsupported image format");

	int width = image.getWidth();
	int heigth = image.getHeight();
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

    glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image.getWidth(), image.getHeight(),0,
				format,  GL_UNSIGNED_BYTE , image.getBitmap());
	return true;
}

// нарисовать множество полигонов как сетку (только рёбра)
void OpenGL_Engine::DrawWire_OpenGL10(OpenGL_Engine::GraphShadow& e, const Faces& faces)
{
	if (e.vertexes != NULL && !faces.triangles.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.objectCount++;
		total.vertexCount += e.vertexes->size();
		total.triangleCount += faces.triangles.size();
         
        for EACH_CONST(TriangleVector, faces.triangles, it)
        {
	        glBegin(GL_LINE_LOOP);
			glVertex3fv(e.vertexes->at( it->a[0] ).getfv());
            glVertex3fv(e.vertexes->at( it->a[1] ).getfv());
            glVertex3fv(e.vertexes->at( it->a[2] ).getfv());
		    glEnd();
        }
    }
}

// нарисовать множество линий
void OpenGL_Engine::DrawLines_OpenGL10(OpenGL_Engine::GraphShadow &e)
{
	if(e.vertexes && e.lines && !e.lines->index.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.objectCount++;
		total.vertexCount += e.vertexes->size();
		total.triangleCount += e.lines->index.size();
		
		glBegin(GL_LINES);
        for(unsigned int i=0; i < e.lines->index.size(); i++)
        {
			// TODO color
            glVertex3fv(&e.vertexes->at( e.lines->index[i].a[0] ).x);
            glVertex3fv(&e.vertexes->at( e.lines->index[i].a[1] ).x);
        }
        glEnd();
    }
}

// нарисовать нормали к вершинам
void OpenGL_Engine::DrawNormals_OpenGL10(OpenGL_Engine::GraphShadow &e)
{
	if(e.normals != NULL && e.vertexes != NULL && e.vertexes->size() == e.normals->size())
	{
		glColor3f(0,0,1);
		AABB &f = e.aabb;
		float diag = (f.max-f.min).getLength()*0.05f;
		if(diag > 1) diag = 1.0f;		
		if(diag<EPSILON) diag = 0.01f;

		glBegin(GL_LINES);
		for(unsigned int i=0; i < e.vertexes->size(); i++)
		{
			v3 s = e.vertexes->at(i);
			v3 d = e.vertexes->at(i) + e.normals->at(i)*diag;

			glVertex3f(s.x, s.y, s.z);
			glVertex3f(d.x, d.y, d.z);
		}
		glEnd();
		glColor3f(1,1,1);
	}
}

// нарисовать вершины
void OpenGL_Engine::DrawVertexes_OpenGL10(OpenGL_Engine::GraphShadow &e)
{
	if(e.vertexes && !e.vertexes->empty())
	{
		glPointSize(5.0f);
		glColor3f(0.5f, 1.0f, 1.0f);

		glBegin(GL_POINTS);
		for(unsigned int i=0; i < e.vertexes->size(); i++)
		{
			glVertex3fv(&e.vertexes->at(i).x);
		}
		glEnd();

		glPointSize(1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

// нарисовать AABB
void OpenGL_Engine::DrawAABB_OpenGL10(OpenGL_Engine::GraphShadow &e)
{
	AABB &c = e.aabb;
	if(c.empty()) return;

	glColor3f(1.0f, 0.8f, 0.8f);

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);

	glVertex3f(c.min.x, c.min.y, c.min.z);	glVertex3f(c.max.x, c.min.y, c.min.z);
	glVertex3f(c.min.x, c.min.y, c.min.z);	glVertex3f(c.min.x, c.max.y, c.min.z);
	glVertex3f(c.min.x, c.min.y, c.min.z);	glVertex3f(c.min.x, c.min.y, c.max.z);

	glVertex3f(c.min.x, c.max.y, c.max.z);	glVertex3f(c.max.x, c.max.y, c.max.z);
	glVertex3f(c.max.x, c.min.y, c.max.z);	glVertex3f(c.max.x, c.max.y, c.max.z);
	glVertex3f(c.max.x, c.max.y, c.min.z);	glVertex3f(c.max.x, c.max.y, c.max.z);

	glVertex3f(c.min.x, c.min.y, c.max.z);	glVertex3f(c.max.x, c.min.y, c.max.z);
	glVertex3f(c.min.x, c.max.y, c.min.z);	glVertex3f(c.max.x, c.max.y, c.min.z);

	glVertex3f(c.min.x, c.min.y, c.max.z);	glVertex3f(c.min.x, c.max.y, c.max.z);
	glVertex3f(c.max.x, c.min.y, c.min.z);	glVertex3f(c.max.x, c.max.y, c.min.z);

	glVertex3f(c.min.x, c.max.y, c.min.z);	glVertex3f(c.min.x, c.max.y, c.max.z);	
	glVertex3f(c.max.x, c.min.y, c.min.z);	glVertex3f(c.max.x, c.min.y, c.max.z);	

	glEnd();

	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
}


