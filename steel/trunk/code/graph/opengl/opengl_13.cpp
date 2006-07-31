/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Функции для рендерига объектов на OpenGL 1.3

		Multitexture
		Bump mapping
		Cube maps
		Compressed Textures
 ************************************************************/

#include "opengl_engine.h"

v3 getstangent(v2 A, v3 B, v3 N, v2 S)
{
    A.normalize();
//    Vector2 S(1.0, 0.0); // Os teksturnih koordinat OX
    float sina = A.pseudoscalarProduct(S); // Vect mull
    float cosa = A.dotProduct(S); // skalarnoe
    // a - ugon mezhdu S i A
    // esli v normalnom prostranstve povernut' vektor B na ugol -a (minus a), to mipolu4im vektor napravlennij v storonu uveli4enija koordinati S (sTangent)

//    return B.rotate(N, sina, cosa);

    matrix33 M;
    M.setRotationAxis(sina, cosa, v3(N.x, N.y, N.z)); // povernut' na ang v ploskoti perpedukularnoj N

    v3 V(B.x, B.y, B.z), R;
    R = M*V;
    v3 Res(R.x, R.y, R.z);
    Res.normalize();
    return Res;

}

/*
void OpenGL_Engine::getTangentSpace(Vertexes const *vertex, TexCoords const *mapcoord, Triangles const *triangle, Normals const *normal, steel::vector<v3> **sTangent, steel::vector<v3> **tTangent)
{ // TODO: mem cleanup
	int id = vertex->getId();
	
	if(!vertex->wasChanged() && id>0 && tangentSpaceCache.find(id) != tangentSpaceCache.end())
	{
		*sTangent = tangentSpaceCache[id].s;
		*tTangent = tangentSpaceCache[id].t;
		return;
	}

	if(!vertex || !mapcoord || mapcoord->data.empty()) return;

    unsigned int size = vertex->data.size();
//    tangentSpaceLight.resize(s); // TODO

	v3List *S, *T;
	S = new v3List(size);
	T = new v3List(size);
	v3List &s = *S;
	v3List &t = *T;
	
    for (unsigned int i=0; i<size; i++)
    {
        s[i].loadZero();
        t[i].loadZero();
    }

    for (unsigned int i=0; i<triangle->data.size(); i++)
    {
        int a = triangle->data[i].a[0];
        int b = triangle->data[i].a[1];
        int c = triangle->data[i].a[2];
        int e, f; // from e to f

		v2 me;
		v3  ve, ne;

        e = a;
        // vertex a (vector ab)
		me = mapcoord->data[e]; ve = vertex->data[e];		ne = normal->data[e];

        f = b;  s[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
        // vertex a (vector ac)
        f = c;  s[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
        f = b;  t[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
        f = c;  t[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));

        e = b;
		me = mapcoord->data[e]; ve = vertex->data[e];		ne = normal->data[e];
        f = a;  s[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
        f = c;  s[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
        f = a;  t[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
        f = c;  t[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));

        e = c;
		me = mapcoord->data[e]; ve = vertex->data[e];		ne = normal->data[e];
        f = a;  s[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
        f = b;  s[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
        f = a;  t[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
        f = b;  t[e] += getstangent(mapcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
    }
    for (unsigned int i=0; i<size; i++)
    {
        s[i].normalize();
        t[i].normalize();
    }
	
	*sTangent = S;
	*tTangent = T;
	if(id > 0)
	{
		tangentSpaceCache[id].s = S;
		tangentSpaceCache[id].t = T;
	}
};

void OpenGL_Engine::genTangentSpaceLight(steel::vector<v3> const &sTangent, steel::vector<v3> const &tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light,	v3List &tangentSpaceLight)
{
	matrix34 inverseModelMatrix;
    inverseModelMatrix = matrix.getInverse();

	v3 objectLightPosition = inverseModelMatrix*light;

	v3List &tl = tangentSpaceLight;

    // vi4isljaem vektor napravlennij na isto4nik sveta v tangensnom prostranstve kazhdoj ver6ini
    for (unsigned int i=0; i<vertex.data.size(); i++)
    {
		v3 lightVector =  objectLightPosition - vertex.data[i];
		tl[i].x = sTangent[i].dotProduct(lightVector); // scalar product
		tl[i].y = tTangent[i].dotProduct(lightVector);
		tl[i].z = normal.data[i].dotProduct(lightVector);
		
    }
}


void OpenGL_Engine::genTangentSpaceSphere(Vertexes const &vertex, Normals	const &normal, matrix34 const matrix, const v3 _camera,	v3List &tangentSpaceLight)
{
	matrix34 inverseModelMatrix;
    inverseModelMatrix = matrix.getInverse();

	v3 camera = inverseModelMatrix*_camera;

	tangentSpaceLight.resize(vertex.data.size());
	v3List &tl = tangentSpaceLight;

    // vi4isljaem vektor napravlennij na isto4nik sveta v tangensnom prostranstve kazhdoj ver6ini
    for (unsigned int i=0; i<vertex.data.size(); i++)
    {
		v3 c = _camera - matrix*vertex.data[i];
        c.normalize();
        v3 d = (matrix*(vertex.data[i]+normal.data[i]))- matrix*vertex.data[i];
        d.normalize(); // realnaja normal'

        float pscale = c.dotProduct(d); // cos ugla mezhdu c i nermal

        v3 p = d*pscale;

//		tangentSpaceLight[i] = - p - p + c;
		tl[i] = p + p - c;
    }
}


void OpenGL_Engine::drawDistColor(DrawElement &e, matrix34 const matrix, v3 const light, float const distance)
{
	float *coords = new float[e.vertex->size()];

	int i = 0;
	for(Vertexes::iterator it = e.vertex->begin(); it != e.vertex->end(); it++)
	{
		float d = (light-(*it)).getLength();
		float c = 1 - d/distance;
		if(c<0) c = 0;
		if(c>1) c = 1;
		coords[i] = c;
		i++;
	}

	glBindTexture(GL_TEXTURE_2D, distMap);
    glEnable(GL_TEXTURE_2D);

	glTexCoordPointer(1, GL_FLOAT, 0, coords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    drawFaces(e);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	delete coords;
}


bool OpenGL_Engine::drawDiffuse(DrawElement &e, matrix34 const matrix, v3 const light)
{
//	if(GL_TEXTURE_CUBE_MAP_ARB_supported)
	{
		v3List *sTangent, *tTangent, *tangentSpaceLight;

//		getTangentSpace(e.vertex, e.mapcoord, e.triangle, e.normal, &sTangent, &tTangent);
//		genTangentSpaceLight(*sTangent, *tTangent, *e.vertex, *e.normal, matrix, light, &tangentSpaceLight);

	//Bind normalisation cube map to texture unit 1
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);

		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, lightCubeMap);

		glTexCoordPointer(3, GL_FLOAT, 12, &tangentSpaceLight->front());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		drawFaces(e);

		glDisable(GL_TEXTURE_CUBE_MAP_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		delete tangentSpaceLight;
		return true;
	}
//	else
//		return false;
}


void OpenGL_Engine::drawBump(DrawElement &e, TexCoords *coords, matrix34 const matrix, v3 const light, uid bufId, int curTexArb, Image *img)
{

	glActiveTextureARB(GL_TEXTURE0_ARB + curTexArb);
	glClientActiveTextureARB(GL_TEXTURE0_ARB + curTexArb);

	//Bind normalisation cube map to texture unit 1
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	//Send tangent space light vectors for normalisation to unit 1

    tangentSpaceLightBufferedArray tangentSpaceLight;
	tangentSpaceLight.changed = true;
	tangentSpaceLight.id = bufId;
	tangentSpaceLight.data.resize(e.vertex->data.size());

	v3List *sTangent, *tTangent;

	getTangentSpace(e.vertex, coords, e.triangle, e.normal, &sTangent, &tTangent);
	genTangentSpaceLight(*sTangent, *tTangent, *e.vertex, *e.normal, matrix, light, tangentSpaceLight.data);

	if(bind(&tangentSpaceLight, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
	{
		glTexCoordPointer(3, GL_FLOAT, 0,0);
	}

	glActiveTextureARB(GL_TEXTURE1_ARB + curTexArb);
	glClientActiveTextureARB(GL_TEXTURE1_ARB + curTexArb);

	bindTexture(img);
	if(bind(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 2))
	{
		glTexCoordPointer(2, GL_FLOAT, 0,0);
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
}

void OpenGL_Engine::drawReflect(DrawElement &e, matrix34 const matrix, v3 const light, uid bufId)
{
    tangentSpaceLightBufferedArray tangentSpaceLight;
	tangentSpaceLight.changed = true;
	tangentSpaceLight.id = bufId;
	tangentSpaceLight.data.resize(e.vertex->data.size());

	genTangentSpaceSphere(*e.vertex, *e.normal, matrix, light, tangentSpaceLight.data);

	if(bind(&tangentSpaceLight, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
	{
		glTexCoordPointer(3, GL_FLOAT, 0,0);
	}
}
*/
