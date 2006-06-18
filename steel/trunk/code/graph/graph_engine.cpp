/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		Графический джижок. Получет трехмерный объект в виде 3d-mesh
		и потомки графического движка (OpenGL_Engine) занимаются
		отображением этого объекта на экране, расчётом динамического
		освещения, инициализацией граф. режима.
 ************************************************************/

#include "graph_engine.h"

bool GraphEngine::inject(GraphInterface *object)
{
	if(object == NULL) return false;

	objects.push_back(object);
	total.global++;
	return true;
}

bool GraphEngine::remove(GraphInterface *object)
{
	for(steel::vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		if(*it == object)
		{
			total.global--;
			object->cleanup();
			objects.erase(it);
			return true;
		}
	return false;
}


bool GraphEngine::prepare(GraphInterface *object, matrix34 parent_matrix)
{
	if(object == NULL) return false;
	total.object++;

	matrix34 object_matrix = object->getPosition(); // global 

	PositionKind::PositionKind pos = object->getPositionKind();
	if(pos == PositionKind::local)
	{
		object_matrix = parent_matrix*object_matrix;
		object->processGraph(parent_matrix.getInverse()*camera.eye, parent_matrix.getInverse()*(camera.center-camera.eye));
	}
	else if(pos == PositionKind::global) 
		object->processGraph(camera.eye, (camera.center-camera.eye));
	else
		return false;

	aabb frame = object->getFrame();
	frame.mul(object_matrix);

/*проверка, находится ли frame
внутри пирамиды, которую образует угол обзора камеры.
Если не попадает, то откидываем этот объект и всех его потомков
*/
	if(!isVisible(frame)) return false;
	
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
	GLines		*glines = object->getLines();

	if(l != NULL)
	{
		for(Lights::iterator it = l->begin(); it != l->end(); it++)
		{
			light.push_back(*it);
			light[light.size()-1].pos = object_matrix*v3(0,0,0);
		}
	}

	if(v != NULL)
		total.vertex += v->data.size();

	if(m != NULL)
	for(FaceMaterials::iterator it = (*m).begin(); it != (*m).end(); it++)
	{
		int c = element.size();
		element.resize(c+1);
		DrawElement &e = element[c];

		e.object = object;

		e.material = (*it).material;
		e.triangle = it->triangles;
		e.vertex = v;
		e.matrix = object_matrix;
		e.normal = n;
		e.frame = frame;
		e.blend = element[c].material->blend;
		e.lines = NULL;

		total.triangle += element[c].triangle->data.size();
	}

	if(glines)
	{
		int c = element.size();
		element.resize(c+1);
		DrawElement &e = element[c];

		e.object = object;

		e.material = NULL;
		e.triangle = NULL;
		e.vertex = v;
		e.matrix = object_matrix;
		e.normal = NULL;
		e.frame = frame;
		e.lines = glines;
		e.blend = false;
	}

	return true;
}

bool GraphEngine::clear()
{
	for(steel::vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->cleanup();

	objects.clear();

	total.global = 0;

	return true;
}


void Camera::setup(const v3 &EYE, const v3 &DIR)
{
	eye = EYE;
	center = EYE + DIR;
}

Camera::Camera(): 
		up(v3(0.0, 0.0, 1.0)), 
		eye(10.0, 10.0, 1.0), 
		center(v3(0.0, 0.0, 0.0)) 
{
}
