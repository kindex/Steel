/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    Licence:
        Только для Division
    Description:
		Графический джижок. Получет трехмерный объект в виде 3d-mesh
		и потомки графического движка (OpenGL_Engine) занимаются
		отображением этого объекта на экране, расчётом динамического
		освещения, инициализацией граф. режима.
 ************************************************************/

#include "graph_engine.h"

using namespace std;


bool GraphEngine::inject(GraphInterface *object, matrix4 matrix)
{
	if(object == NULL) return false;
	aabb frame = object->getFrame();

	objects.push_back(object);
	total.object++;
	matrix4 cur_matrix, new_matrix;

	cur_matrix = object->getMatrix();
	new_matrix = matrix*cur_matrix;

/* TODO: сюда надо поставить проверку, находится ли frame
внутри пирамиды, которую образует угол обзора камеры.
Если не попадает, то откидываем этот объект и всех его потомков
*/
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

	if(v != NULL)
		total.vertex += v->size();

	if(t != NULL)
	for(FaceMaterials::iterator it = m->begin(); it != m->end(); it++)
	{
		int c = element.size();
		element.resize(c+1);

		element[c].material = it->first;

// TODO	it->secondm olny tringles from this material

		element[c].triangle = new Triangles;

		int s = it->second.size();
		element[c].triangle->clear();

		for(int i=0; i<s; i++)
			element[c].triangle->push_back(t->operator [](it->second[i]));

		element[c].vertex = v;
		element[c].mapcoord = object->getMapCoords();
		element[c].matrix = new_matrix;
		element[c].normal = n;
		element[c].frame = frame;

		total.triangle += s;
	}

//		v3 pos = o.getPos();
	return true;
}

bool GraphEngine::clear()
{
//	for(vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		//it->

	for(vector<DrawElement>::iterator it = element.begin(); it != element.end(); it++)
		delete it->triangle;

	for(vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->cleanup();

	objects.clear();
	element.clear();
	light.clear();

	total.vertex = 0;
	total.triangle = 0;
	total.object = 0;


	return true;
}


