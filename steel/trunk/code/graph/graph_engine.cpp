/*id*********************************************************
	File: graph/graph_engine.cpp
	Unit: graph engine
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

#include "../steel.h"
#include "graph_engine.h"

bool GraphEngine::inject(GraphObject *object)
{
	// если объект не хочет добавляться
	if(!object->GraphBeforeInject()) return false;
	// кешируем объект
	if(!makeStorageForObject(object)) return false;
	makeStorageForChildren(object);
	// список глобальных объектов
	objects.push_back(object);

	return true;
}


bool GraphEngine::clear()
{
	while(!objects.empty())
	{
		remove(objects.back());
	}

	return true;
}

bool GraphEngine::remove(GraphObject *object)
{
	deleteStorageForChildren(findSid(object->getId()));
	deleteStorageForObject(findSid(object->getId()));
	
	for(steel::vector<GraphObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		if(*it == object)
		{
			objects.erase(it);
			break;
		}
	object->GraphAfterRemove();

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
