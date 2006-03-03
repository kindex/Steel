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

class GraphEngine: public Engine
{
protected:
	// data, to store collected information
	vertexes	vertex;
	triangles	triangle;
public:
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

