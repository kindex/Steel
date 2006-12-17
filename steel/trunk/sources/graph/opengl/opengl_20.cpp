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
			(this->*BindTexCoords)(e.getTexCoords(material->diffuse_map));

			bindTextureToShader(program, "diffuse_map", 0, material->diffuse_map.image);
			bindTextureToShader(program, "diffuse2_map", 1, material->diffuse2_map.image);
			bindTextureToShader(program, "normal_map", 2, material->normal_map.image);
			bindTextureToShader(program, "emission_map", 3, material->emission_map.image);
			bindTextureToShader(program, "specular_map", 4, material->specular_map.image);

			program->setUniformFloat("material.specularPower", material->specularPower);
			program->setUniformFloat("material.speculark", material->speculark);
			program->setUniformFloat("material.diffusek", material->diffusek);
			program->setUniformFloat("material.emissionk", material->emissionk);

			program->setUniformVector("camera.position", camera.getPosition());
			program->setUniformVector("camera.direction", camera.getDirection());
			program->setUniformVector("camera.upVector", camera.getUpVector());

			int lightCount = (int)e.lights.size();
			if (lightCount > 4) lightCount = 4;

			program->setUniformInt("lightCount", lightCount);
			for(int i = 0; i < lightCount; i++)
			{
				glLightfv(GL_LIGHT0 + i, GL_POSITION, e.lights[i]->position.getfv());
				glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, e.lights[i]->light->constantAttenuation);
				glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, e.lights[i]->light->linearAttenuation);
				glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, e.lights[i]->light->quadraticAttenuation);

//				glLightfv(GL_LIGHT0 + i, GL_AMBIENT, e.lights[i]->light->ambient.getfv());
				glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, e.lights[i]->light->diffuse.getfv());
				glLightfv(GL_LIGHT0 + i, GL_SPECULAR, e.lights[i]->light->specular.getfv());

//				glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, e.lights[i]->light->maxDistance);
				float spot[4];
				spot[0] = e.lights[i]->light->minDistance;
				spot[1] = e.lights[i]->light->maxDistance;
				spot[2] = e.lights[i]->light->k;
				glLightfv(GL_LIGHT0 + i, GL_AMBIENT, spot);
			}


			TexCoords3f *sTangent, *tTangent;
			getTangentSpace(e.vertexes, e.getTexCoords(material->normal_map), e.faceMaterials, e.normals, &sTangent, &tTangent);

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

