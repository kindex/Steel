#ifndef __GRAPH_RES_MODEL_H
#define __GRAPH_RES_MODEL_H

#include "../graph_interface.h"
#include "../../res/model/model.h"

class res_model: public GraphInterface
{
private:
	Model *m;
public:
	void assign(Model *M)
	{
		m = M;
	}

	GraphInterfaceList getChildrens()
	{
		GraphInterfaceList a;
		return a;
	}
	v3 getPos()
	{
		return v3(0.0, 0.0, 0.0);
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
		return &m->vertex;
	}
	Triangles*	getTriangles()
	{
		return &m->triangle;
	}
	MapCoords*	getMapCoords()
	{
		return &m->mapcoord;
	}
	FaceMaterials* getFaceMaterials()
	{
		return &m->faceMaterial;
	}

};

#endif
