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


bool GraphEngine::inject(GraphInterface *object)
{
	if(object == NULL) return false;

	objects.push_back(object);
	total.global++;
	return true;
}

bool GraphEngine::prepare(GraphInterface *object, matrix44 parent_matrix)
{
	if(object == NULL) return false;
	total.object++;

	matrix44 object_matrix = object->getMatrix(); // global 

	Interface::PositionKind pos = object->getPositionKind();
	if(pos == Interface::local)
	{
		object_matrix = parent_matrix*object_matrix;

		object->processGraph(parent_matrix.getInverse()*camera.eye, parent_matrix.getInverse()*(camera.center-camera.eye));
	}
	else if(pos == Interface::global) 
	{
		object->processGraph(camera.eye, (camera.center-camera.eye));
	}
	else
		return false;

	aabb frame = object->getFrame();
	frame.mul(object_matrix);


/* TODO: сюда надо поставить проверку, находится ли frame
внутри пирамиды, которую образует угол обзора камеры.
Если не попадает, то откидываем этот объект и всех его потомков
*/


	GraphInterface &o = *(GraphInterface*)object;
	GraphInterfaceList children = o.getChildrens();
	for(GraphInterfaceList::iterator it=children.begin();
		it != children.end();
		it++)
		if(!prepare(*it, object_matrix)) return false;

	FaceMaterials* m = object->getFaceMaterials();
	Vertexes	*v = object->getVertexes();
	Lights		*l = object->getLights();
	Normals		*n = object->getNormals();

	if(l != NULL)
	{
		for(Lights::iterator it = l->begin(); it != l->end(); it++)
		{
			light.push_back(*it);
			light[light.size()-1].pos = object_matrix.getCoords();
		}
	}

	if(v != NULL)
		total.vertex += v->data.size();

//	if(t != NULL)
	{
//		vector<bool>	usedTriangle(t->data.size(), false);
//		int totalUsed = 0;

	if(m != NULL)
	for(FaceMaterials::iterator it = (*m).begin(); it != (*m).end(); it++)
	{
		int c = element.size();
		element.resize(c+1);

		DrawElement &e = element[c];

		e.object = object;

		e.material = (*it).material;
		e.triangle = &it->triangles;

		int s = element[c].triangle->data.size();
		e.vertex = v;
		e.matrix = object_matrix;
		e.normal = n;
		e.frame = frame;
		e.blend = element[c].material->blend;

		total.triangle += s;
	}

	}

/*	Sprites		*s = object->getSprites();
	if(s != NULL)
	for(Sprites::iterator it = s->begin(); it != s->end(); it++)
	{
		int c = element.size();
		element.resize(c+1);

		matrix44 matrix = new_matrix;

		matrix.a[12] += it->pos.x;
		matrix.a[13] += it->pos.y;
		matrix.a[14] += it->pos.z;

		element[c].materialName = it->material;
		element[c].material = (Config*)res->get(Res::config, string("material/") + element[c].materialName);
		if(element[c].material == NULL)
		{
			alog.msg("error renderer res material", string("Material not found ")+ element[c].materialName);
			return false;
		}
		element[c].alpha = element[c].material->gets("color_mode") == "alpha";

		element[c].triangle = new Triangles(2);
		for(int i=0; i<3; i++)
		{
			element[c].triangle->operator[](0).a[i] = 3-i;
			element[c].triangle->operator[](1).a[i] = 3-(i+2)%4;
		}


		coord size = it->width;

		element[c].mapcoord = new MapCoords(4);
		element[c].mapcoord->operator [](0) = v2(0, 0);
		element[c].mapcoord->operator [](1) = v2(1, 0);
		element[c].mapcoord->operator [](2) = v2(1, 1);
		element[c].mapcoord->operator [](3) = v2(0, 1);

		element[c].matrix = matrix;

		element[c].normal = new Normals(4);
		for(int i=0; i<4; i++)
			element[c].normal->operator [](i) = dir;

		total.triangle += 2;
	}*/
	return true;
}

bool GraphEngine::clear()
{
//	for(vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		//it->

	for(vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->cleanup();

	objects.clear();

	total.global = 0;

	return true;
}


