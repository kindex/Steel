/*id*********************************************************
	File: graph/opengl/opengl_13.cpp
	Unit: opengl
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

#include "../../steel.h"
#include "opengl_engine.h"
#include "gl/libext.h"


// нарисовать множество полигонов с указанным материалом / Multitexture
void OpenGL_Engine::DrawFill_OpenGL13(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total)
{
	if(material != NULL && GL_EXTENSION_MULTITEXTURE)
	{
		svector<uid> buffersToDelete;

		total.objectCount++;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		int texCount = material->getTextureCount();

		if(texCount>0)
		{
			switch(material->getTexture(0)->mode)
			{
				case TEXTURE_BLEND_MODE_MUL:	
						glEnable(GL_BLEND);
						glBlendFunc(GL_DST_COLOR, GL_ZERO);
						break;
				case TEXTURE_BLEND_MODE_ADD:
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE, GL_ONE);
						break;
				case TEXTURE_BLEND_MODE_REPLACE: 
				default:
					glDisable(GL_BLEND);
					break;
			}
		}

		TextureBlendMode inheritedMode = TEXTURE_BLEND_MODE_NONE, currentMode;
		int currentTextureArb = 0;

		// TODO check OPENGL_EXTENSION_MULTITEXTURE_TEXTURE_UNITS
		for(int i=0; i<texCount; i++)
		{
			const Texture &texture = *material->getTexture(i); // текущая текстура

			if(inheritedMode == TEXTURE_BLEND_MODE_NONE)
				currentMode = texture.mode;
			else
			{
				currentMode = inheritedMode;
				inheritedMode = TEXTURE_BLEND_MODE_NONE;
			}

			// skip texture
			if(texture.format == TEXTURE_FORMAT_BUMP_MAP && !GL_EXTENSION_DOT3
				|| texture.format == TEXTURE_FORMAT_ENV && !GL_EXTENSION_TEXTURE_CUBE_MAP
				|| texture.format == TEXTURE_FORMAT_NORMAL_MAP)
			{
				inheritedMode = currentMode;
				continue;
			}

			glActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glClientActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
			glDisable(GL_TEXTURE_COORD_ARRAY);

			// режим мультитекстурирования
			GLint mode = GL_REPLACE;
			if(currentTextureArb>0)
			{
				switch(currentMode)
				{
					case TEXTURE_BLEND_MODE_MUL: 	mode = GL_MODULATE; break;
					case TEXTURE_BLEND_MODE_ADD:	mode = GL_ADD;		break; // TODO: GL_ADD_SIGNED_ARB ??
					case TEXTURE_BLEND_MODE_BLEND:	mode = GL_BLEND;	break;
					default: mode = GL_REPLACE;		break;
				}
			}

			if(conf->geti("drawBump") && !lights.empty() && (i==0) && !e.blend && (texture.format == TEXTURE_FORMAT_BUMP_MAP || texture.format == TEXTURE_FORMAT_COLOR_MAP))
			{
				const TexCoords *coords = e.texCoords[i];

				int j = 0;

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

				uid bufId = objectIdGenerator.genUid();
				buffersToDelete.push_back(bufId);

				if(texture.format == TEXTURE_FORMAT_BUMP_MAP)
				{
					drawBump(e, coords, e.position, lights[j].position, bufId, currentTextureArb, texture.image);
					currentTextureArb +=2;
				}
				else
				{
					drawBump(e, coords, e.position, lights[j].position, bufId, currentTextureArb, zeroNormal);
					glActiveTextureARB(GL_TEXTURE2_ARB + currentTextureArb);
					glClientActiveTextureARB(GL_TEXTURE2_ARB + currentTextureArb);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					
					(this->*BindTexture)(texture.image, true); // 2D texture (auto detect from Image)

					(this->*BindTexCoords)(coords);
					currentTextureArb += 3;
				}
			}
			else
			if(conf->geti("drawReflect") && texture.format == TEXTURE_FORMAT_ENV) // карта отражения
			{ // загружаем текстуру
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
				glActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
				glClientActiveTextureARB(GL_TEXTURE0_ARB + currentTextureArb);
				(this->*BindTexture)(texture.image, true); // Cube texture (auto detect from Image)

				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
                glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);

				glEnable(GL_TEXTURE_GEN_S); 
                glEnable(GL_TEXTURE_GEN_T); 
                glEnable(GL_TEXTURE_GEN_R); 

//                glEnable(GL_NORMALIZE);

				currentTextureArb += 1;
			}
			else
			if(texture.format == TEXTURE_FORMAT_COLOR_MAP)
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
				if(BindTexture) (this->*BindTexture)(texture.image, true);

				const TexCoords *coords = e.texCoords[i];
				assert(coords->data.size() == e.vertexes->data.size(), "TexCoords.size != Vertex.size");
				if(BindTexCoords != NULL) (this->*BindTexCoords)(coords);
				currentTextureArb++;
			}
			else
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
				if(BindTexCoords) (this->*BindTexCoords)(NULL);
				currentTextureArb++;
			}

			if(texture.format == TEXTURE_FORMAT_COLOR) 
				glColor4fv(&texture.color.r);

		}
		
		if(DrawTriangles) (this->*DrawTriangles)(e, triangles, NULL, total);

	   	glPopAttrib();
		glPopClientAttrib();

		for(svector<uid>::const_iterator it = buffersToDelete.begin(); it != buffersToDelete.end(); it++)
		{
			cleanBuffer(*it);
		}
	}
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


void OpenGL_Engine::getTangentSpace(const Vertexes *vertex, const TexCoords *mapcoord, const FaceMaterials *faceMaterials, Normals const *normal, svector<v3> **sTangent, svector<v3> **tTangent)
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

	for(FaceMaterials::const_iterator it = faceMaterials->begin(); it != faceMaterials->end(); it++)
	{
		Triangles *triangle = it->triangles;
		

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

void OpenGL_Engine::genTangentSpaceLight(svector<v3> const &sTangent, svector<v3> const &tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light,	v3List &tangentSpaceLight)
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

/*
void OpenGL_Engine::drawDistColor(DrawElement &e, matrix34 const matrix, v3 const light, float const distance)
{
	float *coords = new float[e.vertexes->size()];

	int i = 0;
	for(Vertexes::iterator it = e.vertexes->begin(); it != e.vertexes->end(); it++)
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

//		getTangentSpace(e.vertexes, e.mapcoord, e.triangle, e.normals, &sTangent, &tTangent);
//		genTangentSpaceLight(*sTangent, *tTangent, *e.vertexes, *e.normals, matrix, light, &tangentSpaceLight);

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
*/

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
	tangentSpaceLight.data.resize(e.vertexes->data.size());

	v3List *sTangent, *tTangent;

	getTangentSpace(e.vertexes, coords, e.faceMaterials, e.normals, &sTangent, &tTangent);
	genTangentSpaceLight(*sTangent, *tTangent, *e.vertexes, *e.normals, matrix, light, tangentSpaceLight.data);

	if(BindTexCoords3f) (this->*BindTexCoords3f)(&tangentSpaceLight);

	glActiveTextureARB(GL_TEXTURE0_ARB + curTexArb + 1);
	glClientActiveTextureARB(GL_TEXTURE0_ARB + curTexArb + 1);
	(this->*BindTexture)(img, true);

	if(BindTexCoords) (this->*BindTexCoords)(coords);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
}

