/*id*********************************************************
	File: res/model/model.h
	Unit: res/model
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс для хранения модели в виде многогранника
 ************************************************************/

// sTangent - vektor v storonu uveli4enija S teksturnoj koordinati (vpravo)
    // tangentSpaceLight - vektor ot ver6uni do to4ki osveshenija v TBN (Tangent, Binormal, Normal) prostrastve (sTangent, tTangent, normals) - tangent space http://www.paulsprojects.net/tutorials/tutorials.html
    //( Sx Sy Sz )
    //( Tx Ty Tz ) - matrix to convert from object space to tangent space
    //( Nx Ny Nz )
    // links: http://www.ati.com/developer/sdk/RADEONSDK/Html/Tutorials/RADEONBumpMap.html
 
//    mat_id mat;  


#include "../../steel.h"
#include "model.h"
#include "../../common/logger.h"
#include "../../common/utils.h"

/*void Model:: updateMaterial()
{
    if (MatName != "")
        mat = material.getindex(MatName);
    else
        mat = 0;
}
*/

const TexCoords	*Model::getTexCoords(int texNumber) const
{
	return &texCoords;
}


/*int Model::duplicateVertex(int src, v3 newnormal)
{
    int newsize = vertexes.size()+1;

    vertexes.resize(newsize);
    vertexes[newsize-1] = vertexes[src];

    normals.resize(newsize);
    normals[newsize-1] = newnormal;

    mapcoord.resize(newsize);
    mapcoord[newsize-1] = mapcoord[src];
    return newsize-1;
}*/

void Model::generateNormals()
{
    // Kollapse - soedinenie blizkih vershin
/*   if (mapcoord.empty())
    for (unsigned int i=0; i<vertexes.size(); i++)
        for (unsigned int j = i+1; j<vertexes.size(); j++)
        if ((vertexes[i]-vertexes[j]).GetSquaredLength()<0.00001) // ravni
        {
            int last = vertexes.size()-1;
            for (unsigned int k=0; k<face.size()*3; k++) // Collapse
            {
                if ((unsigned)index[k] == j)    index[k] = i;
                if ((unsigned)index[k] == (unsigned)last) index[k] = j;
            }
            vertexes[j] = vertexes[last];
            vertexes.resize(vertexes.size()-1);
            j--;
        }
*/


	pvector<v3> facenormal;
	facenormal.resize(allFaces.triangles.size());

    normals.resize(vertexes.size());

    for (unsigned int i=0; i < normals.size(); i++)
    {
        normals[i].loadZero();
    }
    // Pods4et normalej poligonov
	for (unsigned int i=0; i < allFaces.triangles.size(); i++)
    {
		int a = allFaces.triangles[i].a[0];
        int b = allFaces.triangles[i].a[1];
        int c = allFaces.triangles[i].a[2];

        v3 p = vertexes[b] - vertexes[a];
        v3 q = vertexes[c] - vertexes[a];
        facenormal[i] = p*q;
        if (facenormal[i] == v3(0,0,0))
        {
            facenormal[i] = v3(0,1,0); // TEMP TODO - Up
        }
        facenormal[i].normalize();
        normals[a] += facenormal[i];
        normals[b] += facenormal[i];
        normals[c] += facenormal[i];
    }
    for (unsigned int i=0; i<normals.size(); i++)
	{
        normals[i].normalize();
	}

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

/*    int s = vertexes.size();
    for (unsigned int i=0; i<s; i++)
    if (use[i]>1)
    {
        for (unsigned int j=0; j<face.size(); j++)
        {
            if ((index[j].a == i) && ( facenormal[j].DotProduct(normals[i])<k))
            {
                index[j].a = duplicateVertex(i, facenormal[j]);
                use[i]--; i--; break;
            }
            if ((index[j].b == i) && ( facenormal[j].DotProduct(normals[i])<k))
            {
                index[j].b = duplicateVertex(i, facenormal[j]);
                use[i]--; i--; break;
            }
            if ((index[j].c == i) && ( facenormal[j].DotProduct(normals[i])<k))
            {
                index[j].c = duplicateVertex(i, facenormal[j]);
                use[i]--; i--; break;
            }
        }
    }
*/
//    normals.resize(vertexes.size());

    for (unsigned int i = 0; i < normals.size(); i++)
	{
        normals[i].loadZero();
	}

    for (unsigned int i = 0; i < allFaces.triangles.size(); i++)
    {
        int a = allFaces.triangles[i].a[0];
        int b = allFaces.triangles[i].a[1];
        int c = allFaces.triangles[i].a[2];
        normals[a] += facenormal[i];
        normals[b] += facenormal[i];
        normals[c] += facenormal[i];
    }
    for (unsigned int i = 0; i < normals.size(); i++)
	{
        normals[i].normalize();
	}
}

void Model::updateAABB()
{
	frame.clear();
	for EACH(pvector<v3>, vertexes, it)
	{
		frame.merge(*it);
	}
}

float Model::calculateVolume() // вычислить объём
{
	float volume = 0;
	for EACH(TriangleVector, allFaces.triangles, it)
	{
		v3 a(vertexes[it->a[0]]);
		v3 b(vertexes[it->a[1]]);
		v3 c(vertexes[it->a[2]]);
		volume += (a*b)&c;
	}

	return fabs(volume)/6.0f;
}

float Model::getVolume() // вычислить объём
{
	if (volume < -EPSILON)
    {
		return volume = calculateVolume();
    }
	else
    {
		return volume;
    }
}

bool Model::checkModel() const
{
    if (faceMaterials.empty())
    {
        log_msg("warning res model", std::string("Model ") + name + ": faceMaterials is empty");
    }

    if (vertexes.empty())
    {
        log_msg("error res model", std::string("Model ") + name + ": vertexes is empty");
        return false;
    }

    if (!texCoords.empty() && texCoords.size() != vertexes.size())
    {
        log_msg("error res model", std::string("Model ") + name + ": texCoords.size() != vertexes.size()");
        return false;
    }

	for (FaceMaterialVector::const_iterator it = faceMaterials.begin(); it != faceMaterials.end(); it++)
	{
		Faces* faces = it->faces;

        if (it->material == NULL)
        {
            log_msg("warning res model", std::string("Model ") + name + ": material is NULL");
        }
		
		for (size_t a=0; a < faces->triangles.size(); a++)
		{
            for (int i = 0; i < 3; i++)
            {
			    unsigned int index = faces->triangles[a].a[0];
                if (index >= vertexes.size())
                {
                    log_msg("error res model", std::string("Model ") + name + ": vertex index is out of range (" + IntToStr(index) + "/" + IntToStr(vertexes.size()) + ")");
                    return false;
                }
            }
	    }
	}
    return true;
}

