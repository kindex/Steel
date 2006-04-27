#ifndef __RES__MODEL_MODEL_H
#define __RES__MODEL_MODEL_H

#include "../res.h"
#include "../../math/vector2d.h"
#include "../../math/vector3d.h"

#include "../../graph/graph_interface.h"


 //3D Object as triangles mesh, Abstract class
class Model: public Res
{
public:
	aabb				frame;
    Vertexes			vertex;    // Vertexes

    Triangles			triangleAll;        // Triangles

    MapCoords			mapCoords; // Texture coordinates
    std::vector<v3>		normal;    // Vertex normals
	FaceMaterials		faceMaterial;

    // sTangent - vektor v storonu uveli4enija S teksturnoj koordinati (vpravo)
    // tangentSpaceLight - vektor ot ver6uni do to4ki osveshenija v TBN (Tangent, Binormal, Normal) prostrastve (sTangent, tTangent, normal) - tangent space http://www.paulsprojects.net/tutorials/tutorials.html
    //( Sx Sy Sz )
    //( Tx Ty Tz ) - matrix to convert from object space to tangent space
    //( Nx Ny Nz )
    // links: http://www.ati.com/developer/sdk/RADEONSDK/Html/Tutorials/RADEONBumpMap.html
 
//    mat_id mat;  

    std::string name;

    Model(): name() { }
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
	MapCoords	*getMapCoords();
};


#endif
