/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    Licence:
        Только для Division
    Description:
		Графический джижок. Получет трехмерный объект в виде 3d-mesh
		и потомки графического движка (OpenGL_Engine) занимаются
		отображением этого объекта на экране, расчётом динамического
		освещения, инициализацией граф. режима.
 ************************************************************/

#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../res/conf/conf.h"

/********************************* GRAPH **********************************************/

#include "graph_interface.h"

class GraphEngine;

class Camera
{
public:
    v3 eye, center, up;
public:
    void seteye(const v3 &EYE) { eye = EYE; }
    void setcenter(const v3 &CENTER) { center = CENTER; }

	void setup(const v3 &EYE, const v3 &DIR)
	{
		eye = EYE;
		center = EYE + DIR;
	}

	Camera(): 
		up(v3(0.0, 0.0, 1.0)), 
		eye(10.0, 10.0, 1.0), 
		center(v3(0.0, 0.0, 0.0)) {}

};

class GraphEngine: public Engine
{
protected:
	struct DrawElement
	{
		std::string material;

		Vertexes	*vertex;
		Triangles	*triangle;
		MapCoords	*mapcoord;
		Normals		*normal;
		MATRIX4X4	matrix;
//		std::vector<v3> sTangent, tTangent, tangentSpaceLight;
	};

	// data, to store collected information
	std::vector<DrawElement> elements;
	std::vector<GraphInterface*> objects;
	Lights light;

public:
	Camera camera;
	virtual void processCamera() = 0;
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object, MATRIX4X4 matrix = MATRIX4X4());
	// Draw colelcted information. May be called few times without recollection information
	virtual bool process() = 0; // Override OpenGL, D3D, ...\
	// Clear collected information
	bool clear();
};

#endif

