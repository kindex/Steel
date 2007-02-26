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
#include <string>
#include "../../libs/opengl/libext.h"
#include "opengl_engine.h"

namespace opengl
{

void OpenGL_Engine::DrawFill_SetupStdShader_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material, GLSL& program)
{
	bindTextureToShader(program, "diffuse_map",  0, material.diffuse_map.image  ? material.diffuse_map.image  : none);
	bindTextureToShader(program, "emission_map", 2, material.emission_map.image ? material.emission_map.image : black);
	bindTextureToShader(program, "normal_map",   1, material.normal_map.image   ? material.normal_map.image   : zeroNormal);

	if (flags.lighting)
	{
		bindTextureToShader(program, "specular_map", 3, material.specular_map.image ? material.specular_map.image : white); // TODO: env map koef
	}
	if (material.env_map.cubeMap != NULL)
	{
		bindTextureToShader(program, "env_map", 4, material.env_map.cubeMap);
		program.setUniformFloat("env_k", material.env_map.k);
	}
	else
	{
		bindTextureToShader(program, "env_map", 4, none);
		program.setUniformFloat("env_k", 0.0f);
	}

	program.setUniformFloat("material.specularPower", material.specularPower);
	program.setUniformFloat("material.speculark", material.specular_map.k);
	program.setUniformFloat("material.diffusek", material.diffuse_map.k);
	program.setUniformFloat("material.emissionk", material.emission_map.k);

	program.setUniformVector("camera.position", info.camera.getPosition());
	program.setUniformVector("camera.direction", info.camera.getDirection());
	program.setUniformVector("camera.upVector", info.camera.getUpVector());

	program.setUniformInt("blending", material.blend);

	if (flags.lighting)
	{
		int lightCount = (int)e.lights.size();
		if (lightCount > maxLightsInShader) lightCount = maxLightsInShader;

		program.setUniformInt("lightCount", lightCount);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		bool cubeMapSet = false;
		for(int i = 0; i < lightCount; i++)
		{
			float pos[4];
			pos[0] = e.lights[i]->position.x;
			pos[1] = e.lights[i]->position.y;
			pos[2] = e.lights[i]->position.z;
			pos[3] = 0.0f;
			glLightfv(GL_LIGHT0 + i,GL_POSITION, (float*)pos);
			glLightf(GL_LIGHT0 + i,	GL_CONSTANT_ATTENUATION,	e.lights[i]->light->constantAttenuation);
			glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION,		e.lights[i]->light->linearAttenuation);
			glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION,	e.lights[i]->light->quadraticAttenuation);

			glLightfv(GL_LIGHT0 + i, GL_AMBIENT,	e.lights[i]->light->ambient.getfv());
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE,	e.lights[i]->light->diffuse.getfv());
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR,	e.lights[i]->light->specular.getfv());

			std::string lighti = std::string("lights[") + IntToStr(i) + "]";
			program.setUniformFloat(lighti + ".k", e.lights[i]->light->k);
			program.setUniformFloat(lighti + ".minDistance", e.lights[i]->light->minDistance);
			program.setUniformFloat(lighti + ".maxDistance", e.lights[i]->light->maxDistance);
			program.setUniformFloat(lighti + ".sqrtAttenuation", e.lights[i]->light->sqrtAttenuation);
			program.setUniformVector(lighti + ".up", e.lights[i]->light->up);
			program.setUniformVector(lighti + ".direction", e.lights[i]->light->direction);

			if (e.lights[i]->light->cubeMap != NULL && !cubeMapSet)
			{
				program.setUniformInt(lighti + ".type", 1);
				bindTextureToShader(program, "light_cube_map", 5, e.lights[i]->light->cubeMap);
				cubeMapSet = true;
			}
			else
			{
				program.setUniformInt(lighti + ".type", 0);
			}
		}
		if (!cubeMapSet)
		{
			bindTextureToShader(program, "light_cube_map", 5, none);
		}
		glPopMatrix();
	}

	if (flags.lighting)
	{
		TexCoords3f* sTangent = NULL;
		TexCoords3f* tTangent = NULL;
		getTangentSpace(e.vertexes, e.getTexCoords(material.normal_map), e.faceMaterials, e.normals, &sTangent, &tTangent);

		if (sTangent != NULL)	
		{ 
			glActiveTextureARB(GL_TEXTURE0_ARB + 7);
			glClientActiveTextureARB(GL_TEXTURE0_ARB + 7);
			
			(this->*BindTexCoords3f)(sTangent);
		}
	}
}

