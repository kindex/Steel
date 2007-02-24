/*id*********************************************************
	File: graph/opengl/opengl_glsl.h
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2006
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

namespace opengl
{

class GLSL
{
public:
	GLSL(): programId(0), vertexShaderId(0), fragmentShaderId(0)
		, shader(NULL)
	{};

	~GLSL();

	GLuint getProgramId() { return programId; }
	bool LoadShader(GLuint shader, Text* text);

	bool init(Shader*);
	bool isError();
	void loadLog(GLuint object);
	void bind();
	void unbind();
	bool setTexture(const char* name, int texNum);

	GLuint getGL_Id() { return programId;}

//    bool        setUniformVector  ( const char * name, const Vector4D& value  );
//    bool        setUniformVector  ( int loc,            const Vector4D& value );
	bool        setUniformVector  ( const std::string& name, const v3& value  );
    bool        setUniformVector  ( int loc,            const v3& value );
    bool        setUniformVector  ( const char* name, const v2& value  );
    bool        setUniformVector  ( int loc,            const v2& value );
    bool        setUniformFloat   (const std::string& name, float value);
    bool        setUniformFloat   ( int loc,            float value           );
//    bool        setUniformMatrix  ( const char * name, const Matrix4x4& value );
    bool        setUniformMatrix  ( const char* name, const matrix33&  value );
    bool        setUniformMatrix  ( const char* name, float value [16]       );
    bool        setUniformInt     ( const std::string& name, int value              );
    bool        setUniformInt     ( int loc,            int value             );
//    Vector4D    getUniformVector  ( const char * name );
    //Vector4D    getUniformVector  ( int loc            );

    int         locForUniformName ( const char * name );

protected:
	GLuint programId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;
	Shader* shader;
};

} // namespace opengl

#endif
