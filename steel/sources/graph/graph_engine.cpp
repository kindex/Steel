/*id*********************************************************
	File: graph/graph_engine.cpp
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
#include "../engine/game_object.h"

bool GraphEngine::inject(GameObject* object)
{
    if (!object->supportsInterface(*this, INTERFACE_GRAPH))
	{
		return false;
	}

	// если объект не хочет добавляться
	if (!object->beforeInject(*this, INTERFACE_GRAPH))
	{
		return false;
	}

	// кешируем объект
	if (!makeShadowForObject(object, INTERFACE_GRAPH))
	{
		return false;
	}
	// список глобальных объектов
	objects.push_back(object);

	setCurrentObject(object, INTERFACE_GRAPH);
	object->bindEngine(*this, INTERFACE_GRAPH);

	return true;
}


bool GraphEngine::clear()
{
	while (!objects.empty())
	{
		remove(objects.back());
	}

	return true;
}

bool GraphEngine::remove(GameObject *object)
{
    deleteShadowForChildren(findSid(object->getObjectId(), INTERFACE_GRAPH), INTERFACE_GRAPH);
	deleteShadowForObject(findSid(object->getObjectId(), INTERFACE_GRAPH), INTERFACE_GRAPH);
	
	for EACH(pvector<GameObject*>, objects, it)
    {
		if (*it == object)
		{
			objects.erase(it);
			break;
		}
    }
	object->afterRemove(*this, INTERFACE_GRAPH);

	return true;
}


Camera::Camera(): 
	upVector(v3(0.0, 0.0, 1.0)), 
	position(0.0, 0.0, 0.0), 
	direction(v3(1.0, 0.0, 0.0))
{
}

Light::Light():
	constantAttenuation(0.0f),
	linearAttenuation(0.0f),
	quadraticAttenuation(0.0f),
	minDistance(0.1f),
	maxDistance(100.0f),
	k(1.0f)
{ 
	id = objectIdGenerator.genUid(); 
}
