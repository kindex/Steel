/*id*********************************************************
	Unit: OpenGL Engine / GLSL
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		GLSL шейдёры
 ************************************************************/

#ifndef OPENGL_GLSL_H
#define OPENGL_GLSL_H

#include "../graph_engine.h"
#include "gl/libext.h"
#include "../../res/text/text.h"

class GLSL
{
protected:
	GLuint programId, vertexShaderId, fragmentShaderId;
	Text *vertexShader, *fragmentShader;
	Material *material;
	const Config	*conf;
public:
	GLSL(void): programId(0), vertexShaderId(0), fragmentShaderId(0),
		vertexShader(NULL), fragmentShader(NULL), material(NULL), conf(NULL) {};
	~GLSL(void);

	bool LoadShader(GLuint shader, Text *text);

	bool init(Material *material);
	bool isError(void);
	void loadLog(GLuint object);
	void bind(void);
	void unbind(void);
	bool setTexture(const char *name, int texNum);

	GLuint getGL_Id(void) { return programId;}

//    bool        setUniformVector  ( const char * name, const Vector4D& value  );
//    bool        setUniformVector  ( int loc,            const Vector4D& value );
    bool        setUniformVector  ( const char * name, const v3& value  );
    bool        setUniformVector  ( int loc,            const v3& value );
    bool        setUniformVector  ( const char * name, const v2& value  );
    bool        setUniformVector  ( int loc,            const v2& value );
    bool        setUniformFloat   ( const char * name, float value            );
    bool        setUniformFloat   ( int loc,            float value           );
//    bool        setUniformMatrix  ( const char * name, const Matrix4x4& value );
    bool        setUniformMatrix  ( const char * name, const matrix33&  value );
    bool        setUniformMatrix  ( const char * name, float value [16]       );
    bool        setUniformInt     ( const char * name, int value              );
    bool        setUniformInt     ( int loc,            int value             );
//    Vector4D    getUniformVector  ( const char * name );
    //Vector4D    getUniformVector  ( int loc            );

    int         locForUniformName ( const char * name );
};


#endif
