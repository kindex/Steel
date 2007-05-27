/*id*********************************************************
	File: objects/sphere.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Многогранник с изменяемой формой и количеством полигонов
 ************************************************************/

#ifndef __OBJECTS_SPHERE_H
#define __OBJECTS_SPHERE_H

#include <steel.h>
#include <graph/graph_interface.h>
#include <graph/graph_engine.h>
#include <engine/game_object.h>

class Sphere: public GameObject
{
public:
	Sphere();
	virtual ~Sphere();

	bool beforeInject(IN OUT Engine&){return true;}
	void afterRemove(IN OUT Engine&){}
	bool updateInformation(IN OUT Engine&);
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
	bool isSuportingInterface(Engine& engine)
	{
		return engine.isSupportingInterface(INTERFACE_GRAPH);
	}


private:
	void DeleteTriangle(int n);
	void createSphere();

	float size;
	int height;
	int radius;

	ObjectPosition position;
	Config	*conf;

	Vertexes *vertexes, *normals;
	FaceMaterialVector *faces;
	TexCoords *texCoords0, *texCoords1;
	GraphEngine *graphEngine;

};

#endif
