/*id*********************************************************
	File: graph/opengl/opengl_20.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 2.0

		Shader Objects
 ************************************************************/

#include "../../steel.h"
#include "opengl_glsl.h"
#include "opengl_engine.h"
#include "gl/libext.h"

// нарисовать множество полигонов с указанным материалом / Multitexture
void OpenGL_Engine::DrawFill_OpenGL20(OpenGL_Engine::GraphShadow &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total)
{
	if(material != NULL && GL_EXTENSION_GLSL)
	{
		if(!material->isShader())
		{
			OpenGL_Engine::DrawFill_OpenGL13(e, triangles, material, total);
			return;
		}

		total.object++;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		GLSL *program = BindShader(material);

		if(program)
		{
			int texCount = material->getTextureCount();

			for(int i=0; i<texCount; i++)
			{
				const Texture &texture = *material->getTexture(i); // текущая текстура

				glActiveTextureARB(GL_TEXTURE0_ARB + i);
				glClientActiveTextureARB(GL_TEXTURE0_ARB + i);

				if(texture.format == TEXTURE_FORMAT_COLOR_MAP)
				{
					TexCoords *coords = e.texCoords[i];

					(this->*BindTexture)(texture.image, false);

					std::string name2("image" + IntToStr(i));
					const char *name = name2.c_str();
					if(!program->setTexture(name, i))
					{
						error("glsl error", std::string("Cannot find texture '") + name + "' in shader");
					}

					(this->*BindTexCoords)(coords);
				}
				else
				{
					if(BindTexCoords) (this->*BindTexCoords)(NULL);
				}
			}

			program->setUniformVector("camera_eye", camera.eye);
			program->setUniformVector("camera_dir", (camera.center - camera.eye).getNormalized());

			for(unsigned int i = 0; i < lights.size(); i++)
			{
				std::string pos("light[" + IntToStr(i)+"].position");
				program->setUniformVector(pos.c_str(), lights[i].position);
			}


			if(DrawTriangles) (this->*DrawTriangles)(e, triangles, NULL, total);

			program->unbind();
		}

	   	glPopAttrib();
	}
}


GLSL *OpenGL_Engine::BindShader(Material *material)
{
	if(!material->isShader()) return NULL;
// TODO:
/*	uid id = material->getId(); 

	OpenGL_Buffer &buf = buffer[id];

	if(!buf.loaded)
	{
		GLSL *shader = new GLSL;
		if(shader->init(material))
		{
			buf.loaded = true;
			buf.glid = shader->getGL_Id();
			buf.object = (char*)shader;
		}
	}

	if(buf.loaded)
	{
		// setup variables
		glUseProgramObjectARB(buf.glid);
		return (GLSL*)buf.object;
	}
	*/
	return NULL;
}

