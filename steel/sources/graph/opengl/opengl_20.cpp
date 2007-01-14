/*id*********************************************************
	File: graph/opengl/opengl_20.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 2.0

		Shader Objects
 ************************************************************/

#include "../../steel.h"
#include "opengl_glsl.h"
#include "opengl_engine.h"
#include "../../libs/opengl/libext.h"
#include <string>

// нарисовать множество полигонов с указанным материалом / Multitexture
bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL20(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, MaterialStd *material)
{
	if(material != NULL && GL_EXTENSION_GLSL)
	{
		total.objectCount++;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		
		GLSL *program = BindShader(&shaderStd);

		if(program != NULL)
		{
			(this->*BindTexCoords)(e.getTexCoords(material->diffuse_map), &material->diffuse_map.textureMatrix);

			bindTextureToShader(program, "diffuse_map", 0, material->diffuse_map.image ? material->diffuse_map.image : none);
//			bindTextureToShader(program, "diffuse2_map", 1, material->diffuse2_map.image ? material->diffuse2_map.image : black);
			bindTextureToShader(program, "normal_map", 2, material->normal_map.image ? material->normal_map.image : zeroNormal);
			bindTextureToShader(program, "emission_map", 3, material->emission_map.image ? material->emission_map.image : black);
			bindTextureToShader(program, "specular_map", 4, material->specular_map.image ? material->specular_map.image : white);

			program->setUniformFloat("material.specularPower", material->specularPower);
			program->setUniformFloat("material.speculark", material->specular_map.k);
			program->setUniformFloat("material.diffusek", material->diffuse_map.k);
			program->setUniformFloat("material.emissionk", material->emission_map.k);

			program->setUniformVector("camera.position", camera.getPosition());
			program->setUniformVector("camera.direction", camera.getDirection());
			program->setUniformVector("camera.upVector", camera.getUpVector());

			int lightCount = (int)e.lights.size();
			if (lightCount > maxLightsInShader) lightCount = maxLightsInShader;

			program->setUniformInt("lightCount", lightCount);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			for(int i = 0; i < lightCount; i++)
			{
				float pos[4];
				pos[0] = e.lights[i]->position.x;
				pos[1] = e.lights[i]->position.y;
				pos[2] = e.lights[i]->position.z;
				pos[3] = 0.0f;
				glLightfv(GL_LIGHT0 + i, GL_POSITION, (float*)pos);
				glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, e.lights[i]->light->constantAttenuation);
				glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, e.lights[i]->light->linearAttenuation);
				glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, e.lights[i]->light->quadraticAttenuation);

				glLightfv(GL_LIGHT0 + i, GL_AMBIENT, e.lights[i]->light->ambient.getfv());
				glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, e.lights[i]->light->diffuse.getfv());
				glLightfv(GL_LIGHT0 + i, GL_SPECULAR, e.lights[i]->light->specular.getfv());

				std::string lighti = std::string("lights[") + IntToStr(i) + "]";
				program->setUniformFloat(lighti + ".k", e.lights[i]->light->k);
				program->setUniformFloat(lighti + ".minDistance", e.lights[i]->light->minDistance);
				program->setUniformFloat(lighti + ".maxDistance", e.lights[i]->light->maxDistance);
				program->setUniformFloat(lighti + ".sqrtAttenuation", e.lights[i]->light->sqrtAttenuation);
			}
			glPopMatrix();

			TexCoords3f *sTangent = NULL;
			TexCoords3f *tTangent = NULL;
			getTangentSpace(e.vertexes, e.getTexCoords(material->normal_map), e.faceMaterials, e.normals, &sTangent, &tTangent);

			if(sTangent != NULL)	
			{ 
				glActiveTextureARB(GL_TEXTURE0_ARB + 7);
				glClientActiveTextureARB(GL_TEXTURE0_ARB + 7);
				
				(this->*BindTexCoords3f)(sTangent);
			}

			if(DrawTriangles) (this->*DrawTriangles)(e, triangles, NULL);

			program->unbind();
		}

		if(CleanupDrawTriangles != NULL) (this->*CleanupDrawTriangles)();

		unbindTexCoords();

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

