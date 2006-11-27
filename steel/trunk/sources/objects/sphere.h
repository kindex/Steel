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
#include "../graph/graph_engine.h"

class Sphere: public GameObject
{
protected:
	Vertexes *vertexes, *normals;
	FaceMaterials *faces;
	TexCoords *texCoords0, *texCoords1;
	GraphEngine *graphEngine;

public:
	Sphere(void);
	virtual ~Sphere(void);

	bool isSuportingInterface(InterfaceId id) { return id == GraphEngine::interfaceId; }
	bool updateInformation(InterfaceId id, Engine* engine);
	void bindEngine(InterfaceId, Engine*);

	void ProcessGraph();
	void DeleteTriangle(int n);
};

#endif
