#include "graph_engine.h"



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
		Triangle r;
		r.a[0] = it->a[0] + start;
		r.a[1] = it->a[1] + start;
		r.a[2] = it->a[2] + start;
		triangle.push_back(r);
	}

//		v3 pos = o.getPos();
	return true;
}

