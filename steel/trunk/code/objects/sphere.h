/*id*********************************************************
    Unit: objects/sphere
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		������������ � ���������� ������ � ����������� ���������
 ************************************************************/

#ifndef __OBJECTS_SPHERE_H
#define __OBJECTS_SPHERE_H

#include "../graph/graph_interface.h"

class Sphere: public GraphObject
{
protected:
	Vertexes *vertexes, *normals;
	FaceMaterials *faces;
	TexCoords *texCoords0, *texCoords1;
public:
	Sphere();
	~Sphere();

	TexCoords*	getTexCoords(int texNumber)
	{
		if(texNumber == 0)
			return texCoords0;
		else
			return texCoords1;
	}
	Vertexes*	getVertexes() { return vertexes;}
	Normals*	getNormals() { return normals;} // ������ �������� � ��������
	FaceMaterials* getFaceMaterials() { return faces;}
	void ProcessGraph(const GraphEngineInfo &info);

	void DeleteTriangle(int n);
};

#endif
