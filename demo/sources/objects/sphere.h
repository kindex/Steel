/*id*********************************************************
	File: objects/sphere.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
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

	bool updateInformation(IN OUT Engine&, IN const InterfaceId id);
	void bindEngine(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
    Config* getConfig() const;
    bool supportsInterface(Engine& engine, IN const InterfaceId id)
	{
		return id == INTERFACE_GRAPH;
	}


private:
	void DeleteTriangle(int n);
	void createSphere();

	float size;
	int height;
	int radius;

	ObjectPosition      position;
	Config*             conf;

	VertexVector*       vertexes;
    VertexVector*       normals;
	FaceMaterialVector* faces;
	TexCoords*          texCoords0;
    TexCoords*          texCoords1;
	GraphEngine*        graphEngine;

};

#endif
