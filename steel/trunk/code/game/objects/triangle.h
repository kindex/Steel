#ifndef __GAME_OBJECTS_CUSTOM_H
#define __GAME_OBJECTS_CUSTOM_H

#include "../game_obj.h"

class GameTriangleObj: public GameObjDummy
{
protected:
	Vertexes	vertex;
	Triangles	triangle;
	FaceMaterials	face;
	TexCoords	texCoords;
public:
	bool init(ScriptLine	&s, ResCollection &res);

	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }
	Vertexes*	getVertexes() { return &vertex; }
	Vertexes*	getPVertexes() { return getVertexes(); }
	Triangles*	getTriangles() { return &triangle; }
	FaceMaterials* getFaceMaterials() { return &face; }
};

#endif
