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
	Config	*conf;
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

	GLuint getGL_Id(void) { return programId;}
};


#endif
