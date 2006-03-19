#include "graph_engine.h"


WindowSettings::WindowSettings() 
{ 
	title	= "Steel Engine"; 
	left	= 0;
	top		= 0;
	width	= 650; 
	height	= width*3/4; 
	bpp		= 32;

	fullscreen = false;
}

bool GraphEngine::inject(GraphInterface *object)
{
//		Engine::inject(object);
	objects.push_back(object);

	GraphInterface &o = *(GraphInterface*)object;
	GraphInterfaceList children = o.getChildrens();
	for(GraphInterfaceList::iterator it=children.begin();
		it != children.end();
		it++)
		if(!inject(*it)) return false;

	FaceMaterials* m = object->getFaceMaterials();
	Vertexes *v = object->getVertexes();
	Triangles *t = object->getTriangles();

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

	return true;
}
