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

TexCoords	*Model::getTexCoords(int texNumber)
{
	return &texCoords;
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
    facenormal.resize(triangleAll.data.size());

    normal.data.resize(vertex.data.size());

    for (unsigned int i=0; i<normal.data.size(); i++)
    {
        normal.data[i].loadZero();
    }
    // Pods4et normalej poligonov
    for (unsigned int i=0; i<triangleAll.data.size(); i++)
    {
		int a = triangleAll.data[i].a[0];
        int b = triangleAll.data[i].a[1];
        int c = triangleAll.data[i].a[2];

        v3 p = vertex.data[b] - vertex.data[a];
        v3 q = vertex.data[c] - vertex.data[a];
        facenormal[i] = p*q;
        if (facenormal[i] == v3(0,0,0)) facenormal[i] = v3(0,1,0); // TEMP TODO - Up
        facenormal[i].normalize();
        normal.data[a] += facenormal[i];
        normal.data[b] += facenormal[i];
        normal.data[c] += facenormal[i];
    }
    for (unsigned int i=0; i<normal.data.size(); i++)
        normal.data[i].normalize();

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

    for (unsigned int i=0; i<normal.data.size(); i++)
        normal.data[i].loadZero();

    for (unsigned int i=0; i<triangleAll.data.size(); i++)
    {
        int a = triangleAll.data[i].a[0];
        int b = triangleAll.data[i].a[1];
        int c = triangleAll.data[i].a[2];
        normal.data[a] += facenormal[i];
        normal.data[b] += facenormal[i];
        normal.data[c] += facenormal[i];
    }
    for (unsigned int i=0; i<normal.data.size(); i++)
        normal.data[i].normalize();
}

void Model::updateAABB()
{
	frame.clear();
	for(std::vector<v3>::iterator it = vertex.data.begin(); it != vertex.data.end(); it++)
		frame.merge(*it);
}


float Model::calculateVolume() // вычислить объём
{
	float volume = 0;
	for(std::vector<Triangle>::iterator it = triangleAll.data.begin(); it != triangleAll.data.end(); it++)
	{
		v3 a(vertex.data[it->a[0]]);
		v3 b(vertex.data[it->a[1]]);
		v3 c(vertex.data[it->a[2]]);
		volume += (a*b)&c/6.0;
	}

	return fabs(volume);
}

float Model::getVolume() // вычислить объём
{
	if(volume<-EPSILON)
		return volume = calculateVolume();
	else
		return volume;
}
