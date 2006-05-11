#ifndef __GRAPH_HELPER_H
#define __GRAPH_HELPER_H

#include "../helper.h"
#include "graph_interface.h"

class GraphHelper: virtual public EngineHelper, virtual public GraphInterface
{
	steel::time time;

	struct Element
	{
		steel::time start, fade, end;
	};

	struct LineElement
	{
		Line line;
	};

public:
// Helper
	void setTime(const steel::time _time);
	void clean() {}
	void drawLine(const Line line, const steel::time duration, const steel::time fade) {}
	void drawVector(const Line line, const steel::time duration, const steel::time fade) {}

// Graph
	void		processGraph(v3	cameraEye, v3 cameraDirection) {}

	matrix44	getMatrix()		{	return matrix44(); }
	void		setMatrix(matrix44 const &m) {}
	PositionKind	getPositionKind() {return Interface::global; }
	void		changePositionKind(const PositionKind newKind) {}
	virtual GraphInterfaceList getChildrens() { return GraphInterfaceList(0);}

	uid			getId()			{	return 0;		}
	aabb		getFrame()		{	return aabb(v3(-INF, -INF, -INF), v3(INF, INF, INF));	}
	bool		cleanup()		{	return true;	}
	Vertexes*	getVertexes()	{	return NULL;	}
	Triangles*	getTriangles()	{	return NULL;	}
	Normals*	getNormals()	{	return NULL;	}
	Lights*		getLights()		{	return NULL;	}
	FaceMaterials* getFaceMaterials()	{	return NULL;	}
	TexCoords*	getTexCoords(int texNumber)	{	return NULL;	}


};

#endif
