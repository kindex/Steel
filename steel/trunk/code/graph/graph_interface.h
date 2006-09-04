/*id*********************************************************
	Unit: GraphEngine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Bond [Andrey Bondarenko]
	License:
		Steel Engine License
	Description:
		����������� ������ - ��������� ��������� �������
 ************************************************************/

#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include "../engine.h"

#include "types.h"

#include "../res/material/material.h"

// �������� + ������������, � ������� �� ���������
struct FaceMaterial
{
	Material		*material;
	Triangles		*triangles;
	std::string		name;
};

typedef steel::vector<FaceMaterial>	FaceMaterials;


/*
��������� ��� ���� �������� � �������, ������� ������ ���������� ����� 
������������ ��������. ������� ���������. �������� ���������� ������
������������ � ���� ��������. � ������ (aabb) ������ ����� � ������� ��������.
��� ����� ��� �������� ��������� ��������� ��������.
����������� ���������, � ������� �� �����������, ������ ��������� ����������.
������� ������, ������ ������ �������.
�� ����, ������ ����� � �� ����� ���������� � ����� �� ������, � ������������ �� �� ����,
��������, ��� ����-������, ������� ������
*/

class GraphObject;
typedef steel::svector<GraphObject*> GraphObjectList;

class GraphObject: public Interface
{
protected:
	int gInjectedCount;
public:
// *** Common ***
	GraphObject(): gInjectedCount(0) {}
	// ������ �����
	/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	// ���������� ���������� �����
	virtual int getGraphChildrenCount(void) { return 0; } 
	// ������ � ��������� �������
	virtual GraphObject* getGraphChildren(int number) { return NULL; }

	// ��������������� ����� ����������� � ������ ���������� 
	virtual bool GraphBeforeInject() { gInjectedCount++; return true;}
	// ����� �������� �� ������ ���������� ��������� afterRemove
	virtual void GraphAfterRemove() {gInjectedCount--;}

	// ���������� ����� ������ ��������� ���������. ������ ���� ��������� ������ ����� ������ ��������� ���� ���������
	virtual	void ProcessGraph(const GraphEngineInfo &info) {}

// *** Configuration ***

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
//	virtual aabb getFrame() { return = 0; // AABB of object

	// ��������� ������� ��� 3DS � OpenGL
	// ��������� ������� ���������� ������ �� ������� ������ (NULL if none), � ������ 
	// �������� �� ������� ���� ������ �� ���������� ������ ���� �������
	// ��� ������ cleanup
	virtual Vertexes*	getVertexes() { return NULL;} // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
	virtual Normals*	getNormals() { return NULL;} // ������ �������� � ��������

	virtual GLines*		getLines() { return NULL;} // ������� ������ ��� ����� � ����� ����� (for debug)

	// ������ �������� ������, ������� �������� ������������ (�����) + ���������
	virtual FaceMaterials* getFaceMaterials() { return NULL;}
	virtual TexCoords*	getTexCoords(int texNumber) { return NULL;}

	virtual Lights*		getLights() { return NULL;}
};

#endif

