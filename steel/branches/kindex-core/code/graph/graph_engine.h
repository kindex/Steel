#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../engine.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/geometry.h"

// RES
#include "../res/model/model.h"

/********************************* GRAPH **********************************************/




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
typedef vector<GraphInterface*> GraphInterfaceList;

class GraphInterface: public Interface
{
public:
/*	������ ��������� ������ ������� (��������). ��������, ��� ���� - ��� ����� � �������, � ��� ������� ��� ����� ���� ������� ����.*/
	virtual GraphInterfaceList getChildrens() = 0;

/*����� ����� ������ �������*/
	virtual v3 getPos() = 0; // return center of object, origin
/*������ - �������������, � ������� ���������� ������. ����� ���� ������, �� �� ������ ������������, ���������� ��������. ������ ����������� ������*/
	virtual aabb getFrame() = 0; // AABB of object
/*����� ������� ��� ��������� (��� ����� ��������)
 full shape for physics, graph (polygon, 3D mesh)
 quality = 0 - aabb
 quality = 1 - �� ����� ��������
 � ����������� �� ���������� � ������������������ ������.
 */
	//virtual GShape getGShape(float quality=1) = 0; 
	/*
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


class GraphEngine: public Engine
{
protected:
	// data, to store collected information
	vertexes	vertex;
	triangles	triangle;
public:
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object);
	// Draw colelcted information. May be called few times without recollection information
	bool process() = 0; // Override OpenGL, D3D, ...\
	// Clear collected information
	bool clear()
	{
		vertex.clear();
		triangle.clear();
		return true;
	}
};




#endif

