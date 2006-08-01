/*id*********************************************************
    Unit: objects/combiner
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		����������� ����������� � ���������� ��������.
		��������� � ����� ������� (combiner) ���������� ��������
		������ ����������� ������� � ������ ������������.
 ************************************************************/

#include "game_obj.h"

class Combiner: public GameObj
{
protected:
	GraphObject *graph;
	PhysicObject *physic;
public:
	Combiner(): graph(NULL), physic(NULL) {}

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
	bool GraphBeforeInject()
	{ 
		if(graph) 
			if(!graph->GraphBeforeInject()) return false;
		return GameObj::GraphBeforeInject();
	}

	void GraphAfterRemove()
	{
		GameObj::GraphAfterRemove();
		if(graph) graph->GraphAfterRemove();
	}

	void ProcessGraph(const GraphEngineInfo &info)
	{
		if(graph) graph->ProcessGraph(info);
	}

	Vertexes*	getVertexes() { return graph?graph->getVertexes():NULL; } 
	Normals*	getNormals() { return graph?graph->getNormals():NULL; } // ������ �������� � ��������

	GLines*		getLines() { return graph?graph->getLines():NULL;} // ������� ������ ��� ����� � ����� ����� (for debug)

	// ������ �������� ������, ������� �������� ������������ (�����) + ���������
	FaceMaterials* getFaceMaterials() { return graph?graph->getFaceMaterials():NULL;}
	TexCoords*	getTexCoords(int texNumber) { return graph?graph->getTexCoords(texNumber):NULL;}

	Lights*		getLights() { return graph?graph->getLights():NULL;}

// ********** PHYSIC *************
// TODO
};
