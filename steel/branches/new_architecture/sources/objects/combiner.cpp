/*id*********************************************************
	File: objects/combiner.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Компоновщик графических и физических объектов.
		Позволяет в одном объекте (combiner) объединять свойства
		одного физического объекта и одного графического.
 ************************************************************/

#include "../steel.h"
#include "combiner.h"
#include "../graph/graph_engine.h"

GraphObject::GraphObject(void): 
	vertexes(NULL), 
	normals(NULL), 
	faces(NULL),
	texCoords(NULL)
{}


bool GraphObject::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	ConfigArray *vertexesConfig = conf->getArray("vertexes");  if(vertexesConfig == NULL) return false;
	ConfigArray *trianglesConfig = conf->getArray("triangles");if(trianglesConfig == NULL) return false;
	ConfigArray *texCoordsConfig = conf->getArray("texCoords");if(texCoordsConfig == NULL || texCoordsConfig->size() != vertexesConfig->size()) return false;
	Config *materialConfig = conf->find("material");

	Material *m = NULL;
	if(materialConfig != NULL)
		m = new Material(materialConfig);


	std::string s = vertexesConfig->Dump();

	int vert = vertexesConfig->size();
	int trg  = trianglesConfig->size();

	vertexes = new Vertexes; vertexes->changed = false;	vertexes->data.resize(vert);
	texCoords = new TexCoords; texCoords->changed = false;	texCoords->data.resize(vert);

	for(int i = 0; i < vert; i++)
	{
		vertexes->data[i] = vertexesConfig->getArrayElement(i)->getv3("");
		
		texCoords->data[i].x = texCoordsConfig ->getArrayElement(i)->getf("[0]");
		texCoords->data[i].y = texCoordsConfig ->getArrayElement(i)->getf("[1]");
	}

	faces = new FaceMaterials(1);
	faces->at(0).material = m;
	faces->at(0).triangles = new Triangles;
	faces->at(0).triangles->changed = false;
	faces->at(0).triangles->data.resize(trg);

	for(int i = 0; i < trg; i++)
	{
		faces->at(0).triangles->data[i].a[0] =  trianglesConfig->getArrayElement(i)->geti("[0]");
		faces->at(0).triangles->data[i].a[1] =  trianglesConfig->getArrayElement(i)->geti("[1]");
		faces->at(0).triangles->data[i].a[2] =  trianglesConfig->getArrayElement(i)->geti("[2]");
	}

	return true;
}

void GraphObject::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId)
	{
		GraphEngine &gengine = *static_cast<GraphEngine*>(engine);
		gengine.setVertexes(vertexes);
		gengine.setFaceMaterials(faces);
		gengine.setTexCoordsCount(1);
		gengine.setTexCoords(0, texCoords);
	}
}

bool Combiner::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;

	position.loadIdentity();
	position.setTranslation(conf->getv3("origin"));
	positionKind = conf->gets("position_kind", "global") == "local"?POSITION_LOCAL:POSITION_GLOBAL;

	graph = new GraphObject;

	bool ok =  graph->InitFromConfig(conf->find("graph"));
	if(!ok)
	{
		delete graph; 
		graph = NULL; 
	}

	return true;
}

