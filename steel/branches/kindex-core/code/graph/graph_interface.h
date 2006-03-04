#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include <vector>
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

/*����� ����� ������ �������. ������������ ���� ����� �������� ������ (����������������)*/
	virtual v3 getPos() = 0; // return center of object, origin
/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getFrame() = 0; // AABB of object
/*����� ������� ��� ��������� (��� ����� ��������)
 full shape for physics, graph (polygon, 3D mesh)
 quality = 0 - aabb
 quality = 1 - �� ����� ��������
 � ����������� �� ���������� � ������������������ ������.

Vertex
Face
Material
Sprites ?
NURBS ?
Lights
Decals ?

��������� ��������������� ��������? no
Video textures (avi, camera)

*/
	// ��������� ������� ��� 3DS � OpenGL
	// ��������� ������� ���������� ������ �� ������� ������ (NULL if none), � ������ 
	// �������� �� ������� ���� ������ �� ���������� ������ ���� �������
	// ��� ������ cleanup
	virtual bool cleanup() = 0; // ��� ������������� �������� 
	virtual vertexes*	getVertexes() = 0; // ������ ������. 
	// ������ �������� ������, ������� �������� ������������ (�����)
	virtual triangles*	getTriangles() = 0; 
	// �������� ������ �����
//	virtual faces*	getFaces() = 0; // NULL if none

};

#endif

