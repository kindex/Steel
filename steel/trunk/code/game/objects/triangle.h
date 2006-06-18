#ifndef __GAME_OBJECTS_CUSTOM_H
#define __GAME_OBJECTS_CUSTOM_H

#include "../game_obj.h"

class GameTriangleObj: public GameObj
{
protected:
	Vertexes	vertex;
	Triangles	triangle;
	FaceMaterials	face;
	TexCoords	texCoords;
	aabb		frame;
public:
	bool init(ScriptLine	&s);

	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }
	Vertexes*	getVertexes() { return &vertex; }
	Vertexes*	getPVertexes() { return getVertexes(); }
	Triangles*	getTriangles() { return &triangle; }
	FaceMaterials* getFaceMaterials() { return &face; }
	aabb getFrame()		{	return frame; }
	aabb getPFrame()	{	return frame; }
};

#endif
