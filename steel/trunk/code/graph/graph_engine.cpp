#include "graph_engine.h"


WindowSettings::WindowSettings() 
{ 
	title	= "Steel Engine"; 
	left	= 0;
	top		= 0;
	width	= 700; 
	height	= width*3/4; 
	bpp		= 32;

	fullscreen = false;
}

bool GraphEngine::inject(GraphInterface *object, MATRIX4X4 matrix)
{
	objects.push_back(object);
	MATRIX4X4 cur_matrix, new_matrix;

	cur_matrix = object->getMatrix();
	new_matrix = matrix*cur_matrix;

	GraphInterface &o = *(GraphInterface*)object;
	GraphInterfaceList children = o.getChildrens();
	for(GraphInterfaceList::iterator it=children.begin();
		it != children.end();
		it++)
		if(!inject(*it, new_matrix)) return false;

	FaceMaterials* m = object->getFaceMaterials();
	Vertexes	*v = object->getVertexes();
	Triangles	*t = object->getTriangles();
	Lights		*l = object->getLights();
	Normals		*n = object->getNormals();

	if(l != NULL)
	{
		for(Lights::iterator it = l->begin(); it != l->end(); it++)
		{
			light.push_back(*it);
			light[light.size()-1].pos = new_matrix.getCoords();
		}
	}


	if(t != NULL)
	for(FaceMaterials::iterator it = m->begin(); it != m->end(); it++)
	{
		int c = elements.size();
		elements.resize(c+1);

		elements[c].material = it->first;

// TODO	it->secondm olny tringles from this material

		elements[c].triangle = new Triangles;

		int s = it->second.size();
		elements[c].triangle->clear();

		for(int i=0; i<s; i++)
			elements[c].triangle->push_back(t->operator [](it->second[i]));

		elements[c].vertex = v;
		elements[c].mapcoord = object->getMapCoords();
		elements[c].matrix = new_matrix;
		elements[c].normal = n;

	}

//		v3 pos = o.getPos();
	return true;
}

bool GraphEngine::clear()
{
//	for(vector<GraphInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		//it->

	for(vector<DrawElement>::iterator it = elements.begin(); it != elements.end(); it++)
		delete it->triangle;

	objects.clear();
	elements.clear();
	light.clear();

	return true;
}

v3 getstangent(v2 A, v3 B, v3 N, v2 S)
{
    A.Normalize();
//    Vector2 S(1.0, 0.0); // Os teksturnih koordinat OX
    float sina = A.pseudoscalarProduct(S); // Vect mull
    float cosa = A.dotProduct(S); // skalarnoe
    // a - ugon mezhdu S i A
    // esli v normalnom prostranstve povernut' vektor B na ugol -a (minus a), to mipolu4im vektor napravlennij v storonu uveli4enija koordinati S (sTangent)

//    return B.rotate(N, sina, cosa);

    MATRIX4X4 M;
    M.SetRotationAxis(sina, cosa, v3(N.x, N.y, N.z)); // povernut' na ang v ploskoti perpedukularnoj N

    v4 V(B.x, B.y, B.z, 1.0), R;
    R = M*V;
    v3 Res(R.x, R.y, R.z);
    Res.Normalize();
    return Res;

}


void GraphEngine::DrawElement::updateTangentSpace()
{
	if(!vertex || !mapcoord || mapcoord->empty() || !triangle) return;

    unsigned int s = vertex->size();
    tangentSpaceLight.resize(s); // TODO

    sTangent.resize(s);
    tTangent.resize(s);

    for (unsigned int i=0; i<s; i++)
    {
        sTangent[i].LoadZero();
        tTangent[i].LoadZero();
    }

    for (unsigned int i=0; i<triangle->size(); i++)
    {
        int a = triangle->operator[](i).a[0];
        int b = triangle->operator[](i).a[1];
        int c = triangle->operator[](i).a[2];
        int e, f; // from e to f

		v2 me;
		v3  ve, ne;

        e = a;
        // vertex a (vector ab)
		ve = vertex->operator[](e);
		ne = normal->operator[](e);

        f = b;  sTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        // vertex a (vector ac)
        f = c;  sTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = b;  tTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
        f = c;  tTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));

        e = b;
		me = mapcoord->operator[](e);ve = vertex->operator[](e);
		ne = normal->operator[](e);
        f = a;  sTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = c;  sTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = a;  tTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
        f = c;  tTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));

        e = c;
		me = mapcoord->operator[](e);ve = vertex->operator[](e);
		ne = normal->operator[](e);
        f = a;  sTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = b;  sTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = a;  tTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
        f = b;  tTangent[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
    }
    for (unsigned int i=0; i<s; i++)
    {
        sTangent[i].Normalize();
        tTangent[i].Normalize();
    }
}
