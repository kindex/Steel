#ifndef __GRAPH_RES_MODEL_H
#define __GRAPH_RES_MODEL_H

#include "../graph_interface.h"
#include "../../res/model/model.h"

class res_model: public GraphInterface
{
private:
	Model *m;
public:

	res_model() { m = NULL; }

	void assignModel(Model *M)
	{
		m = M;
	}

	GraphInterfaceList getChildrens()
	{
		GraphInterfaceList a;
		return a;
	}
	aabb getFrame()
	{
		aabb A;
		return A;
	}
	bool cleanup()
	{
		return true;
	}
	Vertexes*	getVertexes()
	{
		if(m)
			return &m->vertex;
		else
			return NULL;
	}
	Triangles*	getTriangles()
	{
		if(m)
			return &m->triangle;
		else
			return NULL;
	}
	MapCoords*	getMapCoords()
	{
		if(m)
			return &m->mapcoord;
		else
			return NULL;
	}
	FaceMaterials* getFaceMaterials()
	{
		if(m)
			return &m->faceMaterial;
		else
			return NULL;
	}
	MATRIX4X4 getMatrix()
	{
		return MATRIX4X4();
	}
	Lights* getLights()
	{
		return NULL;
	}


};

#endif
