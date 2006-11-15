/*id*********************************************************
	File: objects/sphere.cpp
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

#include "../steel.h"
#include "sphere.h"
#include "../res/res_main.h"

void Sphere::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphEngine::interfaceId)
	{
		GraphEngine &gengine = *static_cast<GraphEngine*>(engine);

		gengine.setPositionKind(POSITION_GLOBAL);
		gengine.setPosition(ObjectPosition::getIdentity());
	}
}

bool Sphere::updateInformation(InterfaceId id, Engine* engine)
{
	if(id == GraphEngine::interfaceId)
	{
		ProcessGraph();
		GraphEngine &gengine = *static_cast<GraphEngine*>(engine);

		gengine.setCurrentObject(this);
		gengine.setVertexes(vertexes);
		gengine.setNormals(normals);
		gengine.setFaceMaterials(faces);
		gengine.setTexCoordsCount(2);
		gengine.setTexCoords(0, texCoords0);
		gengine.setTexCoords(1, texCoords1);
		return true;
	}
	return false;
}


Sphere::Sphere()
{
	int height = 12;
	int radius = 16;

	vertexes = new Vertexes;
	vertexes->setId(objectIdGenerator.genUid());
	vertexes->setChanged(true);
	vertexes->data.resize( (height+1)*radius);

	normals = new Normals;
	normals->setId(objectIdGenerator.genUid());
	normals->setChanged(true);
	normals->data.resize( vertexes->data.size() );


	texCoords0 = new TexCoords;
	texCoords0->setId(objectIdGenerator.genUid());
	texCoords0->setChanged(false);
	texCoords0->data.resize( (height+1)*radius);

	texCoords1 = new TexCoords;
	texCoords1->setId(objectIdGenerator.genUid());
	texCoords1->setChanged(false);
	texCoords1->data.resize( (height+1)*radius);

	for(int i=0; i < height+1; i++)
	{
		float d = sqrt(1- sqr((0.5f-(float)i/height)*2));

		for(int j=0; j < radius; j++)
			vertexes->data[i*radius + j].set(
				d*(float)cos((float)j/radius*M_PI*2.0f)*0.5f,
				d*(float)sin((float)j/radius*M_PI*2.0f)*0.5f,
				(float)i/height - 0.5f);

		for(int j=0; j < radius; j++)
			texCoords0->data[i*radius + j].set(
				(float)j/radius,
				(float)i/height);

		for(int j=0; j < radius; j++)
			texCoords1->data[i*radius + j].set(
				(float)i/height,
				(float)j/radius);
	}
	
	faces = new FaceMaterials(1);

	faces->at(0).material = new Material("/sphere/sphere");
	faces->at(0).triangles = new Triangles;
	faces->at(0).triangles->data.resize(height*radius*2);
	faces->at(0).triangles->setId(objectIdGenerator.genUid());
	faces->at(0).triangles->setChanged(true);
	
	for(int i=0; i < height; i++)
 		for(int j=0; j < radius; j++)
		{
			faces->at(0).triangles->data[(i*radius+j)*2 + 0].set(
				i*radius + j, 
				i*radius + (j + 1)%radius, 
				(i+1)*radius + j);

			faces->at(0).triangles->data[(i*radius+j)*2 + 1].set(
				i*radius + (j + 1)%radius, 
				(i+1)*radius + (j + 1)%radius, 
				(i+1)*radius + j);

		}
}

Sphere::~Sphere()
{
	delete vertexes;
	delete normals;
	delete texCoords0;
	delete texCoords1;
	for(FaceMaterials::iterator it = faces->begin(); it != faces->end(); it++)
	{
		delete it->triangles;
	}
	delete faces;

	// todo delete faces
}


void Sphere::ProcessGraph()
{
	for(unsigned int i=0; i < vertexes->data.size(); i++)
	{
		vertexes->data[i] += v3(prand()*0.001f, prand()*0.001f, prand()*0.001f);

		vertexes->data[i] += vertexes->data[i]*v3(0,0,1)*0.001f*frand();
	}

	if(frand()<0.1f && !faces->at(0).triangles->data.empty())
	{
		DeleteTriangle((int)floor((frand()*(faces->at(0).triangles->data.size()-1))));
	}
	for(unsigned int i = 0; i<normals->data.size(); i++)
	{
		normals->data[i] = vertexes->data[i].getNormalized();
	}
}

void Sphere::DeleteTriangle(int n)
{
	faces->at(0).triangles->data[n] = faces->at(0).triangles->data.back();
	faces->at(0).triangles->data.pop_back();
	// TODO delete vertexes
}
