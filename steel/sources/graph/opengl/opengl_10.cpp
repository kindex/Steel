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
#include "ext/libext.h"
#include "../../common/logger.h"
#include "../material.h"
#include "../../common/utils.h"

namespace opengl
{

bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL10(GraphShadow& e, const Faces& triangles, MaterialStd& material)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	const TexCoords* texCoords = NULL;
	if (material.diffuse_map.image != NULL)
	{
		(this->*BindTexture)(*material.diffuse_map.image, true);
		texCoords = e.getTexCoords(material.diffuse_map);
	}
	else if (material.emission_map.image != NULL)
	{
		(this->*BindTexture)(*material.emission_map.image, true);
		texCoords = e.getTexCoords(material.emission_map);
	}
	else
	{
		glColor4fv(material.color.getfv());
	}

	if (texCoords != NULL)
	{
		glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
		glScalef(		material.textureMatrix.texCoordsScale.x,		material.textureMatrix.texCoordsScale.y,		material.textureMatrix.texCoordsScale.z); 
		glTranslatef(	material.textureMatrix.texCoordsTranslation.x,	material.textureMatrix.texCoordsTranslation.y,	material.textureMatrix.texCoordsTranslation.z); 
		glRotatef(		material.textureMatrix.texCoordsRotation/(float)(M_PI)*180.0f, 0.0f, 0.0f, 1.0f); 
	}

	(this->*DrawTriangles)(e, triangles, texCoords);
	if (CleanupDrawTriangles != NULL) (this->*CleanupDrawTriangles)();

	if (texCoords != NULL)
	{
		glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
	}

	glPopClientAttrib();
   	glPopAttrib();

	return true;
}


// нарисовать множество полигонов с указанным материалом
void OpenGL_Engine::DrawTriangles_OpenGL10(GraphShadow& e, const Faces& faces, const TexCoords* coords)
{
	if (e.vertexes && !(faces.triangles.empty() && faces.quads.empty()) && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.vertexCount += e.vertexes->size();
		total.faceCount += faces.triangles.size() + faces.quads.size();

		if (!faces.triangles.empty())
		{
			total.batchCount++;
			glBegin(GL_TRIANGLES);
			for EACH_CONST(TriangleVector, faces.triangles, it)
			{
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[0]).getfv());	glVertex3fv(e.vertexes->at(it->a[0]).getfv());
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[1]).getfv());	glVertex3fv(e.vertexes->at(it->a[1]).getfv());
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[2]).getfv());	glVertex3fv(e.vertexes->at(it->a[2]).getfv());
			}
			glEnd();
		}

		if (!faces.quads.empty())
		{
			total.batchCount++;
			glBegin(GL_QUADS);
			for EACH_CONST(QuadVector, faces.quads, it)
			{
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[0]).getfv());	glVertex3fv(e.vertexes->at(it->a[0]).getfv());
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[1]).getfv());	glVertex3fv(e.vertexes->at(it->a[1]).getfv());
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[2]).getfv());	glVertex3fv(e.vertexes->at(it->a[2]).getfv());
				if (coords != NULL) glTexCoord2fv(coords->at(it->a[3]).getfv());	glVertex3fv(e.vertexes->at(it->a[3]).getfv());
			}
			glEnd();
		}
	}
}

