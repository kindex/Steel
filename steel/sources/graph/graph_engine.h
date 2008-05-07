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
#include <set>

class GameObject;
struct Line;
struct Plane;

class GraphEngine : public GraphInterface
{
public:
	struct GraphTotalInfo
	{
		size_t vertexCount;
		size_t faceCount;
		size_t batchCount;
		size_t globalObjectCount;
		size_t lightCount;
	} total;

	// Collect information about object: how to render it
	virtual bool inject(GameObject* object);
	virtual bool remove(GameObject* object);
	
	virtual bool process(IN const ProcessInfo&) abstract;
	virtual bool setCaption(const std::string& caption) abstract;

	// Draw colelcted information. May be called few times without recollection information
	virtual bool isVisible(AABB box) abstract;
	// Clear collected information
	virtual bool clear();
	virtual bool findCollision(const Line&                        lineSegment,
                               const std::set<const GameObject*>& exclude_objects,
                               OUT GameObject*&                   crossing_object,
                               OUT v3&                            crossing_position,
                               OUT Plane&                         crossing_triangle) const abstract;

protected:
	pvector<GameObject*>	objects;
};

#endif
