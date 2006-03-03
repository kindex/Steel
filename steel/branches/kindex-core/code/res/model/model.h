#ifndef __RES__MODEL_MODEL_H
#define __RES__MODEL_MODEL_H

#include "../res.h"
#include "../../math/vector2d.h"
#include "../../math/vector3d.h"

/*typedef struct {
	unsigned char material, filler;
	unsigned long sm_group;
	unsigned short flags;
} Faces;
*/

typedef std::vector<v3> vertexes;

// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned short a[3];
};

typedef std::vector<Triangle> triangles;


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


 //3D Object as triangles mesh, Abstract class
struct Model: public Res
{
//public:
    std::vector<v3> vertex;    // Vertexes
    std::vector <Triangle> triangle;        // Triangles
//    vector <Faces> face;        // Triangles
    
    std::vector<v2> mapcoord; // Texture coordinates

    std::vector<v3> normal, sTangent, tTangent, tangentSpaceLight;    // Vertex normals
    // sTangent - vektor v storonu uveli4enija S teksturnoj koordinati (vpravo)
    // tangentSpaceLight - vektor ot ver6uni do to4ki osveshenija v TBN (Tangent, Binormal, Normal) prostrastve (sTangent, tTangent, normal) - tangent space http://www.paulsprojects.net/tutorials/tutorials.html
    //( Sx Sy Sz )
    //( Tx Ty Tz ) - matrix to convert from object space to tangent space
    //( Nx Ny Nz )
    // links: http://www.ati.com/developer/sdk/RADEONSDK/Html/Tutorials/RADEONBumpMap.html
 
//    mat_id mat;   
	std::string MatName;

    std::string name;

    Model(): name(), MatName("") { }
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
};


#endif
