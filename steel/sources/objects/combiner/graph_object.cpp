/*id*********************************************************
	File: objects/combiner/graph_object.cpp
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

#include "graph_object.h"
#include "../../graph/graph_engine.h"
#include "../../res/res_main.h"

using namespace std;

GraphObject* findGraphObject(const string &_class)
{
	if(_class == "mesh") return new GraphObjectMesh;
	if(_class == "box") return new GraphObjectBox;
	if(_class == "model") return new GraphObjectModel;

	error("objects", string("GraphObject class '") + _class + "' not found");
	return NULL;
}

// ***************** Custom *****************

GraphObjectCustom::GraphObjectCustom(): 
	vertexes(NULL), 
	normals(NULL), 
	faces(NULL),
	texCoords(NULL)
{}


void GraphObjectCustom::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId)
	{
		GraphEngine &gengine = *static_cast<GraphEngine*>(engine);
		gengine.setVertexes(vertexes);
		gengine.setNormals(normals);
		gengine.setFaceMaterials(faces);
		gengine.setTexCoordsCount(1);
		gengine.setTexCoords(0, texCoords);
	}
}

// ***************** Mesh *****************
bool GraphObjectMesh::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	ConfigArray *vertexesConfig = conf->getArray("vertexes");  if(vertexesConfig == NULL) return false;
	ConfigArray *trianglesConfig = conf->getArray("triangles");if(trianglesConfig == NULL) return false;
	ConfigArray *texCoordsConfig = conf->getArray("texCoords");if(texCoordsConfig == NULL || texCoordsConfig->size() != vertexesConfig->size()) return false;
	Config *materialConfig = conf->find("material");

	Material *m = NULL;
	if(materialConfig != NULL)
		m = createMaterial(materialConfig);

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

// ***************** Box *****************
bool GraphObjectBox::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	Config *materialConfig = conf->find("material");
	v3 size = conf->getv3("size", v3(1.0f, 1.0f, 1.0f));

	Material *m = NULL;
	if(materialConfig != NULL)
		m = createMaterial(materialConfig);

	int vert = 6*4;
	int trg  = 12;

	vertexes = new Vertexes; vertexes->changed = false;	
	normals = new Vertexes; normals->changed = false;	
	texCoords = new TexCoords; texCoords->changed = false;

#define t(a, b, c, d, e) vertexes->data.push_back(v3(a*0.5f*size.x, b*0.5f*size.y, c*0.5f*size.z)); texCoords->data.push_back(v2(d, e))

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

	faces = new FaceMaterials(1);
	faces->at(0).material = m;
	faces->at(0).triangles = new Triangles;
	faces->at(0).triangles->changed = false;
	faces->at(0).triangles->data.resize(trg);

	for(int i=0; i < trg; i++)
		if(i*3+2 < (int)vertexes->data.size())
		faces->at(0).triangles->data[i].set(i*3, i*3+1, i*3+2);

	normals->data.resize(vertexes->data.size());
	for EACH(svector<Triangle>, faces->at(0).triangles->data, it)
	{
		v3 a = vertexes->data[it->a[1]] - vertexes->data[it->a[0]];
		v3 b = vertexes->data[it->a[2]] - vertexes->data[it->a[0]];
		v3 normal = (a*b).getNormalized();
		normals->data[it->a[0]] = normal;
		normals->data[it->a[1]] = normal;
		normals->data[it->a[2]] = normal;
	}

	return true;
}

GraphObjectModel::GraphObjectModel(): 
	model(NULL) 
{}

bool GraphObjectModel::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	model = resModel.add(conf->getPath("file"));

	Config *material = conf->find("material");
	if(material != NULL)
	{
		model->faceMaterials[0].material = createMaterial(material);
	}

	return model != NULL;
}

void GraphObjectModel::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId)
	{
		GraphEngine &gengine = *static_cast<GraphEngine*>(engine);
		gengine.setVertexes(model->getVertexes());
		gengine.setNormals(model->getNormals());
		gengine.setFaceMaterials(model->getFaceMaterials());

		size_t cnt = model->getFaceMaterials()->size();
		gengine.setTexCoordsCount(cnt);
		for (size_t i = 0; i < cnt; i++)
			gengine.setTexCoords(i, model->getTexCoords(i));
	}
}