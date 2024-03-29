/*id*********************************************************
	File: graph/opengl/opengl_13.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Функции для рендерига объектов на OpenGL 1.3

		Multitexture
		Bump mapping
		Cube maps
		Compressed Textures
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"
#include "ext/libext.h"
#include "../../common/logger.h"
#include "../material.h"

namespace opengl
{

// нарисовать множество полигонов с указанным материалом / Multitexture
bool OpenGL_Engine::DrawFill_MaterialStd_OpenGL13(GraphShadow& e, const Faces& triangles, MaterialStd& material)
{
	if (GL_EXTENSION_MULTITEXTURE)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		pvector<uid> buffersToDelete;

		bool bump_map = material.normal_map.image != NULL && flags.bump && !e.lights.empty() && e.normals != NULL && GL_EXTENSION_DOT3 && GL_EXTENSION_TEXTURE_CUBE_MAP;
		bool diffuse_map = material.diffuse_map.image != NULL && flags.textures;
		bool emission_map = material.emission_map.image != NULL && flags.textures;
        bool blend_map = material.blend_map.image != NULL;
		int currentTextureArb = 0;

		if (bump_map)
		{
			const TexCoords *texCoords = e.getTexCoords(material.normal_map);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			uid bufId = objectIdGenerator.genUid();
			buffersToDelete.push_back(bufId);

			drawBump(e, texCoords, e.position, e.lights[0]->position, bufId, currentTextureArb, material.normal_map.image);
			currentTextureArb +=2;
		}

		if (diffuse_map)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glClientActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glDisable(GL_TEXTURE_COORD_ARRAY);

			GLint mode = GL_REPLACE;
			if(currentTextureArb > 0)
				mode = GL_MODULATE;

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);

			(this->*BindTexture)(*material.diffuse_map.image, true);

			const TexCoords *texCoords = NULL;

			if (material.diffuse_map.texCoordsUnit < e.texCoords.size())
			{
				texCoords = e.texCoords[material.diffuse_map.texCoordsUnit];
			}
			else if(!e.texCoords.empty())
			{
				texCoords = e.texCoords[0];
			}

			assert(texCoords->size() == e.vertexes->size(), "TexCoords.size != Vertex.size");
			if(BindTexCoords != NULL) (this->*BindTexCoords)(texCoords, &material.textureMatrix);
			currentTextureArb++;
		}

		if (emission_map)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glClientActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glDisable(GL_TEXTURE_COORD_ARRAY);

			GLint mode = GL_REPLACE;
			if (currentTextureArb > 0)
			{
				mode = GL_ADD;
			}

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);

			(this->*BindTexture)(*material.emission_map.image, true);

			const TexCoords *texCoords = NULL;

			if (material.diffuse_map.texCoordsUnit < e.texCoords.size())
			{
				texCoords = e.texCoords[material.diffuse_map.texCoordsUnit];
			}
			else if (!e.texCoords.empty())
			{
				texCoords = e.texCoords[0];
			}

			assert(texCoords->size() == e.vertexes->size(), "TexCoords.size != Vertex.size");
			(this->*BindTexCoords)(texCoords, &material.textureMatrix);
			currentTextureArb++;
		}

		if (blend_map)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glClientActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glDisable(GL_TEXTURE_COORD_ARRAY);

            GLint mode = GL_MODULATE;

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);

			(this->*BindTexture)(*material.blend_map.image, true);

			const TexCoords *texCoords = NULL;

			if (material.blend_map.texCoordsUnit < e.texCoords.size())
			{
				texCoords = e.texCoords[material.blend_map.texCoordsUnit];
			}
			else if (!e.texCoords.empty())
			{
				texCoords = e.texCoords[0];
			}

			assert(texCoords->size() == e.vertexes->size(), "TexCoords.size != Vertex.size");
			(this->*BindTexCoords)(texCoords, &material.textureMatrix);
			currentTextureArb++;
		}

        if (!diffuse_map)
		{
			glColor4fv(material.color.getfv());
		}

		(this->*DrawTriangles)(e, triangles, NULL);

		if (CleanupDrawTriangles != NULL) (this->*CleanupDrawTriangles)();

		for(uidVector::const_iterator it = buffersToDelete.begin(); it != buffersToDelete.end(); it++)
		{
			cleanBuffer(*it);
		}
		
		unbindTexCoords();

		glPopClientAttrib();
	   	glPopAttrib();

		return true;
	}
	return false;
}

static v3 getstangent(v2 A, v3 B, v3 N, v2 S)
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


void OpenGL_Engine::getTangentSpace(const VertexVector *vertex, const TexCoords *texcoord, const FaceMaterialVector *faceMaterials, Normals const *normal, TexCoords3f **sTangent, TexCoords3f **tTangent)
{ // TODO: mem cleanup
	int id = vertex->getId();
	
	assert(id > 0, "vertex.id > 0");

	bool was = tangentSpaceCache.find(id) != tangentSpaceCache.end();

	*sTangent = &tangentSpaceCache[id].t;
	*tTangent = &tangentSpaceCache[id].b;

	if (!vertex->wasChanged() && was)
	{
		return;
	}

	if (!vertex || !texcoord || texcoord->size() != vertex->size()) return;

    unsigned int size = vertex->size();
//    tangentSpaceLight.resize(s); // TODO

/*	v3List *S, *T;
	S = new v3List(size);
	T = new v3List(size);
	v3List &s = *S;
	v3List &t = *T;
	
    for (unsigned int i=0; i<size; i++)
    {
        s[i].loadZero();
        t[i].loadZero();
    }

	for(FaceMaterialVector::const_iterator it = faceMaterials->begin(); it != faceMaterials->end(); it++)
	{
		Faces *triangle = it->triangles;
		

		for (unsigned int i=0; i<faces->triangles.size(); i++)
		{
			int a = faces->triangles[i].a[0];
			int b = faces->triangles[i].a[1];
			int c = faces->triangles[i].a[2];
			int e, f; // from e to f

			v2 me;
			v3  ve, ne;

			e = a;
			// vertex a (vector ab)
			me = texcoord->data[e]; ve = vertex->data[e];		ne = normal->data[e];

			f = b;  s[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
			// vertex a (vector ac)
			f = c;  s[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
			f = b;  t[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
			f = c;  t[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));

			e = b;
			me = texcoord->data[e]; ve = vertex->data[e];		ne = normal->data[e];
			f = a;  s[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
			f = c;  s[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
			f = a;  t[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
			f = c;  t[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));

			e = c;
			me = texcoord->data[e]; ve = vertex->data[e];		ne = normal->data[e];
			f = a;  s[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
			f = b;  s[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(1.0, 0.0));
			f = a;  t[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
			f = b;  t[e] += getstangent(texcoord->data[f]-me, vertex->data[f] - ve, ne, v2(0.0, -1.0));
		}
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
*/
	pvector<v3>& S = **sTangent;
	pvector<v3>& T = **tTangent;
	S.resize(size);
	T.resize(size);

	memset(&S[0], 0, size * sizeof(v3));
    
	for (FaceMaterialVector::const_iterator it = faceMaterials->begin(); it != faceMaterials->end(); it++)
	{
		Faces* faces = it->faces;
		
		for (size_t a=0; a < faces->triangles.size(); a++)
		{
			unsigned int i1 = faces->triangles[a].a[0];
			unsigned int i2 = faces->triangles[a].a[1];
			unsigned int i3 = faces->triangles[a].a[2];
        
			const v3& vertex1 = vertex->at(i1);
			const v3& vertex2 = vertex->at(i2);
			const v3& vertex3 = vertex->at(i3);
        
			const v2& w1 = texcoord->at(i1);
			const v2& w2 = texcoord->at(i2);
			const v2& w3 = texcoord->at(i3);
        
			float x1 = vertex2.x - vertex1.x;
			float x2 = vertex3.x - vertex1.x;
			float y1 = vertex2.y - vertex1.y;
			float y2 = vertex3.y - vertex1.y;
			float z1 = vertex2.z - vertex1.z;
			float z2 = vertex3.z - vertex1.z;
        
			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;
        
			float r = 1.0F / (s1 * t2 - s2 * t1);
			v3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
					(t2 * z1 - t1 * z2) * r);
        
			S[i1] += sdir;
			S[i2] += sdir;
	        S[i3] += sdir;
	    }
	}


	for (long a = 0; a < (int)vertex->size(); a++)
    {
		const v3& n = normal->at(a);
        v3 t = S[a];
        
        // Gram-Schmidt orthogonalize
		t = (t - n * (n.dotProduct(t))).getNormalized();
		v3 b = t * n;

		S[a] = t;
		T[a] = b;
        
        // Calculate handedness
//        tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    }
};

