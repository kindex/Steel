#ifndef __GRAPH_HELPER_H
#define __GRAPH_HELPER_H

#include "../helper.h"
#include "graph_interface.h"
#include <vector>

class GraphHelper: virtual public EngineHelper, virtual public GraphInterface
{
	steel::time time;

	struct Element
	{
		steel::time start, fade, end;
		v4 color;
	};

	struct LineElement: public Element
	{
		Line line;

		LineElement() {}
		LineElement(const Line _line, const steel::time _start, const steel::time _fade, const steel::time _end, const v4 _color) 
			:line(_line)
		{ 
			start = _start; fade = _fade; end = _end; 	
			color = _color;
		}
	};
	std::vector<LineElement> lines, vectors;

	Vertexes	vertex;
	FaceMaterials face;

public:
// Helper
	void setTime(const steel::time _time);
	void clean() {}
	void drawLine(const Line line, const steel::time duration, const steel::time fade, const v4 color) {}
	void drawVector(const Line line, const steel::time duration, const steel::time fade, const v4 color);

// Graph
	void		processGraph(v3	cameraEye, v3 cameraDirection);

	matrix44	getMatrix()		{	return matrix44(); }
	void		setMatrix(matrix44 const &m) {}
	PositionKind	getPositionKind() {return Interface::global; }
	void		changePositionKind(const PositionKind newKind) {}
	GraphInterfaceList getChildrens() { return GraphInterfaceList(0);}

	uid			getId()			{	return 0;		}
	aabb		getFrame()		{	return aabb(v3(-INF, -INF, -INF), v3(INF, INF, INF));	}
	bool		cleanup()		{	return true;	}
	Vertexes*	getVertexes()	{	return &vertex;	}
	Normals*	getNormals()	{	return NULL;	}
	Lights*		getLights()		{	return NULL;	}
	FaceMaterials* getFaceMaterials()	{	return &face;	}
	TexCoords*	getTexCoords(int texNumber)	{	return NULL;	}


};

#endif
