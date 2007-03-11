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

// нарисовать множество полигонов с указанным материалом / Multitexture
bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material)
{
	if (flags.glsl)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		
		if (flags.useDebugShader)
		{
			(this->*BindTexCoords)(e.getTexCoords(material.diffuse_map), &material.textureMatrix);
			SetupDebugShader_OpenGL20(e, faces, material, *program);
		}
		if (flags.textures) // use_std_shader
		{
			StringDict vars;
			vars["lighting"] = IntToStr(flags.lighting);
			vars["lightcount"] = IntToStr(StdShaderGetLightCount(e, faces, material));
			vars["reflecting"] = IntToStr(material.env_map.cubeMap != NULL ? 1 : 0);
			vars["blending"] = IntToStr(flags.blending && flags.current.transparent && material.blend);
			program = bindShader(flags.shaderStd, vars);
			if (program == NULL)
			{
				return false; // shader compile error
			}
			(this->*BindTexCoords)(e.getTexCoords(material.diffuse_map), &material.textureMatrix);
			SetupStdShader_OpenGL20(e, faces, material, *program);
		}

		(this->*DrawTriangles)(e, faces, NULL);
		if (CleanupDrawTriangles != NULL)
		{
			(this->*CleanupDrawTriangles)();
		}

		unbindTexCoords();
		program->unbind();
		glPopClientAttrib();
	   	glPopAttrib();
		return true;
	}
	else
	{
		return DrawFill_MaterialStd_OpenGL13(e, faces, material);
	}
}

size_t OpenGL_Engine::StdShaderGetLightCount(GraphShadow& e, const Faces& faces, MaterialStd& material)
{
	if (flags.lighting)
	{
		size_t result = 0;
		for EACH(LightShadowPVector, e.lights, it)
		{
			LightShadow& lightShadow = **it;
			if (lightShadow.needToBeRendered(flags))
			{
				result++;
			}
			if (result >= flags.maxLightsInShader)
			{
				return result;
			}
		}
		return result;
	}
	else
	{
		return 0;
	}
}


void OpenGL_Engine::SetupStdShader_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material, Shader& shader)
{
	shader.clearTextures();
	shader.bindTexture("emission_map", material.emission_map.image ? material.emission_map.image : black);

	shader.setUniformFloat("material.specularPower", material.specularPower);
	shader.setUniformFloat("material.speculark", material.specular_map.k);
	shader.setUniformFloat("material.diffusek", material.diffuse_map.k);
	shader.setUniformFloat("material.emissionk", material.emission_map.k);
	shader.setUniformFloat("material.ambientk", material.ambient_k);

	shader.setUniformVector("camera.direction", info.camera.getDirection());
	shader.setUniformVector("camera.position", info.camera.getPosition());
	shader.setUniformVector("camera.upVector", info.camera.getUpVector());

	if (flags.lighting)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		bool cubeMapSet = false;
		size_t lightIndex = 0;
		for EACH(LightShadowPVector, e.lights, it)
		{
			LightShadow& lightShadow = **it;
			Light& light = *lightShadow.light;
			if (!lightShadow.needToBeRendered(flags))
			{
				continue;
			}

			std::string lighti = std::string("lights[") + IntToStr(lightIndex) + "].";
			shader.setUniformVector(lighti + "position", lightShadow.position);
			shader.setUniformFloat(lighti + "constantAttenuation", light.constantAttenuation);
			shader.setUniformFloat(lighti + "linearAttenuation", light.linearAttenuation);
			shader.setUniformFloat(lighti + "quadraticAttenuation", light.quadraticAttenuation);

			shader.setUniformVector(lighti + "ambient", light.ambient);
			shader.setUniformVector(lighti + "diffuse", light.diffuse);
			shader.setUniformVector(lighti + "specular", light.specular);

			shader.setUniformFloat(lighti + "k", light.k);
			shader.setUniformFloat(lighti + "minDistance", light.minDistance);
			shader.setUniformFloat(lighti + "maxDistance", light.maxDistance);
			shader.setUniformVector(lighti + "up", light.up);
			shader.setUniformVector(lighti + "direction", light.direction);

			if (light.cubeMap != NULL && !cubeMapSet)
			{
				shader.setUniformInt(lighti + "type", 1);
				shader.bindTexture("light_cube_map", light.cubeMap);
				cubeMapSet = true;
			}
			else
			{
				shader.setUniformInt(lighti + "type", 0);
			}
			lightIndex++;
			if (lightIndex >= flags.maxLightsInShader)
			{
				break;
			}
		}
		size_t lightCount = lightIndex;
		if (!cubeMapSet && lightCount >= 1)
		{
			shader.bindTexture("light_cube_map", none);
		}

		if (flags.lighting && lightCount >= 1)
		{
			shader.bindTexture("diffuse_map",  material.diffuse_map.image  ? material.diffuse_map.image  : none);
			shader.bindTexture("specular_map", material.specular_map.image ? material.specular_map.image : white); // TODO: env map koef
		}
		if (flags.lighting && lightCount >= 1 || material.env_map.cubeMap != NULL)
		{
			shader.bindTexture("normal_map",   material.normal_map.image   ? material.normal_map.image   : zeroNormal);
		}
		if (material.env_map.cubeMap != NULL)
		{
			shader.bindTexture("env_map", material.env_map.cubeMap);
			shader.setUniformFloat("env_k", material.env_map.k);
			shader.setUniformInt("env_type", material.env_map.type);
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

void OpenGL_Engine::SetupDebugShader_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material, Shader& shader)
{
	shader.clearTextures();
	shader.bindTexture("normal_map",   material.normal_map.image ? material.normal_map.image : zeroNormal);

	shader.setUniformVector("camera.position", info.camera.getPosition());
	shader.setUniformVector("camera.direction", info.camera.getDirection());
	shader.setUniformVector("camera.upVector", info.camera.getUpVector());

	shader.setUniformInt("mode", flags.debugShaderMode);

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



Shader* OpenGL_Engine::loadShader(const std::string& path, const StringDict& parameters)
{
	Shader* shader = new Shader(*this);
	ShaderKey key(path, parameters);

	if (shader->init(resText.add(path + ".vert"), resText.add(path + ".frag"), parameters))
	{
		shaders.insert(make_pair(key, shader));
		return shader;
	}
	else
	{
		delete shader;
		shaders.insert(make_pair(key, static_cast<Shader*>(NULL)));
		return NULL;
	}
}

Shader* OpenGL_Engine::bindShader(const std::string& path, const StringDict& parameters)
{
	ShaderKey key(path, parameters);
	ShaderDict::iterator it = shaders.find(key);

	if (it == shaders.end())
	{
		loadShader(path, parameters);
		it = shaders.find(key);
	}

	if (it->second == NULL)
	{
		return NULL;
	}
	else
	{
		it->second->bind();
		return it->second;
	}
	
	return false;
}

} // namespace opengl
