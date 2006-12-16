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
bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL20(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, MaterialStd *material, GraphEngine::GraphTotalInfo &total)
{
	if(material != NULL && GL_EXTENSION_GLSL)
	{
		total.objectCount++;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		
		GLSL *program = BindShader(&shaderStd);

		if(program != NULL)
		{
			(this->*BindTexCoords)(e.getTexCoords(material->color_map));

			bindTextureToShader(program, "color_map", 0, material->color_map.image);
			bindTextureToShader(program, "normal_map", 1, material->normal_map.image);

			program->setUniformVector("camera_eye", camera.eye);
			program->setUniformVector("camera_dir", (camera.center - camera.eye).getNormalized());

			for(unsigned int i = 0; i < e.lights.size(); i++)
			{
				std::string pos("light[" + IntToStr(i)+"].position");
				program->setUniformVector(pos.c_str(), e.lights[i]->position);
			}


			TexCoords3f *sTangent, *tTangent;
			getTangentSpace(e.vertexes, e.getTexCoords(material->normal_map), e.faceMaterials, e.normals, &sTangent, &tTangent);

			GLuint index = glGetAttribLocationARB(program->getProgramId(),"tangent");
			glUseProgramObjectARB(program->getProgramId()); 
			glBindAttribLocationARB(program->getProgramId(), index, "tangent");

			if(sTangent != NULL)	
			{ 
				glActiveTextureARB(GL_TEXTURE0_ARB + 7);
				glClientActiveTextureARB(GL_TEXTURE0_ARB + 7);
				
				(this->*BindTexCoords3f)(sTangent);
			}

			if(DrawTriangles) (this->*DrawTriangles)(e, triangles, NULL, total);

			program->unbind();
		}

		if(CleanupDrawTriangles != NULL) (this->*CleanupDrawTriangles)();

		glPopClientAttrib();
	   	glPopAttrib();
		return true;
	}
	return false;
}

void OpenGL_Engine::bindTextureToShader(GLSL *program, const char *name, int imageNum, Image *image)
{
	if(image != NULL)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + imageNum);
		glClientActiveTextureARB(GL_TEXTURE0_ARB + imageNum);

		(this->*BindTexture)(image, false);

		if(!program->setTexture(name, imageNum))
		{
			error("glsl error", std::string("Cannot find texture '") + name + "' in shader");
		}
	}
}


GLSL *OpenGL_Engine::BindShader(Shader *shader)
{
// TODO:
	uid id = shader->getId(); 

	OpenGL_Buffer &buf = buffer[id];

	if(!buf.loaded)
	{
		GLSL *GLSL_Shader = new GLSL;
		if(GLSL_Shader->init(shader))
		{
			buf.loaded = true;
			buf.glid = GLSL_Shader->getGL_Id();
			buf.object = (char*)GLSL_Shader;
		}
	}

	if(buf.loaded)
	{
		// setup variables
		glUseProgramObjectARB(buf.glid);
		return (GLSL*)buf.object;
	}
	
	return false;
}

