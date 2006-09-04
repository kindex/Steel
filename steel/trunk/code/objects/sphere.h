/*id*********************************************************
	File: objects/sphere.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Многогранник с изменяемой формой и количеством полигонов
 ************************************************************/

#ifndef __OBJECTS_SPHERE_H
#define __OBJECTS_SPHERE_H

#include "../steel.h"
#include "../graph/graph_interface.h"

class Sphere: public GraphObject
{
protected:
	Vertexes *vertexes, *normals;
	FaceMaterials *faces;
	TexCoords *texCoords0, *texCoords1;
public:
	Sphere(void);
	~Sphere(void);

	TexCoords*	getTexCoords(int texNumber)
	{
		if(texNumber == 0)
			return texCoords0;
		else
			return texCoords1;
	}
	Vertexes*	getVertexes(void) { return vertexes;}
	Normals*	getNormals(void) { return normals;} // список нормалей в вершинам
	FaceMaterials* getFaceMaterials(void) { return faces;}
	void ProcessGraph(const GraphEngineInfo &info);

	void DeleteTriangle(int n);
};

#endif
