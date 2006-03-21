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

class OpenGL_Engine: public GraphEngine
{
protected:
	map<std::string, GLuint> registredTextures;

    void create();
    void repair();

	GLuint normalisationCubeMap;
	void makeNormalisationCubeMap();

	typedef
		std::vector<v3>
		v3List;
	typedef 
		std::map<int, v3List*>
		tangentCache;

	 tangentCache tangentSpaceCacheS, tangentSpaceCacheT;

public:
	void processCamera();
	bool init();
	bool process();
	virtual bool deinit();
	
	void drawElement(DrawElement &e);
	void drawBump(DrawElement &e, GLuint normalMap, MATRIX4X4 const matrix, v3 const light);
	void drawFaces(DrawElement &e);

	GLuint getTexture(std::string imageName);

	virtual void swapBuffers() = 0;
	virtual bool createWindow() = 0;
	virtual void setCaption(std::string caption) = 0;

	void getTangentSpace(Vertexes const *vertex, MapCoords const *mapcoord, Triangles const *triangle, Normals const *normal,
									std::vector<v3> **sTangent, std::vector<v3> **tTangent);

	void genTangentSpaceLight(
	std::vector<v3> const &sTangent, 
	std::vector<v3> const &tTangent, 
	Vertexes const &vertex, 
	Normals	const &normal,
	MATRIX4X4 const matrix, const v3 light,
	v3List **tangentSpaceLight);

	void drawNormals(DrawElement &e);


};



#endif
