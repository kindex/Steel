/*id*********************************************************
	File: graph/opengl/opengl_glsl.cpp
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

#include "../../steel.h"
#include "opengl_glsl.h"
#include "../../res/res_main.h"
#include <gl/glu.h>
#include "opengl_engine.h"

namespace opengl
{

Shader::Shader(OpenGL_Engine& engine):
	engine(engine),
	vertexShader(NULL),
	fragmentShader(NULL),
	programId(0),
	vertexShaderId(0),
	fragmentShaderId(0),
	id(objectIdGenerator.genUid())
{}


bool Shader::init(Text*	_vertexShader, Text* _fragmentShader, const StringDict& _parameters)
{
	if (!GL_EXTENSION_GLSL) return false;

	vertexShader = _vertexShader;
	fragmentShader = _fragmentShader;
	parameters = _parameters;

	programId = glCreateProgramObjectARB();
	if (isError()) return false;

    vertexShaderId = glCreateShaderObjectARB (GL_VERTEX_SHADER_ARB);
   	if (!loadShader(vertexShaderId, vertexShader, parameters))
	{
		loadLog(programId);
		return false;
	}
    glAttachObjectARB(programId, vertexShaderId);

    fragmentShaderId = glCreateShaderObjectARB (GL_FRAGMENT_SHADER_ARB);
   	if (!loadShader(fragmentShaderId, fragmentShader, parameters))
	{
		loadLog(programId);
		return false;
	}
    glAttachObjectARB(programId, fragmentShaderId);


    GLint   linked;
    glLinkProgramARB(programId);

	if (isError())
	{
		return false;
	}

    glGetObjectParameterivARB(programId, GL_OBJECT_LINK_STATUS_ARB, &linked);

    loadLog(programId);

    if (!linked)
	{
        return false;
	}

	return true;
}

std::string Shader::getShaderCode(Text* text, const StringDict& parameters)
{
	std::string shaderOriginal = text->getText();
	std::string defines;

	for EACH_CONST(StringDict, parameters, it)
	{
		defines += "#define " + it->first + " " + it->second + "\n";
	}
	defines += "#line 2\n";

	return defines + shaderOriginal;
}

bool Shader::loadShader(GLuint shader, Text* text, const StringDict& parameters)
{
	std::string shaderCode = getShaderCode(text, parameters);
	const char *body = (const char*)shaderCode.c_str();
    int			len  = shaderCode.length();
    GLint       compileStatus;

    glShaderSourceARB(shader, 1, &body,  &len);
    glCompileShaderARB(shader);

    if (isError())
	{
		return false;
	}

    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
    loadLog(shader);

    return compileStatus != 0;
}


bool Shader::isError()
{
 	GLenum glErr = glGetError();

    if ( glErr == GL_NO_ERROR )
	{
    	return false;
	}

    error("opengl glsl", (const char*)gluErrorString (glErr));
    return true;
}

Shader::~Shader()
{
	if (programId) glDeleteObjectARB(programId);                   // it will also detach shaders
	if (vertexShaderId) glDeleteObjectARB(vertexShaderId);
	if (fragmentShaderId) glDeleteObjectARB(fragmentShaderId);
}

void Shader::loadLog(GLuint object)
{
    int         logLength     = 0;
    int         charsWritten  = 0;
    GLcharARB   buffer [2048];
    GLcharARB * infoLog;

    glGetObjectParameterivARB ( object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );

    if (isError())          // check for OpenGL errors
        return;

    if (logLength < 1 )
        return;
                                    // try to avoid allocating buffer
    if (logLength > (int)sizeof(buffer))
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            error("opengl glsl error", "Could not allocate log buffer");

            return;
        }
    }
    else
        infoLog = buffer;

    glGetInfoLogARB ( object, logLength, &charsWritten, infoLog );

	if(strlen(infoLog)>0)
		log_msg("opengl glsl info", infoLog);

    if ( infoLog != buffer )
        free ( infoLog );
}

void Shader::bind()
{
	glUseProgramObjectARB(programId);
}

void Shader::unbind()
{
	glUseProgramObjectARB(0);
}


GLint Shader::findVariable(const std::string& name)
{
	VariableLocationCache::iterator it = variableLocationCache.find(name);
	if (it != variableLocationCache.end())
	{
		return it->second;
	}
	else
	{
		GLint loc = glGetUniformLocationARB(programId, name.c_str());
		variableLocationCache.insert(make_pair(name, loc));
		if (loc < 0)
		{
			error("opengl glsl", std::string("Cannot find variable '") + name + "' in shader " + getShaderDecription());
		}
		return loc;
	}
}


bool Shader::setUniformFloat(const std::string& name, float value)
{
	GLint loc = findVariable(name);

	if (loc >= 0)
	{
		glUniform1fARB(loc, value);
		return true;
	}

    return true;
}

bool Shader::setUniformVector(const std::string& name, const v3& value)
{
	GLint loc = findVariable(name);

    if (loc >= 0)
	{
	    glUniform3fvARB(loc, 1, value);
        return true;
	}
    return false;
}

bool Shader::setUniformInt(const std::string& name, int value)
{
	GLint loc = findVariable(name);
    if (loc >= 0)
	{
		glUniform1iARB(loc, value);
	    return true;
	}
	return false;
}


void Shader::bindTexture(const std::string& name, Image* image)
{
	if (image != NULL)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + textureIndex);
		glClientActiveTextureARB(GL_TEXTURE0_ARB + textureIndex);

		(engine.*(engine.BindTexture))(*image, false);

		GLint loc = findVariable(name);
		if (loc >= 0)
		{
			glUniform1iARB(loc, textureIndex);
			textureIndex++;
		}
		else
		{
			error("glsl error", std::string("Cannot find texture '") + name + "' in shader " + getShaderDecription());
		}
	}
}

std::string Shader::getShaderDecription() const
{
	return fragmentShader->getFileName() + " (" + joinMap(parameters, ", ") + ")";
}

void Shader::clearTextures()
{
	textureIndex = 0;
}


} // namespace opengl
