#include "model.h"





/*void Model:: updateMaterial()
{
    if (MatName != "")
        mat = material.getindex(MatName);
    else
        mat = 0;
}
*/

int Model::duplicateVertex(int src, v3 newnormal)
{
    int newsize = vertex.size()+1;

    vertex.resize(newsize);
    vertex[newsize-1] = vertex[src];

    normal.resize(newsize);
    normal[newsize-1] = newnormal;

    mapcoord.resize(newsize);
    mapcoord[newsize-1] = mapcoord[src];
    return newsize-1;
}

