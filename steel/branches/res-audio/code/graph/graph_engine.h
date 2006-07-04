/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		����������� ������. ������� ���������� ������ � ���� 3d-mesh
		� ������� ������������ ������ (OpenGL_Engine) ����������
		������������ ����� ������� �� ������, �������� �������������
		���������, �������������� ����. ������.
 ************************************************************/

#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../res/conf/conf.h"
#include "graph_interface.h"

class GraphEngine;

class Camera
{
public:
    v3 eye, center, up;

public:
    void seteye(const v3 &EYE) { eye = EYE; }
    void setcenter(const v3 &CENTER) { center = CENTER; }
	void setup(const v3 &EYE, const v3 &DIR);

	Camera();
};

class GraphEngine: public Engine
{
protected:
	struct DrawElement // ��������� ������������� ������ ���������
	{
		GraphInterface *object;
		Vertexes	*vertex;
		Triangles	*triangle;
		Normals		*normal;

		Material	*material;
		GLines		*lines;

		matrix34	matrix;
		aabb		frame;
		bool		blend; // true if blending
		coord		distance; // ���������� �� ������

		bool	operator < (const DrawElement &sec) const { return distance > sec.distance; }
	};

	// data, to store collected information
	steel::vector<DrawElement> element;
	steel::vector<GraphInterface*> objects;
	Lights light;

public:
	struct TotalInfo
	{
		int vertex, triangle, object, global;
	} total;

public:
	Camera camera;
	virtual void processCamera() = 0;
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object);
	virtual bool remove(GraphInterface *object);
	virtual bool prepare(GraphInterface *object, matrix34 matrix = matrix34::getIdentity());

	// Draw colelcted information. May be called few times without recollection information
	virtual bool process() = 0; // Override OpenGL, D3D, ...
	virtual bool isVisible(aabb box) = 0;
	// Clear collected information
	bool clear();
};

#endif
