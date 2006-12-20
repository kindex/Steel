/*id*********************************************************
	File: res/model/model.h
	Unit: res/model
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для хранения модели в виде многогранника
 ************************************************************/

#ifndef __RES__MODEL_MODEL_H
#define __RES__MODEL_MODEL_H

#include "../../steel.h"
#include "../res.h"
#include "../../math/vector2d.h"
#include "../../math/vector3d.h"

#include "../../graph/graph_interface.h"

 //3D Object as triangles mesh
class Model: public Res
{
public:
	float calculateVolume(void); // вычислить объём
	float getVolume(void); // вычислить объём

	const Vertexes *getVertexes(void) const { return &vertexes; }
	const Normals *getNormals(void) const { return &normals; }
	const FaceMaterials *getFaceMaterials(void) const { return &faceMaterials; }
	const TexCoords	*getTexCoords(int mapNumber) const;

	friend class GraphObjectModel; // TEMP:

protected:
	AABB				frame;
    Vertexes			vertexes;    // Vertexes

    Triangles			triangleAll;        // Triangles

    TexCoords			texCoords; // Texture coordinates
    Normals				normals;    // Vertex normals
	FaceMaterials		faceMaterials;
	
	std::string			name;
	float				volume;

    Model(void): name(), volume(-1) { }
    void updateMaterial(void);
    void updateNormals(void);
	void updateAABB(void);
    void updateTangentSpace(void);
    void update(void)
    {
        updateNormals();
        updateMaterial();
        updateTangentSpace();
    }
    void updatetangentSpaceLight(void);
    void updateReflectCoords(void);
    void updateSphereMap(v3 &center);
    int duplicateVertex(int src, v3 newnormal);

	void generateNormals(void);
};

#endif