void OpenGL_Engine::DrawFill_SetupDebugShader_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material, GLSL& program)
{
	bindTextureToShader(program, "normal_map",   0, material.normal_map.image   ? material.normal_map.image   : zeroNormal);

	program.setUniformVector("camera.position", info.camera.getPosition());
	program.setUniformVector("camera.direction", info.camera.getDirection());
	program.setUniformVector("camera.upVector", info.camera.getUpVector());

	program.setUniformInt("mode", flags.debugShaderMode);

	TexCoords3f* sTangent = NULL;
	TexCoords3f* tTangent = NULL;
	getTangentSpace(e.vertexes, e.getTexCoords(material.normal_map), e.faceMaterials, e.normals, &sTangent, &tTangent);

	if (sTangent != NULL)	
	{ 
		glActiveTextureARB(GL_TEXTURE0_ARB + 7);
		glClientActiveTextureARB(GL_TEXTURE0_ARB + 7);
		
		(this->*BindTexCoords3f)(sTangent);
	}
}

// нарисовать множество полигонов с указанным материалом / Multitexture
bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material)
{
	if (GL_EXTENSION_GLSL)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		
		GLSL* program = NULL;
		if (!flags.textures)
		{
			program = BindShader(&shaderNoTexture, StringDict());
		}
		else if (flags.useDebugShader)
		{
			program = BindShader(&shaderDebug, StringDict());
			if (program != NULL)
			{
				(this->*BindTexCoords)(e.getTexCoords(material.diffuse_map), &material.textureMatrix);
				DrawFill_SetupDebugShader_OpenGL20(e, faces, material, *program);
			}
		}
		else // use_std_shader
		{
			StringDict parameters;
			parameters["lighting"] = IntToStr(flags.lighting);
			parameters["lighcount"] = "4";
			program = BindShader(&shaderStd, parameters);
			if (program != NULL)
			{
				(this->*BindTexCoords)(e.getTexCoords(material.diffuse_map), &material.textureMatrix);
				DrawFill_SetupStdShader_OpenGL20(e, faces, material, *program);
			} 
		}

		if (program != NULL)
		{
			(this->*DrawTriangles)(e, faces, NULL);

			program->unbind();
			if (CleanupDrawTriangles != NULL)
			{
				(this->*CleanupDrawTriangles)();
			}

			unbindTexCoords();
		}
		else // shader is not loaded
		{
			DrawFill_MaterialStd_OpenGL13(e, faces, material);
		}

		glPopClientAttrib();
	   	glPopAttrib();
		return true;
	}
	return false;
}

void OpenGL_Engine::bindTextureToShader(GLSL& program, const std::string& name, int imageNum, Image* image)
{
	if (image != NULL)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + imageNum);
		glClientActiveTextureARB(GL_TEXTURE0_ARB + imageNum);

		(this->*BindTexture)(*image, false);

		if (!program.setTexture(name.c_str(), imageNum))
		{
			error("glsl error", std::string("Cannot find texture '") + name + "' in shader");
		}
	}
}

bool OpenGL_Engine::loadShader(Shader* shader, const StringDict& parameters)
{
	GLSL* GLSL_Shader = new GLSL;
	ShaderOpenGL_ObjectKey key(shader->id, parameters);
	shaders.insert(make_pair(key, ShaderOpenGL_Object()));
	ShaderDict::iterator it = shaders.find(key);

	if (GLSL_Shader->init(shader, parameters))
	{
//		it->second.loaded = true;
		it->second.glid = GLSL_Shader->getGL_Id();
		it->second.GLSL_Shader = GLSL_Shader;
		it->second.failed = false;
		return true;
	}
	else
	{
		it->second.failed = true;
		return false;
	}
}

GLSL* OpenGL_Engine::BindShader(Shader* shader, const StringDict& parameters)
{
	ShaderOpenGL_ObjectKey key(shader->id, parameters);
	ShaderDict::iterator it = shaders.find(key);

	if (it == shaders.end())
	{
		loadShader(shader, parameters);
		it = shaders.find(key);
	}

	if (it->second.failed)
	{
		return NULL;
	}
	else
	{
		glUseProgramObjectARB(it->second.glid);
		return it->second.GLSL_Shader;
	}
	
	return false;
}

} // namespace opengl
