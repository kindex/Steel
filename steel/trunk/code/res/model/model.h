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
    std::vector<v3> vertex;    // Vertexes
    std::vector<Triangle> triangle;        // Triangles
//    vector <Faces> face;        // Triangles
    
    std::vector<v2> mapcoord; // Texture coordinates

    std::vector<v3> normal;    // Vertex normals
    // sTangent - vektor v storonu uveli4enija S teksturnoj koordinati (vpravo)
    // tangentSpaceLight - vektor ot ver6uni do to4ki osveshenija v TBN (Tangent, Binormal, Normal) prostrastve (sTangent, tTangent, normal) - tangent space http://www.paulsprojects.net/tutorials/tutorials.html
    //( Sx Sy Sz )
    //( Tx Ty Tz ) - matrix to convert from object space to tangent space
    //( Nx Ny Nz )
    // links: http://www.ati.com/developer/sdk/RADEONSDK/Html/Tutorials/RADEONBumpMap.html
 
//    mat_id mat;  
	FaceMaterials *faceMaterial;

    std::string name;

    Model(): name() { }
    void updateMaterial();
    void updateNormals();
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
		delete faceMaterial;
		return true;
	}
};


#endif
