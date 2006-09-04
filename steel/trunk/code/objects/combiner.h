/*id*********************************************************
	File: objects/combiner.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Компоновщик графических и физических объектов.
		Позволяет в одном объекте (combiner) объединять свойства
		одного физического объекта и одного графического.
 ************************************************************/

#include "../steel.h"
#include "game_obj.h"

class Combiner: public GameObj
{
protected:
	GraphObject *graph;
	PhysicObject *physic;
public:
	Combiner(void): graph(NULL), physic(NULL) {}

	virtual void setGraphObject(GraphObject *_graph)
	{ // TODO check error (if graph was already set)
		graph = _graph;
	}

	ModificationTime getModificationTime(void) 
	{ 
		if(graph) 
			return graph->getModificationTime();
		else 
			return 0;
	}

// ********** GRAPH *********
	bool GraphBeforeInject(void)
	{ 
		if(graph) 
			if(!graph->GraphBeforeInject()) return false;
		return GameObj::GraphBeforeInject();
	}

	void GraphAfterRemove(void)
	{
		GameObj::GraphAfterRemove();
		if(graph) graph->GraphAfterRemove();
	}

	void ProcessGraph(const GraphEngineInfo &info)
	{
		if(graph) graph->ProcessGraph(info);
	}

	Vertexes*	getVertexes(void) { return graph?graph->getVertexes():NULL; } 
	Normals*	getNormals(void) { return graph?graph->getNormals():NULL; } // список нормалей в вершинам

	GLines*		getLines(void) { return graph?graph->getLines():NULL;} // индексы вершин для линий и цвета линий (for debug)

	// массив индексов вершин, которые образуют треугольники (грани) + материалы
	FaceMaterials* getFaceMaterials(void) { return graph?graph->getFaceMaterials():NULL;}
	TexCoords*	getTexCoords(int texNumber) { return graph?graph->getTexCoords(texNumber):NULL;}

	Lights*		getLights(void) { return graph?graph->getLights():NULL;}

// ********** PHYSIC *************
// TODO
};
