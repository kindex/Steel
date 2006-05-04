/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
		* Bond [Andrey Bondarenko]
    Licence:
        ������ ��� Division
    Description:
		����������� ������ - ��������� ��������� �������
 ************************************************************/

#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include "../engine.h"
#include "../math/geometry.h"
#include "../math/maths.h"
#include "../math/vector3d.h"

#include "../res/material/material.h"

struct Light
{
	v3 pos;
	float intensivity, range;
};

/*
Sprite, Billboard. �������, ������� ������ ���������� ��������� ������.
*/

typedef std::vector<Light>		Lights;

class MapCoord: public BufferedElement
{
public:
	std::vector<v2>	data;

//	kind : 1d, 2d, 3d
};

typedef Vertexes	Normals;


typedef MapCoord MapCoords;
//typedef std::vector<v2> MapCoords;


// �������� + ������������, � ������� �� ���������
struct FaceMaterial
{
	Material		*material;
	Triangles		triangles;
	std::string		name;
};

typedef std::vector<FaceMaterial>	FaceMaterials;


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

class GraphInterface;
typedef std::vector<GraphInterface*> GraphInterfaceList;

class GraphInterface: public Interface
{
public:
/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	virtual GraphInterfaceList getChildrens() = 0;

/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getFrame() = 0; // AABB of object
/*����� ������� ��� ��������� (��� ����� ��������)
 full shape for physics, graph (polygon, 3D mesh)
 quality = 0 - aabb
 quality = 1 - �� ����� ��������
 � ����������� �� ���������� � ������������������ ������.


NURBS ?
Decals ?

��������� ��������������� ��������? no
Video textures (avi, camera)

*/
	// ��������� ������� ��� 3DS � OpenGL
	// ��������� ������� ���������� ������ �� ������� ������ (NULL if none), � ������ 
	// �������� �� ������� ���� ������ �� ���������� ������ ���� �������
	// ��� ������ cleanup
	virtual bool cleanup() = 0; // ��� ������������� �������� 
	virtual Vertexes*	getVertexes() = 0; // ������ ������ (���������� ������������ ������� getMatrix() � ���� ������ �������)
	virtual Normals*	getNormals() = 0; // ������ �������� � ��������

	// ������ �������� ������, ������� �������� ������������ (�����)
//	virtual Triangles*	getTriangles() = 0; 

	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual MapCoords*	getMapCoords() = 0;

	virtual Lights*		getLights() = 0;

	virtual	uid			getId() = 0;
	virtual	void		processGraph(v3	cameraEye, v3 cameraDirection) = 0;
};

#endif

