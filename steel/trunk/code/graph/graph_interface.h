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
#include "../math/aabb.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/vector2d.h"

#include "../res/material/material.h"

struct Light
{
	v3 pos;
	float intensivity, range;
};

typedef std::vector<Light>		Lights;

class TexCoord: public BufferedElement
{
public:
	std::vector<v2>	data;

//	kind : 2d only
};

class GLine // al-a triangle
{
public:
	int a[2]; // vertex indexes
	v4 color; // line color
	void set(const int A, const int B) { a[0] = A; a[1] = B; }
};

typedef std::vector<GLine> GLines;

typedef Vertexes	Normals;

typedef TexCoord TexCoords;
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

	virtual GLines*	getLines() = 0; // ������� ������ ��� ����� � ����� ����� (for debug)

	// ������ �������� ������, ������� �������� ������������ (�����)
//	virtual Triangles*	getTriangles() = 0; 

	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual TexCoords*	getTexCoords(int texNumber) = 0;

	virtual Lights*		getLights() = 0;

	virtual	uid			getId() = 0;
	virtual	void		processGraph(v3	cameraEye, v3 cameraDirection) = 0;
};

#endif

