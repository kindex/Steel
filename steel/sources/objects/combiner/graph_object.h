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

    virtual const VertexVector*       getVertexes() const { return NULL; }
	virtual const Normals*            getNormals() const { return NULL; }
	virtual const FaceMaterialVector* getFaces() const { return NULL; }
	virtual const Faces*              getAllFaces() const { return NULL; }
	virtual const TexCoords*          getTexCoords(int mapNumber) const { return NULL; }
	virtual const GraphTextVector*    getGraphText() const { return NULL; }
};

class GraphObjectText: public GraphObject
{
public:
	bool InitFromConfig(Config& conf);
    Config* getConfig() const;
	void bindEngine(Engine&, IN const InterfaceId id);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id){return false;}
	void process(IN const ProcessInfo&) {}

protected:
    GraphTextVector text;
};

class GraphObjectModel: public GraphObject
{
public:
	GraphObjectModel();
	bool InitFromConfig(Config& conf);
    Config* getConfig() const;
	void bindEngine(Engine&, IN const InterfaceId id);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id){return false;}
	void process(IN const ProcessInfo&) {}

    const VertexVector*       getVertexes() const;
    const Normals*            getNormals() const;
    const FaceMaterialVector* getFaces() const;
    const Faces*              getAllFaces() const;
    const TexCoords*          getTexCoords(int mapNumber) const;

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

    const VertexVector*       getVertexes() const { return vertexes; }
    const Normals*            getNormals() const { return normals; }
    const FaceMaterialVector* getFaces() const { return faces; }
    const Faces*              getAllFaces() const { return allFaces; }
    const TexCoords*          getTexCoords(int mapNumber) const { return texCoords; }

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
    Config* getConfig() const;
};

class GraphObjectBox: public GraphObjectCustom
{
public:
    GraphObjectBox();
	bool InitFromConfig(Config& conf);
    Config* getConfig() const;
    v3 getSize() const;
    void setSize(const v3 new_size);

private:
    v3 size;
    MaterialStd* material;
};


#endif
