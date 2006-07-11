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
	virtual int GetGraphChildrenCount(void) { return 0; } 
	// ������ � ��������� �������
	virtual GraphObject* GetGraphChildren(int number) { return NULL; }

	// ��������������� ����� ����������� � ������ ���������� 
	virtual bool GraphBeforeInject() { return true;}
	// ����� �������� �� ������ ���������� ��������� afterRemove
	virtual void GraphAfterRemove() {}

	// ���������� ����� ������ ��������� ���������. ������ ���� ��������� ������ ����� ������ ��������� ���� ���������
	virtual	void ProcessGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime, v3	cameraEye, v3 cameraDirection) {}

// *** Configuration ***

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
//	virtual aabb getFrame() { return = 0; // AABB of object

	// ��������� ������� ��� 3DS � OpenGL
	// ��������� ������� ���������� ������ �� ������� ������ (NULL if none), � ������ 
	// �������� �� ������� ���� ������ �� ���������� ������ ���� �������
	// ��� ������ cleanup
	virtual Vertexes*	GetVertexes() { return NULL;} // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
	virtual Normals*	GetNormals() { return NULL;} // ������ �������� � ��������

	virtual GLines*		GetLines() { return NULL;} // ������� ������ ��� ����� � ����� ����� (for debug)

	// ������ �������� ������, ������� �������� ������������ (�����) + ���������
	virtual FaceMaterials* GetFaceMaterials() { return NULL;}
	virtual TexCoords*	GetTexCoords(int texNumber) { return NULL;}

	virtual Lights*		GetLights() { return NULL;}
};

#endif

