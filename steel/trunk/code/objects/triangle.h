/*id*********************************************************
	File: objects/triangle.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Програмно создаваемый треугольник
 ************************************************************/

#ifndef __GAME_OBJECTS_CUSTOM_H
#define __GAME_OBJECTS_CUSTOM_H

#include "../steel.h"
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

	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }
	Vertexes*	getVertexes(void) { return &vertex; }
	Vertexes*	getPVertexes(void) { return getVertexes(); }
	Triangles*	getTriangles(void) { return &triangle; }
	FaceMaterials* getFaceMaterials(void) { return &face; }
	aabb getFrame(void)		{	return frame; }
	aabb getPFrame(void)	{	return frame; }
};

#endif
