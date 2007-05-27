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
#include "../../common/containers/string_dict.h"
class Text;

namespace opengl
{

class OpenGL_Engine;

class Shader
{
public:
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
	void clearCache();

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

	std::map<GLint, float> floatCache;
	std::map<GLint, int> intCache;
	std::map<GLint, v3> v3Cache;
	std::map<GLint, GLint> imageCache;
};

} // namespace opengl

#endif
