#include "sphere.h"
#include "../res/res_main.h"

Sphere::Sphere()
{
	int height = 24;
	int radius = 32;

	vertexes = new Vertexes;
	vertexes->setId(objectIdGenerator.genUid());
	vertexes->setChanged(true);
	vertexes->data.resize( (height+1)*radius);

	texCoords = new TexCoords;
	texCoords->setId(objectIdGenerator.genUid());
	texCoords->setChanged(true);
	texCoords->data.resize( (height+1)*radius);

	for(int i=0; i < height+1; i++)
	{
		float d = sqrt(1- sqr((0.5f-(float)i/height)*2));

		for(int j=0; j < radius; j++)
			vertexes->data[i*radius + j].set(
				d*(float)cos((float)j/radius*M_PI*2.0f)*0.5f,
				d*(float)sin((float)j/radius*M_PI*2.0f)*0.5f,
				(float)i/height);

		for(int j=0; j < radius; j++)
			texCoords->data[i*radius + j].set(
				(float)j/radius,
				(float)i/height);
	}

	
	faces = new FaceMaterials(1);

	faces->at(0).material = resMaterial.add("sphere/sphere");

	faces->at(0).triangles = new Triangles;
	faces->at(0).triangles->data.resize(height*radius*2);
	
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

	faces->at(0).triangles->setId(objectIdGenerator.genUid());
	faces->at(0).triangles->setChanged(true);
}

void Sphere::ProcessGraph(const GraphEngineInfo &info)
{
	for(unsigned int i=0; i < vertexes->data.size(); i++)
	{
		vertexes->data[i] += v3(prand()*0.001f, prand()*0.001f, prand()*0.001f);

		vertexes->data[i] += vertexes->data[i]*v3(0,0,1)*0.001f*frand();
	}

	if(frand()<0.1f && !faces->at(0).triangles->data.empty())
	{
		DeleteTriangle((int)(frand()*faces->at(0).triangles->data.size()));
	}
}

void Sphere::DeleteTriangle(int n)
{
	faces->at(0).triangles->data[n] = faces->at(0).triangles->data.back();
	faces->at(0).triangles->data.pop_back();
	// TODO delete vertexes
}
