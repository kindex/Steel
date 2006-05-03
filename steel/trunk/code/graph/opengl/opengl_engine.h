/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    Licence:
        Только для Division
    Description:
		Графический движок реализлванный на фунциях OpenGL. 
		Рисует трехмерный объект в виде 3d-mesh,
		расчитывает динамическое освещение
 ************************************************************/

#ifndef __GRAPH_OPENGL_ENGINE_H
#define __GRAPH_OPENGL_ENGINE_H

#include <string>

#include "../graph_engine.h"
#ifdef __linux
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#else
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#endif

struct OpenGLBuffer
{
	typedef enum 
	{
		none,
		image,
		array,
		index
	} buffer_kind;

	GLuint	glid;
	int		usedCnt, loadCnt;
	steel::time lastUsedTime;
	bool	loaded;
	buffer_kind kind;
};

class OpenGL_Engine: public GraphEngine
{
protected:
	std::map<uid, OpenGLBuffer> buffer;

		//, registedCubeMaps;

	GLuint normalisationCubeMap, lightCubeMap, distMap;

	typedef
		std::vector<v3>
		v3List;
	typedef 
		std::map<int, v3List*>
		tangentCache;

	 tangentCache tangentSpaceCacheS, tangentSpaceCacheT;

	 bool focused;
	 steel::time time;

public:
	void processCamera();
	bool init(std::string _conf);
	bool process();
	bool deinit();

	virtual void swapBuffers() = 0;
	virtual bool createWindow() = 0;
	virtual void setCaption(std::string caption) = 0;
	virtual bool isFocused() { return focused; }
	
	void drawElement(DrawElement &e);
	
//	void drawFaces(DrawElement &e);
	void drawNormals(DrawElement &e);
	void drawVertexes(DrawElement &e);
	void drawAABB(DrawElement &e, matrix44 matrix);

	void drawBump(DrawElement &e, GLuint normalMap, matrix44 const matrix, v3 const light);
	void drawReflect(DrawElement &e, GLuint cubeMap, matrix44 const matrix, v3 const light);
	bool drawDiffuse(DrawElement &e, matrix44 const matrix, v3 const light);
	void drawDistColor(DrawElement &e, matrix44 const matrix, v3 const light, float const distance);

//	void getTangentSpace(Vertexes const *vertex, MapCoords2f const *mapcoord, Triangles const *triangle, Normals const *normal, std::vector<v3> **sTangent, std::vector<v3> **tTangent);

	//void genTangentSpaceLight(std::vector<v3> const &sTangent, std::vector<v3> const &tTangent, Vertexes const &vertex, Normals	const &normal, matrix44 const matrix, const v3 light,	v3List **tangentSpaceLight);
	//void genTangentSpaceSphere(std::vector<v3> const &sTangent, std::vector<v3> const &tTangent, Vertexes const &vertex, Normals	const &normal, matrix44 const matrix, const v3 camera,	v3List **tangentSpaceLight);

//	GLuint getCubeMap(std::string imageName);

	bool bindTexture(Image *image);

	template<class Vertexes> bool bind(Vertexes *v, int mode, int mode2, int elCnt);
/*	bool bindTexCoords(MapCoord *coord);
	bool bindVertexes(Vertexes *v);*/
};



#endif