// установить текущуу текстуру
bool OpenGL_Engine::BindTexture_OpenGL10(Image& image, bool enable)
{
	if (enable)
    {
        glEnable(GL_TEXTURE_2D);
    }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format;
	if (image.getFormat() == IMAGE_RGB)
	{
		format = GL_RGB; 
	}
	else if(image.getFormat() == IMAGE_RGBA)
	{
		format = GL_RGBA;
	}
	else 
	{
		assert(false, "Unsupported image format");
	}

	size_t width = image.getWidth();
	size_t heigth = image.getHeight();
	if (!GL_EXTENSION_TEXTURE_NON2POWER)
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
void OpenGL_Engine::DrawWire_OpenGL10(GraphShadow& e, const Faces& faces)
{
	if (e.vertexes != NULL && !(faces.triangles.empty()&& faces.quads.empty()) && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.vertexCount += e.vertexes->size();
		total.faceCount += faces.triangles.size();
         
        for EACH_CONST(TriangleVector, faces.triangles, it)
        {
			total.batchCount++;
	        glBegin(GL_LINE_LOOP);
			    glVertex3fv(e.vertexes->at( it->a[0] ).getfv());
                glVertex3fv(e.vertexes->at( it->a[1] ).getfv());
                glVertex3fv(e.vertexes->at( it->a[2] ).getfv());
		    glEnd();
        }
        for EACH_CONST(QuadVector, faces.quads, it)
        {
			total.batchCount++;
	        glBegin(GL_LINE_LOOP);
			    glVertex3fv(e.vertexes->at( it->a[0] ).getfv());
                glVertex3fv(e.vertexes->at( it->a[1] ).getfv());
                glVertex3fv(e.vertexes->at( it->a[2] ).getfv());
                glVertex3fv(e.vertexes->at( it->a[3] ).getfv());
		    glEnd();
        }
    }
}

// нарисовать множество линий
void OpenGL_Engine::DrawLines_OpenGL10(GraphShadow &e)
{
	if(e.vertexes && e.lines && !e.lines->index.empty() && !e.vertexes->empty())// если есть полигоны и вершины
	{
		total.batchCount++;
		total.vertexCount += e.vertexes->size();
		total.faceCount += e.lines->index.size();
		
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
void OpenGL_Engine::DrawNormals_OpenGL10(GraphShadow &e)
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
void OpenGL_Engine::DrawVertexes_OpenGL10(GraphShadow &e)
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
void OpenGL_Engine::DrawAABB_OpenGL10(GraphShadow &e)
{
	AABB &c = e.aabb;
	if(c.empty()) return;

	glColor3f(1.0f, 0.8f, 0.8f);

    glMatrixMode(GL_MODELVIEW);
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

void OpenGL_Engine::DrawText_OpenGL10(ObjectPosition& parent_position, const GraphTextVector& e)
{
	glMatrixMode(GL_TEXTURE);// TEMP:
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    for EACH_CONST(GraphTextVector, e, text_it)
    {
        const GraphText& text = *text_it;
        if (text.position_kind == POSITION_SCREEN)
        {
            pushPosition(ObjectPosition::getIdentity(), text.position_kind);
        }

        v2 points[] = {
            v2(0, 1),
            v2(1, 1),
            v2(1, 0),
            v2(0, 0),
        };

        v2 tex_points[] = {
            v2(0, 1), 
            v2(1, 1),
            v2(1, 0),
            v2(0, 0)
        };

        v3 real_point[4];
        v3 normal;

        float text_width = float(text.font->getWidth(text.string))/text.font->getSize();
        
        v2 text_size(
            text.size.y,
            text.size.x);

        v3 dx;
        v3 dy;
        calculateSprite(info.camera,
                        (parent_position*text.position).getTranslation(),
                        text.position_kind,
                        text_size,
                        text.align,
                        zero,
                        real_point[0],
                        real_point[1],
                        real_point[2],
                        real_point[3],
                        normal,
                        dx,
                        dy);

        dx *= text_width;

        v3 origin = (real_point[0] + real_point[1] + real_point[2] + real_point[3])/4;
        if (text.text_align == GraphText::ALIGN_CENTER)
        {
            origin -= dx*0.5;
            origin -= dy*0.5;
        }
        else if (text.text_align == GraphText::ALIGN_LEFT_TOP)
        {
            origin -= dy;
        }

        Image& texture = text.font->getTexture();
	    glPushAttrib(GL_ALL_ATTRIB_BITS);
        (this->*BindTexture)(texture, true);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        color4f color = text.color;
        color.a = 1.0f;
        glColor4fv(color.getfv());
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_QUADS);
        float sx = 0;
        float sy = 0;
        float font_size = float(text.font->getSize());
        for (size_t i = 0; i < text.string.length(); i++)
        {
            const Font::Char& c = text.font->getChar(text.string[i]);

            for (int j = 0; j < 4; j++)
            {
                glTexCoord2f(float(c.x + tex_points[j].x*c.width)/texture.getWidth(),
                             -float(c.y + tex_points[j].y*c.height)/texture.getHeight());

                float x = (points[j].x*c.width + c.xoffset)/font_size/text_width;
                float y = 1 - (points[j].y*c.height + c.yoffset)/font_size;
                v3 p = origin
                    + dx*(sx + x)
                    + dy*(sy + y);

                glVertex3fv(p.getfv());
            }
            sx += c.xadvance/font_size/text_width;
        }
        glEnd();
        glPopAttrib();
		total.batchCount++;
		total.vertexCount += text.string.length()*4;
		total.faceCount += text.string.length();

        if (text.position_kind == POSITION_SCREEN)
        {
            popPosition(text.position_kind);
        }
    }
}

} // namespace opengl

