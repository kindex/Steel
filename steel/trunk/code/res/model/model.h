/*id*********************************************************
    Unit: Res-Modele [3D mesh]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс для хранения модели в виде многогранника
 ************************************************************/

#ifndef __RES__MODEL_MODEL_H
#define __RES__MODEL_MODEL_H

#include "../res.h"
#include "../../math/vector2d.h"
#include "../../math/vector3d.h"

#include "../../graph/graph_interface.h"

 //3D Object as triangles mesh
class Model: public Res
{
public:
	aabb				frame;
    Vertexes			vertex;    // Vertexes

    Triangles			triangleAll;        // Triangles

    TexCoords			texCoords; // Texture coordinates
    Normals				normal;    // Vertex normals
	FaceMaterials		faceMaterial;
	float				volume;

    std::string name;

    Model(): name(), volume(-1) { }
    void updateMaterial();
    void updateNormals();
	void updateAABB();
    void updateTangentSpace();
    void update()
    {
        updateNormals();
        updateMaterial();
        updateTangentSpace();
    }
    void updatetangentSpaceLight();
    void updateReflectCoords();
    void updateSphereMap(v3 &center);
    int duplicateVertex(int src, v3 newnormal);

	void generateNormals();
	bool unload()
	{
		return true;
	}
	TexCoords	*getTexCoords(int mapNumber);
	float calculateVolume(); // вычислить объём
	float getVolume(); // вычислить объём
};

#endif
