#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../engine.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/geometry.h"

// RES
#include "../res/model/model.h"

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

	Camera(): 
		up(v3(0.0, 0.0, 1.0)), 
		eye(10.0, 10.0, 1.0), 
		center(v3(0.0, 0.0, 0.0)) {}

};

typedef
	vector<unsigned int>
	vertexNumbers;

class GraphEngine: public Engine
{
protected:
	// data, to store collected information
	vertexes		vertex;
	vertexNumbers	triangle;
public:
	Camera camera;
	virtual void processCamera() = 0;
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object);
	// Draw colelcted information. May be called few times without recollection information
	bool process() = 0; // Override OpenGL, D3D, ...\
	// Clear collected information
	bool clear()
	{
		vertex.clear();
		triangle.clear();
		return true;
	}
};

#endif

