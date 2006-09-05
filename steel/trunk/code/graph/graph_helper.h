/*id*********************************************************
	File: graph/graph_helper.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Class for debug information output (for Physic Engine)
		using graph engine
 ************************************************************/

#ifndef __GRAPH_HELPER_H
#define __GRAPH_HELPER_H

#include "../steel.h"
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
		LineElement(void) {}
		LineElement(const Line _line, const steel::time _start, const steel::time _fade, const steel::time _end, const color4f _color):line(_line)	{ start = _start; fade = _fade; end = _end; color = _color;	}	
	};
	struct BoxElement: public Element
	{
		aabb box;
		BoxElement(void) {}
		BoxElement(const aabb _box, const steel::time _start, const steel::time _fade, const steel::time _end, const color4f _color) :box(_box)		{ 	start = _start; fade = _fade; end = _end; 	color = _color;	}
	};

	steel::vector<LineElement> lines, vectors;
	steel::vector<BoxElement> boxes;

	Vertexes	vertex;
	FaceMaterials face;
	GLines		glines;

public:
	GraphHelper(void): lines(0), vectors(0), boxes(0), vertex(), face(), glines() {}
// Helper
	void setTime(const steel::time _time);
	void clean(void) {}
	void drawLine(const Line line, const steel::time duration, const steel::time fade, const color4f color);
	void drawVector(const Line line, const steel::time duration, const steel::time fade, const color4f color);
	void drawBox(const aabb box, const steel::time duration, const steel::time fade, const color4f color);

// Graph
	void		ProcessGraph(const GraphEngineInfo &info);

	ObjectPosition getPosition(void) { return matrix34::getIdentity(); }
	
	PositionKind	getPositionKind(void) {return POSITION_GLOBAL; }

	GLines*		getLines(void)		{	return &glines;	}
	uid			getId(void)			{	return 0;		}
	aabb		getFrame(void)		{	return aabb(v3(-INF, -INF, -INF), v3(INF, INF, INF));	}
	bool		cleanup(void)		{	return true;	}
	Vertexes*	getVertexes(void)	{	return &vertex;	}
	Normals*	getNormals(void)	{	return NULL;	}
	Lights*		getLights(void)		{	return NULL;	}
	FaceMaterials* getFaceMaterials(void)	{	return &face;	}
	TexCoords*	getTexCoords(int texNumber)	{	return NULL;	}
};

#endif
