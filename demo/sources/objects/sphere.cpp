/*id*********************************************************
	File: objects/sphere.cpp
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

#include <steel.h>
#include "sphere.h"
#include <res/res_main.h>
#include <graph/material.h>

Sphere::Sphere():
	vertexes(NULL),
	normals(NULL),
	faces(NULL),
	texCoords0(NULL),
	texCoords1(NULL),
	graphEngine(NULL)
{}



void Sphere::bindEngine(Engine& engine, IN const InterfaceId id)
{
	GraphEngine& gengine = *static_cast<GraphEngine*>(&engine);

	gengine.setPositionKind(POSITION_GLOBAL);
	gengine.setPosition(ObjectPosition::getIdentity());
}

bool Sphere::updateInformation(Engine& engine, IN const InterfaceId id)
{
	GraphEngine& gengine = *static_cast<GraphEngine*>(&engine);

	gengine.setPosition(position);
	gengine.setVertexes(vertexes);
	gengine.setNormals(normals);
	gengine.setFaceMaterials(faces);
	gengine.setTexCoordsCount(2);
	gengine.setTexCoords(0, texCoords0);
	gengine.setTexCoords(1, texCoords1);
	return true;
}


void Sphere::createSphere()
{
	vertexes = new VertexVector;
	vertexes->setId(objectIdGenerator.genUid());
	vertexes->setChanged(true);
	vertexes->resize( (height+1)*radius);

	normals = new Normals;
	normals->setId(objectIdGenerator.genUid());
	normals->setChanged(true);
	normals->resize( vertexes->size() );

	texCoords0 = new TexCoords;
	texCoords0->setId(objectIdGenerator.genUid());
	texCoords0->setChanged(false);
	texCoords0->resize( (height+1)*radius);

	texCoords1 = new TexCoords;
	texCoords1->setId(objectIdGenerator.genUid());
	texCoords1->setChanged(false);
	texCoords1->resize( (height+1)*radius);

	for (int i=0; i < height+1; i++)
	{
		float d = sqrt(1- sqr((0.5f-(float)i/height)*2));

		for (int j=0; j < radius; j++)
		{
			vertexes->at(i*radius + j).set(
				d*(float)cos((float)j/radius*M_PI*2.0f)*0.5f,
				d*(float)sin((float)j/radius*M_PI*2.0f)*0.5f,
				(float)i/height - 0.5f);
		}

		for (int j=0; j < radius; j++)
		{
			texCoords0->at(i*radius + j).set(
				(float)j/radius,
				(float)i/height);
		}

		for (int j=0; j < radius; j++)
		{
			texCoords1->at(i*radius + j).set(
				(float)i/height,
				(float)j/radius);
		}
	}
	
	faces = new FaceMaterialVector(1);
	FaceMaterial& m = faces->front();

	m.material = createMaterial(conf->find("material"));
	m.faces = new Faces;
	m.faces->triangles.resize(height*radius*2);
	m.faces->triangles.setId(objectIdGenerator.genUid());
	m.faces->triangles.setChanged(true);
	
	for (int i=0; i < height; i++)
 		for (int j=0; j < radius; j++)
		{
			m.faces->triangles[(i*radius+j)*2 + 0].set(
				i*radius + j, 
				i*radius + (j + 1)%radius, 
				(i+1)*radius + j);

			m.faces->triangles[(i*radius+j)*2 + 1].set(
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
	for(FaceMaterialVector::iterator it = faces->begin(); it != faces->end(); it++)
	{
		delete it->faces;
	}
	delete faces;

	// todo delete faces
}


void Sphere::process(IN const ProcessInfo&)
{
	for (size_t i=0; i < vertexes->size(); i++)
	{
		vertexes->at(i) += v3(prand()*0.001f, prand()*0.001f, prand()*0.001f);

		vertexes->at(i) += vertexes->at(i)*v3(0,0,1)*0.001f*frand();
	}

	if (frand() < 0.1f && !faces->front().faces->triangles.empty())
	{
		DeleteTriangle((int)floor((frand()*(faces->front().faces->triangles.size()-1))));
	}
	for (size_t i = 0; i<normals->size(); i++)
	{
		normals->at(i) = vertexes->at(i).getNormalized();
	}
}

void Sphere::DeleteTriangle(int n)
{
	faces->front().faces->triangles[n] = faces->front().faces->triangles.back();
	faces->front().faces->triangles.pop_back();
	// TODO delete vertexes
}

bool Sphere::InitFromConfig(Config& _conf)
{
	conf = &_conf;
	if (conf == NULL) return false;

	v3 origin =  conf->getv3("origin");
	position.loadIdentity();
	position.setTranslation(origin);

	size = conf->getf("size", 1.0f);
	height = conf->geti("height", 12);
	radius = conf->geti("radius", 16);

	createSphere();

	return true;
}
