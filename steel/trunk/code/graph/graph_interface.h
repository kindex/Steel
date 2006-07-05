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
public:
// *** Common ***
	// ������ �����
	/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	// ���������� ���������� �����
	virtual int getGraphChildrenCount(void) { return 0; } 
	// ������ � ��������� �������
	virtual GraphObject* getGraphChildren(int number) { return NULL; }

	// ��������������� ����� ����������� � ������ ���������� 
	virtual bool GraphBeforeInject() { return true;}
	// ����� �������� �� ������ ���������� ��������� afterRemove
	virtual void GraphAfterRemove() {}

	// ���������� ����� ������ ��������� ���������. ������ ���� ��������� ������ ����� ������ ��������� ���� ���������
	virtual	void processGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) {}

// *** Configuration ***

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getFrame() = 0; // AABB of object

	// ��������� ������� ��� 3DS � OpenGL
	// ��������� ������� ���������� ������ �� ������� ������ (NULL if none), � ������ 
	// �������� �� ������� ���� ������ �� ���������� ������ ���� �������
	// ��� ������ cleanup
	virtual bool cleanup() = 0; // ��� ������������� �������� 
	virtual Vertexes*	getVertexes() = 0; // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
	virtual Normals*	getNormals() = 0; // ������ �������� � ��������

	virtual GLines*	getLines() = 0; // ������� ������ ��� ����� � ����� ����� (for debug)

	// ������ �������� ������, ������� �������� ������������ (�����) + ���������
	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual TexCoords*	getTexCoords(int texNumber) = 0;

	virtual Lights*		getLights() = 0;

	virtual	void		processGraph(v3	cameraEye, v3 cameraDirection) = 0;
};

#endif

