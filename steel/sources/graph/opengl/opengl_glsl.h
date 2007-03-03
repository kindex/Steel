/*id*********************************************************
	File: graph/opengl/opengl_glsl.h
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		GLSL шейдёры
 ************************************************************/

#ifndef OPENGL_GLSL_H
#define OPENGL_GLSL_H

#include "../../steel.h"
#include "../graph_engine.h"
#include "../../libs/opengl/libext.h"
#include "../../res/text/text.h"
#include "../../common/containers.h"

namespace opengl
{

class OpenGL_Engine;

struct Shader
{
	Shader(OpenGL_Engine& engine);
	~Shader();

	uid	 getId(){ return id; }

	GLuint getProgramId() { return programId; }
	bool loadShader(GLuint shader, Text* text, const StringDict& parameters);
	std::string getShaderCode(Text* text, const StringDict& parameters);

	bool init(Text*	_vertexShader, Text* _fragmentShader, const StringDict& parameters);
	bool isError();
	void loadLog(GLuint object);
	void bind();
	void unbind();

	GLuint getGL_Id() { return programId;}

	void clearTextures();
	void bindTexture(const std::string& name, Image* image);
    bool setUniformFloat	(const std::string& name,	const float);
	bool setUniformVector	(const std::string& name,	const v3&);
	bool setUniformInt		(const std::string& name,	const int);
	std::string getShaderDecription() const;

	GLint findVariable(const std::string& name);

private:
	OpenGL_Engine&	engine;
	uid		id;
	Text*	vertexShader;
	Text*	fragmentShader;
	bool	failed;
	GLuint	programId;
	GLuint	vertexShaderId;
	GLuint	fragmentShaderId;
	GLuint	glid;
	StringDict	parameters;

	int		textureIndex;

	typedef std::map<std::string, GLint> VariableLocationCache;
	VariableLocationCache variableLocationCache;
};

struct ShaderKey
{
	ShaderKey(std::string, StringDict parameters) : id(id), parameters(parameters) {}

	const	std::string	id;
	const	StringDict	parameters;
	bool operator < (const ShaderKey& second) const
	{
		return id < second.id || id == second.id && parameters < second.parameters;
	}
};


typedef std::map<ShaderKey, Shader*> ShaderDict;

} // namespace opengl

#endif