void OpenGL_Engine::genTangentSpaceLight(const TexCoords3f& sTangent, const TexCoords3f &tTangent, const VertexVector& vertex, const Normals& normal,	const matrix34 matrix, const v3 light,	pvector<v3>& tangentSpaceLight)
{
	matrix34 inverseModelMatrix;
    inverseModelMatrix = matrix.getInverse();

	v3 objectLightPosition = inverseModelMatrix*light;

	pvector<v3>& tl = tangentSpaceLight;

    // vi4isljaem vektor napravlennij na isto4nik sveta v tangensnom prostranstve kazhdoj ver6ini
    for (unsigned int i=0; i<vertex.size(); i++)
    {
		v3 lightVector =  objectLightPosition - vertex[i];
		tl[i].x = sTangent[i].dotProduct(lightVector); // scalar product
		tl[i].y = tTangent[i].dotProduct(lightVector);
		tl[i].z = normal[i].dotProduct(lightVector);
    }
}

void OpenGL_Engine::drawBump(GraphShadow &e, const TexCoords *coords, matrix34 const matrix, v3 const light, uid bufId, int curTexArb, Image *img)
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
	tangentSpaceLight.resize(e.vertexes->size());

	TexCoords3f *sTangent, *tTangent;

	getTangentSpace(e.vertexes, coords, e.faceMaterials, e.normals, &sTangent, &tTangent);
	genTangentSpaceLight(*sTangent, *tTangent, *e.vertexes, *e.normals, matrix, light, tangentSpaceLight);

	if(BindTexCoords3f) (this->*BindTexCoords3f)(&tangentSpaceLight);

	glActiveTextureARB(GL_TEXTURE0_ARB + curTexArb + 1);
	glClientActiveTextureARB(GL_TEXTURE0_ARB + curTexArb + 1);
	(this->*BindTexture)(*img, true);

	if(BindTexCoords) (this->*BindTexCoords)(coords, NULL);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
}

} // namespace opengl
