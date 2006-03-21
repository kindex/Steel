#include "graph_engine.h"


WindowSettings::WindowSettings() 
{ 
	title	= "Steel Engine"; 
	left	= 0;
	top		= 0;
	width	= 700; 
	height	= width*3/4; 
	bpp		= 32;

	fullscreen = false;
}

bool GraphEngine::inject(GraphInterface *object, MATRIX4X4 matrix)
{
	objects.push_back(object);
	MATRIX4X4 cur_matrix, new_matrix;

	cur_matrix = object->getMatrix();
	new_matrix = matrix*cur_matrix;

	GraphInterface &o = *(GraphInterface*)object;
	GraphInterfaceList children = o.getChildrens();
	for(GraphInterfaceList::iterator it=children.begin();
		it != children.end();
		it++)
		if(!inject(*it, new_matrix)) return false;

	FaceMaterials* m = object->getFaceMaterials();
	Vertexes	*v = object->getVertexes();
	Triangles	*t = object->getTriangles();
	Lights		*l = object->getLights();
	Normals		*n = object->getNormals();

	if(l != NULL)
	{
		for(Lights::iterator it = l->begin(); it != l->end(); it++)
		{
			light.push_back(*it);
			light[light.size()-1].pos = new_matrix.getCoords();
		}
	}


	if(t != NULL)
	for(FaceMaterials::iterator it = m->begin(); it != m->end(); it++)
	{
		int c = elements.size();
		elements.resize(c+1);

		elements[c].material = it->first;

// TODO	it->secondm olny tringles from this material

		elements[c].triangle = new Triangles;

		int s = it->second.size();
		elements[c].triangle->clear();

		for(int i=0; i<s; i++)
			elements[c].triangle->push_back(t->operator [](it->second[i]));

		elements[c].vertex = v;
		elements[c].mapcoord = object->getMapCoords();
		elements[c].matrix = new_matrix;
		elements[c].normal = n;

	}

//		v3 pos = o.getPos();
	return true;
}

bool GraphEngine::clear()
{
//	for(vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		//it->

	for(vector<DrawElement>::iterator it = elements.begin(); it != elements.end(); it++)
		delete it->triangle;

	objects.clear();
	elements.clear();
	light.clear();

	return true;
}


