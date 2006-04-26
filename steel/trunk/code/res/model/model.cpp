#include "model.h"
using namespace std;

/*void Model:: updateMaterial()
{
    if (MatName != "")
        mat = material.getindex(MatName);
    else
        mat = 0;
}
*/

MapCoords	*Model::getMapCoords()
{
	return &mapCoords;
}


/*int Model::duplicateVertex(int src, v3 newnormal)
{
    int newsize = vertex.size()+1;

    vertex.resize(newsize);
    vertex[newsize-1] = vertex[src];

    normal.resize(newsize);
    normal[newsize-1] = newnormal;

    mapcoord.resize(newsize);
    mapcoord[newsize-1] = mapcoord[src];
    return newsize-1;
}*/

void Model::generateNormals()
{
    // Kollapse - soedinenie blizkih vershin
/*   if (mapcoord.empty())
    for (unsigned int i=0; i<vertex.size(); i++)
        for (unsigned int j = i+1; j<vertex.size(); j++)
        if ((vertex[i]-vertex[j]).GetSquaredLength()<0.00001) // ravni
        {
            int last = vertex.size()-1;
            for (unsigned int k=0; k<face.size()*3; k++) // Collapse
            {
                if ((unsigned)index[k] == j)    index[k] = i;
                if ((unsigned)index[k] == (unsigned)last) index[k] = j;
            }
            vertex[j] = vertex[last];
            vertex.resize(vertex.size()-1);
            j--;
        }
*/


    vector<v3> facenormal;
    facenormal.resize(triangle.size());

    normal.resize(vertex.vertex.size());

    for (unsigned int i=0; i<normal.size(); i++)
    {
        normal[i].loadZero();
    }
    // Pods4et normalej poligonov
    for (unsigned int i=0; i<triangle.size(); i++)
    {
		int a = triangle[i].a[0];
        int b = triangle[i].a[1];
        int c = triangle[i].a[2];

        v3 p = vertex.vertex[b] - vertex.vertex[a];
        v3 q = vertex.vertex[c] - vertex.vertex[a];
        facenormal[i] = vectmul(p,q);
        if (facenormal[i] == v3(0,0,0)) facenormal[i] = v3(0,1,0); // TEMP TODO - Up
        facenormal[i].normalize();
        normal[a] += facenormal[i];
        normal[b] += facenormal[i];
        normal[c] += facenormal[i];
    }
    for (unsigned int i=0; i<normal.size(); i++)
        normal[i].normalize();

// TODO - speed up

/*    float k = 0.8f;

    if (!smoothmodels)
        k = 5;

// Dublirovanie Normali v vertexu, esli ugol mezhdu poligonami sli6kom bolshoj (cos(a)<k)
   for (unsigned int i=0; i<index.size(); i++)
      for (unsigned int j=i+1; j<index.size(); j++) // ugol sli6kom ostrij 4to-bi sglazhivat' - nado razbit' vershini (ugol mezhdu nomralju vershini (usrednennoj po fejsam) i normalju fejsa)
        if ( (index[i] == index[j]) && (facenormal[i/3].dotProduct(facenormal[j/3])<k)) // nado razdeljat'
        {
             index[j] = duplicateVertex(index[i], facenormal[j/3]);

             unsigned int kk;
             for (kk = 0; kk<index.size(); kk++)
                if ((index[kk] == index[i]) && (facenormal[j/3].dotProduct(facenormal[kk/3]) >=k || !smoothmodels))
                    index[kk] = index[j];
        }
*/

/*    int s = vertex.size();
    for (unsigned int i=0; i<s; i++)
    if (use[i]>1)
    {
        for (unsigned int j=0; j<face.size(); j++)
        {
            if ((index[j].a == i) && ( facenormal[j].DotProduct(normal[i])<k))
            {
                index[j].a = duplicateVertex(i, facenormal[j]);
                use[i]--; i--; break;
            }
            if ((index[j].b == i) && ( facenormal[j].DotProduct(normal[i])<k))
            {
                index[j].b = duplicateVertex(i, facenormal[j]);
                use[i]--; i--; break;
            }
            if ((index[j].c == i) && ( facenormal[j].DotProduct(normal[i])<k))
            {
                index[j].c = duplicateVertex(i, facenormal[j]);
                use[i]--; i--; break;
            }
        }
    }
*/
//    normal.resize(vertex.size());

    for (unsigned int i=0; i<normal.size(); i++)
        normal[i].loadZero();

    for (unsigned int i=0; i<triangle.size(); i++)
    {
        int a = triangle[i].a[0];
        int b = triangle[i].a[1];
        int c = triangle[i].a[2];
        normal[a] += facenormal[i];
        normal[b] += facenormal[i];
        normal[c] += facenormal[i];
    }
    for (unsigned int i=0; i<normal.size(); i++)
        normal[i].normalize();
}

void Model::updateAABB()
{
	frame.clear();
	for(std::vector<v3>::iterator it = vertex.vertex.begin(); it != vertex.vertex.end(); it++)
		frame.merge(*it);
}
