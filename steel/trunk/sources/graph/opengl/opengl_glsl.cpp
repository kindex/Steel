/*id*********************************************************
	File: graph/opengl/opengl_glsl.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		GLSL шейдёры
 ************************************************************/

#include "../../steel.h"
#include "opengl_glsl.h"
#include "../../res/res_main.h"
#include <gl/glu.h>

bool GLSL::init(Shader *_shader)
{
	if(!GL_EXTENSION_GLSL) return false;

	shader = _shader;

	programId = glCreateProgramObjectARB();
	if(isError()) return false;

    vertexShaderId = glCreateShaderObjectARB (GL_VERTEX_SHADER_ARB);
   	if(!LoadShader(vertexShaderId, shader->vertexShader))
	{
		loadLog(programId);
		return false;
	}
    glAttachObjectARB(programId, vertexShaderId);

    fragmentShaderId = glCreateShaderObjectARB (GL_FRAGMENT_SHADER_ARB);
   	if(!LoadShader(fragmentShaderId, shader->fragmentShader))
		return false;
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
        return false;

	return true;
}

bool GLSL::LoadShader(GLuint shader, Text *text)
{
	const char *body = (const char *) text->getText();
    int			len  = text->getLength();
    GLint       compileStatus;

    glShaderSourceARB(shader, 1, &body,  &len);

                                        // compile the particle vertex shader, and print out
    glCompileShaderARB(shader);

    if (isError())              // check for OpenGL errors
        return false;

    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);

    loadLog(shader);

    return compileStatus != 0;
}


bool GLSL::isError()
{
 	GLenum  glErr   = glGetError();

    if ( glErr == GL_NO_ERROR )
    	return false;

    log_msg("opengl glsl error", (const char *) gluErrorString (glErr));		// XXX: what about gluErrorString errors ?

    return true;
}

GLSL::~GLSL()
{
	if(programId) glDeleteObjectARB(programId);                   // it will also detach shaders
	if(vertexShaderId) glDeleteObjectARB(vertexShaderId);
	if(fragmentShaderId) glDeleteObjectARB(fragmentShaderId);
}

void GLSL::loadLog(GLuint object)
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

void GLSL::bind(void)
{
	glUseProgramObjectARB(programId);
}

void GLSL::unbind(void)
{
	glUseProgramObjectARB(0);
}


bool GLSL::setTexture(const char *name, int texNum)
{
	int loc = glGetUniformLocationARB(programId, name);
	if(loc != -1)
	{
		glUniform1iARB(loc, texNum);
		return true;
	}
	else
		return false;
}


bool    GLSL :: setUniformVector ( const char * name, const v3& value )
{
    int loc = glGetUniformLocationARB ( programId, name );

    if ( loc < 0 )
        return false;

    glUniform3fvARB ( loc, 1, value );

    return true;
}

bool    GLSL :: setUniformVector  ( int loc, const v3& value )
{
    glUniform3fvARB ( loc, 1, value );

    return true;
}

bool    GLSL :: setUniformVector ( const char * name, const v2& value )
{
    int loc = glGetUniformLocationARB ( programId, name );

    if ( loc < 0 )
        return false;

    glUniform2fvARB ( loc, 1, value );

    return true;
}

bool    GLSL :: setUniformVector  ( int loc, const v2& value )
{
    glUniform2fvARB ( loc, 1, value );

    return true;
}

bool    GLSL :: setUniformFloat ( const char * name, float value )
{
    int loc = glGetUniformLocationARB ( programId, name );

    if ( loc < 0 )
        return false;

    glUniform1fARB ( loc, value );

    return true;
}

bool    GLSL :: setUniformFloat ( int loc, float value )
{
    glUniform1fARB ( loc, value );

    return true;
}

bool    GLSL :: setUniformInt ( const char * name, int value )
{
    int loc = glGetUniformLocationARB ( programId, name );

    if ( loc < 0 )
        return false;

    glUniform1iARB ( loc, value );

    return true;
}

bool    GLSL :: setUniformInt ( int loc, int value )
{
    glUniform1iARB ( loc, value );

    return true;
}

bool    GLSL :: setUniformMatrix  ( const char * name, float value [16] )
{
    int loc = glGetUniformLocationARB ( programId, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fvARB ( loc, 1, GL_FALSE, value );

    return true;
}

bool    GLSL :: setUniformMatrix  ( const char * name, const matrix33& value )
{
    int loc = glGetUniformLocationARB ( programId, name );

    if ( loc < 0 )
        return false;

	glUniformMatrix3fvARB ( loc, 1, GL_FALSE, &value.data.a[0] );

    return true;
}

int     GLSL :: locForUniformName ( const char * name )
{
    return glGetUniformLocationARB ( programId, name );
}

