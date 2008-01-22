/*id*********************************************************
	File: objects/combiner/graph_object.cpp
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

#include "graph_object.h"
#include "../../graph/graph_engine.h"
#include "../../res/res_main.h"
#include "../../graph/material.h"
#include "../../physic/physic_interface.h"

using namespace std;

GraphObject* graphObjectFactory(const string &_class)
{
	if (_class == "mesh")	return new GraphObjectMesh;
	if (_class == "box")	return new GraphObjectBox;
	if (_class == "model")	return new GraphObjectModel;

	error("objects", string("GraphObject class '") + _class + "' not found");
	return NULL;
}

bool GraphObject::supportsInterface(Engine& engine, IN const InterfaceId id)
{ 
	return id == INTERFACE_GRAPH;
}


// ***************** Custom *****************

GraphObjectCustom::GraphObjectCustom(): 
	vertexes(NULL), 
	normals(NULL), 
	faces(NULL),
	texCoords(NULL),
    allFaces(NULL)
{}


void GraphObjectCustom::bindEngine(Engine& engine, IN const InterfaceId id)
{
    switch (id)
    {
        case INTERFACE_GRAPH:
        {
	        GraphEngine& gengine = *static_cast<GraphEngine*>(&engine);
	        gengine.setVertexes(vertexes);
	        gengine.setNormals(normals);
	        gengine.setFaceMaterials(faces);
	        gengine.setTexCoordsCount(1);
	        gengine.setTexCoords(0, texCoords);
            break;
        }
        case INTERFACE_POPYHEDRA_PHYSIC:
        {
	        PhysicPolyhedraInterface& pengine = *dynamic_cast<PhysicPolyhedraInterface*>(&engine);
            pengine.setVertexes(vertexes);
            pengine.setFaces(allFaces);
            break;
        }
    }
}

// ***************** Mesh *****************
bool GraphObjectMesh::InitFromConfig(Config& conf)
{
	ConfigArray* vertexesConfig = conf.getArray("vertexes");  if(vertexesConfig == NULL) return false;
	ConfigArray* trianglesConfig = conf.getArray("triangles");if(trianglesConfig == NULL) return false;
	ConfigArray* texCoordsConfig = conf.getArray("texCoords");if(texCoordsConfig == NULL || texCoordsConfig->size() != vertexesConfig->size()) return false;
	Config* materialConfig = conf.find("material");

	MaterialStd* m = NULL;
	if(materialConfig != NULL)
	{
		m = createMaterial(materialConfig);
	}

	int vert = vertexesConfig->size();
	int trg  = trianglesConfig->size();

	vertexes = new VertexVector; vertexes->changed = false;	vertexes->resize(vert);
	texCoords = new TexCoords; texCoords->changed = false;	texCoords->resize(vert);

	for(int i = 0; i < vert; i++)
	{
		vertexes->at(i) = vertexesConfig->getArrayElement(i)->getv3("");
		
		texCoords->at(i).x = texCoordsConfig ->getArrayElement(i)->getf("[0]");
		texCoords->at(i).y = texCoordsConfig ->getArrayElement(i)->getf("[1]");
	}

	faces = new FaceMaterialVector(1);
	faces->at(0).material = m;
	faces->at(0).faces = allFaces = new Faces;
	faces->at(0).faces->triangles.changed = false;
	faces->at(0).faces->triangles.resize(trg);

	for(int i = 0; i < trg; i++)
	{
		faces->at(0).faces->triangles[i].a[0] =  trianglesConfig->getArrayElement(i)->geti("[0]");
		faces->at(0).faces->triangles[i].a[1] =  trianglesConfig->getArrayElement(i)->geti("[1]");
		faces->at(0).faces->triangles[i].a[2] =  trianglesConfig->getArrayElement(i)->geti("[2]");
	}

	return true;
}

Config* GraphObjectMesh::getConfig() const
{
    return NULL; // TODO:
}

// ***************** Box *****************
GraphObjectBox::GraphObjectBox():
    material(NULL)
{}

bool GraphObjectBox::InitFromConfig(Config& conf)
{
	Config* materialConfig = conf.find("material");
	size = conf.getv3("size", v3(1.0f, 1.0f, 1.0f));

	if (materialConfig != NULL)
	{
		material = createMaterial(materialConfig);
	}

	int vert = 6*4;
	int trg  = 12;

	vertexes	= new VertexVector; vertexes->changed = false;	
	normals		= new VertexVector; normals->changed = false;	
	texCoords	= new TexCoords; texCoords->changed = false;

#define t(a, b, c, d, e) vertexes->push_back(v3(a*0.5f*size.x, b*0.5f*size.y, c*0.5f*size.z)); texCoords->push_back(v2(d, e))

	t(-1, -1, -1, 0, 0);	t(+1, -1, -1, 1, 0);	t(+1, -1, +1, 1, 1);
	t(-1, -1, -1, 0, 0);	t(+1, -1, +1, 1, 1);	t(-1, -1, +1, 0, 1);

	t(+1, +1, -1, 0, 0);	t(-1, +1, -1, 1, 0);	t(-1, +1, +1, 1, 1);
	t(+1, +1, -1, 0, 0);	t(-1, +1, +1, 1, 1);	t(+1, +1, +1, 0, 1);

	t(-1, +1, -1, 0, 0);	t(-1, -1, -1, 1, 0);	t(-1, -1, +1, 1, 1);
	t(-1, +1, -1, 0, 0);	t(-1, -1, +1, 1, 1);	t(-1, +1, +1, 0, 1);

	t(+1, -1, -1, 0, 0);	t(+1, +1, -1, 1, 0);	t(+1, +1, +1, 1, 1);
	t(+1, -1, -1, 0, 0);	t(+1, +1, +1, 1, 1);	t(+1, -1, +1, 0, 1);

	t(-1, -1, +1, 0, 0);	t(+1, -1, +1, 1, 0);	t(+1, +1, +1, 1, 1);
	t(-1, -1, +1, 0, 0);	t(+1, +1, +1, 1, 1);	t(-1, +1, +1, 0, 1);

	t(+1, -1, -1, 0, 0);	t(-1, -1, -1, 1, 0);	t(-1, +1, -1, 1, 1);
	t(+1, -1, -1, 0, 0);	t(-1, +1, -1, 1, 1);	t(+1, +1, -1, 0, 1);

	faces = new FaceMaterialVector(1);
	faces->at(0).material = material;
	faces->at(0).faces = allFaces = new Faces;
	faces->at(0).faces->triangles.changed = false;
	faces->at(0).faces->triangles.resize(trg);

	for(int i=0; i < trg; i++)
	{
		if(i*3+2 < (int)vertexes->size())
		{
			faces->at(0).faces->triangles[i].set(i*3, i*3+1, i*3+2);
		}
	}

	normals->resize(vertexes->size());
	for EACH(TriangleVector, faces->at(0).faces->triangles, it)
	{
		v3 a = vertexes->at(it->a[1]) - vertexes->at(it->a[0]);
		v3 b = vertexes->at(it->a[2]) - vertexes->at(it->a[0]);
		v3 normal = (a*b).getNormalized();
		normals->at(it->a[0]) = normal;
		normals->at(it->a[1]) = normal;
		normals->at(it->a[2]) = normal;
	}

	return true;
}

Config* GraphObjectBox::getConfig() const
{
    ConfigStruct* result = new ConfigStruct;

	result->setValue("class", new ConfigString("box"));
	result->setValue("size", createV3config(size));
	result->setValue("material", material->getConfig());

    return result;
}

v3 GraphObjectBox::getSize() const
{
    return size;
}

GraphObjectModel::GraphObjectModel(): 
	model(NULL) 
{}

bool GraphObjectModel::InitFromConfig(Config& conf)
{
	model = resModel.add(conf.getPath("file"));

	Config* material = conf.find("material");
	if (material != NULL && model != NULL && !model->faceMaterials.empty())
	{
		model->faceMaterials[0].material = createMaterial(material);
	}

	return model != NULL;
}

Config* GraphObjectModel::getConfig() const
{
    ConfigStruct* result = new ConfigStruct;

	result->setValue("class", new ConfigString("model"));
    if (model != NULL)
    {
	    result->setValue("file", new ConfigString(model->getPath()));

        if (!model->faceMaterials.empty() && model->faceMaterials[0].material != NULL)
        {
    	    result->setValue("material", model->faceMaterials[0].material->getConfig());
        }
    }

    return result;
}

void GraphObjectModel::bindEngine(Engine& engine, IN const InterfaceId id)
{
	if (id == INTERFACE_GRAPH)
	{
		GraphEngine& gengine = *static_cast<GraphEngine*>(&engine);
		gengine.setVertexes(model->getVertexes());
		gengine.setNormals(model->getNormals());
		gengine.setFaceMaterials(model->getFaceMaterials());

		size_t cnt = model->getFaceMaterials()->size();
		gengine.setTexCoordsCount(cnt);
		for (size_t i = 0; i < cnt; i++)
		{
			gengine.setTexCoords(i, model->getTexCoords(i));
		}
	}
    else if (id == INTERFACE_POPYHEDRA_PHYSIC)
    {
		PhysicInterface& pengine = *dynamic_cast<PhysicInterface*>(&engine);
		pengine.setVertexes(model->getVertexes());
		pengine.setFaces(&model->allFaces);
    }
}

const VertexVector*       GraphObjectModel::getVertexes() const { return model->getVertexes(); }
const Normals*            GraphObjectModel::getNormals() const { return model->getNormals(); }
const FaceMaterialVector* GraphObjectModel::getFaces() const { return model->getFaceMaterials(); }
const Faces*              GraphObjectModel::getAllFaces() const { return model->getAllFaces(); }
const TexCoords*          GraphObjectModel::getTexCoords(int mapNumber) const { return model->getTexCoords(mapNumber); }
