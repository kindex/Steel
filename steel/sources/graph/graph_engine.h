/*id*********************************************************
	File: graph/graph_engine.h
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

#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../steel.h"
#include "graph_interface.h"
#include "../engine/interface.h"
#include "../engine/game_object.h"

class GraphEngine: public Engine, public GraphInterface
{
public:

	struct GraphTotalInfo
	{
		int vertexCount;
		int triangleCount;
		int objectCount;
		int globalObjectCount;
	} total;

	// Collect information about object: how to render it
	virtual bool inject(GameObject *object);
	virtual bool remove(GameObject *object);
	
	virtual bool process(IN const ProcessInfo&) abstract;

	// Draw colelcted information. May be called few times without recollection information
	virtual bool isVisible(AABB box) abstract;
	// Clear collected information
	bool clear();

protected:
	pvector<GameObject*>	objects;
};

#endif
