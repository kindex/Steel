/*id*********************************************************
	File: objects/combiner/graph_object.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Физических объектов для Combiner
 ************************************************************/

#ifndef __OBJECTS__COMBINER__GRAPH_OBJECT_H
#define __OBJECTS__COMBINER__GRAPH_OBJECT_H

#include "../../steel.h"
#include "../../engine/game_object.h"
#include "../../graph/graph_interface.h"
#include "../../res/model/model.h"

class GraphObject;

GraphObject* findGraphObject(const std::string &_class);

class GraphObject: public GameObject
{
public:
	bool isSuportingInterface(InterfaceId id) { return id == GraphInterface::interfaceId; }
};

class GraphObjectModel: public GraphObject
{
public:
	GraphObjectModel(void);
	bool InitFromConfig(Config *conf);
	void bindEngine(InterfaceId, Engine*);

protected:
	Model *model;
};

class GraphObjectCustom: public GraphObject
{
public:
	GraphObjectCustom(void);

	void bindEngine(InterfaceId, Engine*);
protected:
	Vertexes *vertexes;
	Normals  *normals;
	FaceMaterials *faces;
	TexCoords *texCoords;
};

class GraphObjectMesh: public GraphObjectCustom
{
public:
	bool InitFromConfig(Config *conf);
};

class GraphObjectBox: public GraphObjectCustom
{
public:
	bool InitFromConfig(Config *conf);
};


#endif
