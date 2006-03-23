#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include "../engine.h"
#include "../math/geometry.h"
#include "../math/matrix4x4.h"


// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];
};

struct Light
{
	v3 pos;
	float intensivity, range;
};

typedef std::vector<v3>			Vertexes;
typedef std::vector<v3>			Normals;
typedef std::vector<Triangle>	Triangles;
typedef std::vector<v2>			MapCoords;
typedef std::vector<Light>		Lights;
typedef std::map<std::string, std::vector<int> >  FaceMaterials;

struct color_24
{
    unsigned char r,g,b;
};

struct color_f
{
    float r,g,b;
    color_f() {}
    color_f(float R, float G, float B): r(R), g(G), b(B) {}
};


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


Sprites ?
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
	virtual Triangles*	getTriangles() = 0; 

	virtual MapCoords*	getMapCoords() = 0; 
	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual Lights* getLights() = 0;

/*?? ����� ����� ������ �������. ������������ ���� ����� �������� ������ (����������������)*/
	virtual MATRIX4X4 getMatrix() = 0;
};

#endif

