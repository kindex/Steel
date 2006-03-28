/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс GModel является посредником между коллекцией ресурсов 
		и графическим интерфейсом
 ************************************************************/

#ifndef GRAPH_PRIMITIVES_H
#define GRAPH_PRIMITIVES_H

#include "graph_interface.h"
#include "../res/model/model.h"

class GDummy: public GraphInterface
{
public:
	GraphInterfaceList getChildrens()
	{
		GraphInterfaceList a;
		return a;
	}
	aabb getFrame()	{		aabb A;		return A;	}

	bool cleanup()	{ return true; }

	Vertexes*	getVertexes()	{		return NULL;	}
	Triangles*	getTriangles()	{		return NULL;	}
	MapCoords*	getMapCoords()	{		return NULL;	}
	Normals*	getNormals()	{		return NULL;	}
	FaceMaterials* getFaceMaterials()	{		return NULL;	}
	MATRIX4X4 getMatrix()		{		return MATRIX4X4();	}
	Lights* getLights()			{		return NULL;	}
};


class GModel: public GDummy
{
protected:
	Model *m;
public:
	GModel() { m = NULL; }
	GModel(Model *M) { m = M;}

	void assignModel(Model *M)
	{
		m = M;
	}

	GraphInterfaceList getChildrens()
	{
		GraphInterfaceList a;
		return a;
	}
	// TODO
	aabb getFrame()	{		aabb A;		return A;	}

	bool cleanup(){	return true;}

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
	Normals* getNormals()
	{
		if(m)
			return &m->normal;
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
