#ifndef __GAME_OBJECTS_CUSTOM_H
#define __GAME_OBJECTS_CUSTOM_H

#include "game_obj.h"

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

	TexCoords*	GetTexCoords(int texNumber) { return &texCoords; }
	Vertexes*	GetVertexes() { return &vertex; }
	Vertexes*	getPVertexes() { return GetVertexes(); }
	Triangles*	getTriangles() { return &triangle; }
	FaceMaterials* GetFaceMaterials() { return &face; }
	aabb getFrame()		{	return frame; }
	aabb getPFrame()	{	return frame; }
};

#endif
