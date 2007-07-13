/*id*********************************************************
	File: objects/combiner/graph_object.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Физических объектов для Combiner
 ************************************************************/

#ifndef __OBJECTS__COMBINER__GRAPH_OBJECT_H
#define __OBJECTS__COMBINER__GRAPH_OBJECT_H

#include "../../steel.h"
#include "../../engine/game_object.h"
#include "../../graph/graph_types.h"

class Model;
class GraphObject;

GraphObject* graphObjectFactory(const std::string& _class);

class GraphObject: public GameObject
{
public:
	bool supportsInterface(IN OUT Engine&, IN const InterfaceId id);
};

class GraphObjectModel: public GraphObject
{
public:
	GraphObjectModel();
	bool InitFromConfig(Config& conf);
	void bindEngine(Engine&, IN const InterfaceId id);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id){return false;}
	void process(IN const ProcessInfo&) {}

protected:
	Model* model;
};

class GraphObjectCustom: public GraphObject
{
public:
	GraphObjectCustom();
	void bindEngine(Engine&, IN const InterfaceId id);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id){return false;}
	void process(IN const ProcessInfo&) {}

protected:
	VertexVector*       vertexes;
	Normals*            normals;
	FaceMaterialVector* faces;
	Faces*              allFaces;
	TexCoords*          texCoords;
};

class GraphObjectMesh: public GraphObjectCustom
{
public:
	bool InitFromConfig(Config& conf);
};

class GraphObjectBox: public GraphObjectCustom
{
public:
	bool InitFromConfig(Config& conf);
};


#endif
