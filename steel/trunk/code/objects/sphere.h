/*id*********************************************************
    Unit: objects/sphere
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Многогранник с изменяемой формой и количеством полигонов
 ************************************************************/

#ifndef __OBJECTS_SPHERE_H
#define __OBJECTS_SPHERE_H

#include "../graph/graph_interface.h"

class Sphere: public GraphObject
{
protected:
	Vertexes *vertexes;
	FaceMaterials *faces;
	TexCoords *texCoords;
public:
	Sphere();

	TexCoords*	getTexCoords(int texNumber) { return texCoords;}
	Vertexes*	getVertexes() { return vertexes;}
	FaceMaterials* getFaceMaterials() { return faces;}
	void ProcessGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime, v3	cameraEye, v3 cameraDirection);
};

#endif
