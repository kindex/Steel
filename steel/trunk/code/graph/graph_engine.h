#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../engine.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/geometry.h"
#include "../math/matrix4x4.h"

// RES
#include "../res/res.h"
#include "../res/material/material.h"

/********************************* GRAPH **********************************************/

#include "graph_interface.h"

class GraphEngine;

struct WindowSettings
{
    string caption, title; // if not fullscreen
    int left, top, width, height, bpp;
	
	bool fullscreen;

	WindowSettings();
};

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
		MATRIX4X4	matrix;
	};

	// data, to store collected information
	vector<DrawElement> elements;
	vector<GraphInterface*> objects;

	ResCollection *res;


//	vertexes		vertex;
	//vertexNumbers	triangle;
public:
	WindowSettings window;
	Camera camera;
	virtual void processCamera() = 0;
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object, MATRIX4X4 matrix = MATRIX4X4());
	// Draw colelcted information. May be called few times without recollection information
	bool process() = 0; // Override OpenGL, D3D, ...\
	// Clear collected information
	bool clear();

	void bindResColelntion(ResCollection *_res) { res = _res; }
};

#endif

