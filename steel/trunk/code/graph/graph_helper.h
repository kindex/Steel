#ifndef __GRAPH_HELPER_H
#define __GRAPH_HELPER_H

#include "../math/line.h"
#include "../helper.h"
#include "graph_interface.h"

class GraphHelper: virtual public EngineHelper, virtual public GraphObject
{
	steel::time time;

	struct Element
	{
		steel::time start, fade, end;
		color4f color;
	};

	struct LineElement: public Element
	{
		Line line;
		LineElement() {}
		LineElement(const Line _line, const steel::time _start, const steel::time _fade, const steel::time _end, const color4f _color):line(_line)	{ start = _start; fade = _fade; end = _end; color = _color;	}	
	};
	struct BoxElement: public Element
	{
		aabb box;
		BoxElement() {}
		BoxElement(const aabb _box, const steel::time _start, const steel::time _fade, const steel::time _end, const color4f _color) :box(_box)		{ 	start = _start; fade = _fade; end = _end; 	color = _color;	}
	};

	steel::vector<LineElement> lines, vectors;
	steel::vector<BoxElement> boxes;

	Vertexes	vertex;
	FaceMaterials face;
	GLines		glines;

public:
	GraphHelper(): lines(0), vectors(0), boxes(0), vertex(), face(), glines() {}
// Helper
	void setTime(const steel::time _time);
	void clean() {}
	void drawLine(const Line line, const steel::time duration, const steel::time fade, const color4f color);
	void drawVector(const Line line, const steel::time duration, const steel::time fade, const color4f color);
	void drawBox(const aabb box, const steel::time duration, const steel::time fade, const color4f color);

// Graph
	void		ProcessGraph(const GraphEngineInfo &info);

	ObjectPosition getPosition(void) { return matrix34::getIdentity(); }
	
	PositionKind	getPositionKind() {return POSITION_GLOBAL; }

	GLines*		getLines()		{	return &glines;	}
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
