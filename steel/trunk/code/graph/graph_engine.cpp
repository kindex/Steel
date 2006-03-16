#include "graph_engine.h"


WindowSettings::WindowSettings() 
{ 
	title	= "Steel Demo"; 
	width	= 650; 
	height	= width*3/4; 
	bpp		= 32;
}

bool GraphEngine::inject(GraphInterface *object)
{
//		Engine::inject(object);
	GraphInterface &o = *(GraphInterface*)object;
	GraphInterfaceList children = o.getChildrens();
	for(GraphInterfaceList::iterator it=children.begin();
		it != children.end();
		it++)
		if(!inject(*it)) return false;

	int start = vertex.size();
	vertexes *v = object->getVertexes();
	for(vertexes::iterator it=v->begin(); it != v->end(); it++)
		vertex.push_back(*it);

	triangles *t = object->getTriangles();
	for(triangles::iterator it=t->begin(); it != t->end(); it++)
	{
		triangle.push_back(it->a[0] + start);
		triangle.push_back(it->a[1] + start);
		triangle.push_back(it->a[2] + start);
	}

//		v3 pos = o.getPos();
	return true;
}

