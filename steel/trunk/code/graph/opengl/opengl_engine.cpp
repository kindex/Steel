/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    Licence:
        Только для Division
    Description:
		Графический джижок релизлванный на фунциях OpenGL. 
		Рисует трехмерный объект в виде 3d-mesh,
		расчитывает динамическое освещение
 ************************************************************/

#include "opengl_engine.h"

#include <iostream>

#include "../../common/utils.h"
#include "../../common/logger.h"

#include "../../res/image/image.h"
#include "gl/libext.h"
//#include "extensions.h"

#include <algorithm>

using namespace std;

bool OpenGL_Engine::bindTexture(Image *image)
{
	if(image == NULL) return false;

	uid	id = image->getId();
	// TODO id 0, id -1
	GLuint glid;
	if(registredTextures.find(id) != registredTextures.end())
	{
		glid = registredTextures[id];
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, glid);
	}
	else
	{
		glGenTextures(1, &glid);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, glid);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int kind;
		if(image->getBpp() == 24) kind = GL_RGB;
		else
		if(image->getBpp() == 32) 
			kind = GL_RGBA;
		else return 0;

		glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image->getWidth(), image->getHeight(),0,
			kind,  GL_UNSIGNED_BYTE , image->getBitmap());

		registredTextures[id] = glid;
	}
	return true;
}

bool OpenGL_Engine::bindVertexes(Vertexes *v)
{
	if(v == NULL) return false;

	uid	id = v->getId();
	// TODO id 0, id -1
	GLuint glid;
	if(buffers.find(id) != buffers.end())
	{
		glid = buffers[id];

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glid);
	    glEnableClientState ( GL_VERTEX_ARRAY );
	    glVertexPointer     ( 3, GL_FLOAT, 0, 0 );
	}
	else
	{
		glGenBuffersARB(1, &glid);

	    glEnableClientState ( GL_VERTEX_ARRAY );
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glid);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 3*sizeof(float)*v->vertex.size(), &v->vertex.front(), GL_STATIC_DRAW);

	    glVertexPointer     ( 3, GL_FLOAT, 0, 0 );

		buffers[id] = glid;
	}

	return true;

}

bool OpenGL_Engine::bindTexCoords(MapCoord *coord)
{
	if(coord == NULL) return false;

	uid	id = coord->getId();
	// TODO id 0, id -1
	GLuint glid;
	if(buffers.find(id) != buffers.end())
	{
		glid = buffers[id];

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glid);
	    glTexCoordPointer ( 2, GL_FLOAT, 0, 0);
	    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

//		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 2*sizeof(float)*coord->mapCoords2f->size(), &coord->mapCoords2f->front());
//		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*sizeof(float)*coord->mapCoords2f->size(), &coord->mapCoords2f->front(), GL_STATIC_DRAW);
		//glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );

	}
	else
	{
		glGenBuffersARB(1, &glid);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glid);

		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*sizeof(float)*coord->mapCoords2f->size(), &coord->mapCoords2f->front(), GL_STATIC_DRAW);

//		glTexCoordPointer(2, GL_FLOAT, 

//		glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );
		glTexCoordPointer ( 2, GL_FLOAT, 0, 0);
	    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

		buffers[id] = glid;
	}

	return true;
}

void OpenGL_Engine::drawElement(DrawElement &e)
{
	glLoadMatrixf(e.matrix.a);
	if(e.triangle && e.vertex)
	{
		Material *m = e.material;
		if(m == NULL)
		{
//			alog.out("Cannot find material '%s'", e.materialName.c_str());
			return;
		}

//		glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );


		bindVertexes(e.vertex);
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glVertexPointer(3, GL_FLOAT, 12 /* sizeof(v3)*/ , &e.vertex->front());

		int texCount = m->map.size();

//	    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
		
		for(int i=0; i<texCount; i++)
		{
			Map map = m->map[i];

			glActiveTextureARB(GL_TEXTURE0_ARB + i);

			bindTexture(map.texture); // 1,2,3D, Cube (auto detect from Image)

//			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
			bindTexCoords(e.mapCoords);

//			glTexCoordPointer(2, GL_FLOAT, 8, &(e.mapCoords->mapCoords2f->front()));

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}


		glDrawElements(GL_TRIANGLES, e.triangle->size()*3, GL_UNSIGNED_INT, &(e.triangle->front()));
		glDisableClientState(GL_VERTEX_ARRAY);

		for(int i=0; i<texCount; i++)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB + i);
			glDisable(GL_TEXTURE_2D);// TODO
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
//	glPopClientAttrib ();
		glActiveTextureARB(GL_TEXTURE0_ARB);


/*		bool alpha = conf->geti("drawAlpha") == 2 && m->gets("color_mode") == "alpha";

		GLuint colorMap = 0;
		if(m->isset("color_map"))
			colorMap = getTexture(m->gets("color_map"));

		GLuint normalMap = 0;
		if(m->isset("normal_map"))
			normalMap = getNormalMap(m->gets("normal_map"));

		GLuint illuminateMap = 0;
		if(m->isset("illuminate_map"))
			illuminateMap = getTexture(m->gets("illuminate_map"));

		GLuint cubeMap = 0;
		if(m->isset("reflect_map"))
			cubeMap = getCubeMap(m->gets("reflect_map"));
		
		int tex = 0;

		glDepthFunc(GL_LEQUAL); 

		glDisable(GL_BLEND);

		if(!alpha)
		for(Lights::iterator it = light.begin(); it != light.end(); it++)
		{
				bool blend = false;
				if(alpha) // TODO - alpha + lights
				{
					glBlendFunc(GL_ONE, GL_ONE);
					glEnable(GL_BLEND);
					blend = true;
				}
				else
				if(tex>0) // blend - mult
				{
					glBlendFunc(GL_ONE, GL_ONE);
					glEnable(GL_BLEND);
					blend = true;
				}

				if(normalMap>0 && conf->geti("drawBump"))
				{
					drawBump(e, normalMap, e.matrix, it->pos);
					tex++;
				}
				else if(e.mapcoord && colorMap>0 && conf->geti("drawLight") ) // Light map (Diffuse)
				{
					if(drawDiffuse(e, e.matrix, it->pos))
						tex++;
				}
	
				if(blend)
		            glDisable(GL_BLEND);

		}

		if(e.mapcoord && colorMap>0  && conf->geti("drawTexture")) // Color map
		{
			bool blend = false;
			if(alpha)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
				blend = true;
			}
			else
			if(tex>0) // blend - mult
			{
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				glEnable(GL_BLEND);
				blend = true;
			}

		    glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, colorMap);
			glTexCoordPointer(2, GL_FLOAT, 2*4, &e.mapcoord->front());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			drawFaces(e);
		    glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			if(blend)
	            glDisable(GL_BLEND);
			tex++;
		}


/*		if(e.mapcoord && cubeMap==0 && conf->geti("drawLigthDist")) // Distance from light
		{
			if(tex>0)
			{
				glDepthFunc(GL_LEQUAL); // For blending
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				glEnable(GL_BLEND);
			}

			drawDistColor(e, e.matrix, light[0].pos, light[0].range);

			if(tex>0)
	            glDisable(GL_BLEND);

			tex++;
		}
*/

/*		glDisable(GL_BLEND);
		if(e.mapcoord && illuminateMap>0 && conf->geti("drawIlluminate"))
		{
			if(tex>0) // blend - additive
			{
				glDepthFunc(GL_LEQUAL); // For blending
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
			}

		    glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, illuminateMap);
			glTexCoordPointer(2, GL_FLOAT, 2*4, &e.mapcoord->front());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			drawFaces(e);

		    glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			
			if(tex>0)
				glDisable(GL_BLEND);

			tex++;
		}

		if(cubeMap>0 && conf->geti("drawReflect"))
		{
			if(tex>0)
			{
				glDepthFunc(GL_LEQUAL); // For blending
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
			}
			drawReflect(e, cubeMap, e.matrix, camera.eye);
			if(tex>0)
				glDisable(GL_BLEND);

			tex++;
		}
*/
		if(conf->geti("drawWire"))
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glPolygonMode(GL_FRONT, GL_LINE);  		// Draw Polygons As Wireframes
			glPolygonMode(GL_BACK, GL_LINE); 
			glDepthFunc(GL_LEQUAL); // For blending
	
			drawFaces(e);

			glPolygonMode(GL_FRONT, GL_FILL);    	// Reset Back-Facing Polygon Drawing Mode
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		if(conf->geti("drawNormals", 0))
			drawNormals(e);
		if(conf->geti("drawAABB", 0))
			drawAABB(e, e.matrix);

	}
}


v3 getstangent(v2 A, v3 B, v3 N, v2 S)
{
    A.normalize();
//    Vector2 S(1.0, 0.0); // Os teksturnih koordinat OX
    float sina = A.pseudoscalarProduct(S); // Vect mull
    float cosa = A.dotProduct(S); // skalarnoe
    // a - ugon mezhdu S i A
    // esli v normalnom prostranstve povernut' vektor B na ugol -a (minus a), to mipolu4im vektor napravlennij v storonu uveli4enija koordinati S (sTangent)

//    return B.rotate(N, sina, cosa);

    matrix44 M;
    M.setRotationAxis(sina, cosa, v3(N.x, N.y, N.z)); // povernut' na ang v ploskoti perpedukularnoj N

    v4 V(B.x, B.y, B.z, 1.0), R;
    R = M*V;
    v3 Res(R.x, R.y, R.z);
    Res.normalize();
    return Res;

}



/*void OpenGL_Engine::getTangentSpace(Vertexes const *vertex, MapCoords const *mapcoord, Triangles const *triangle, Normals	const *normal,	std::vector<v3> **sTangent, std::vector<v3> **tTangent)
{
	// TODO: hash
	if(tangentSpaceCacheS.find((int)vertex) != tangentSpaceCacheS.end())
	{
		*sTangent = tangentSpaceCacheS[(int)vertex];
		*tTangent = tangentSpaceCacheT[(int)vertex];
		return;
	}

	if(!vertex || !mapcoord || mapcoord->empty()) return;

    unsigned int size = vertex->size();
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

        f = b;  s[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        // vertex a (vector ac)
        f = c;  s[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = b;  t[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
        f = c;  t[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));

        e = b;
		me = mapcoord->operator[](e);ve = vertex->operator[](e);
		ne = normal->operator[](e);
        f = a;  s[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = c;  s[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = a;  t[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
        f = c;  t[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));

        e = c;
		me = mapcoord->operator[](e);ve = vertex->operator[](e);
		ne = normal->operator[](e);
        f = a;  s[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = b;  s[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(1.0, 0.0));
        f = a;  t[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
        f = b;  t[e] += getstangent(mapcoord->operator[](f)-me, vertex->operator[](f) - ve, ne, v2(0.0, -1.0));
    }
    for (unsigned int i=0; i<size; i++)
    {
        s[i].normalize();
        t[i].normalize();
    }
	*sTangent = S;	tangentSpaceCacheS[(int)vertex] = S;
	*tTangent = S;	tangentSpaceCacheT[(int)vertex] = T;
};
*/

/*void OpenGL_Engine::genTangentSpaceLight(std::vector<v3> const &sTangent, std::vector<v3> const &tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix44 const matrix, const v3 light,	v3List **tangentSpaceLight)
{
	matrix44 inverseModelMatrix;
    inverseModelMatrix = matrix.getInverse();

	v3 objectLightPosition = inverseModelMatrix*light;

	*tangentSpaceLight = new v3List(vertex.size());
	v3List &tl = **tangentSpaceLight;

    // vi4isljaem vektor napravlennij na isto4nik sveta v tangensnom prostranstve kazhdoj ver6ini
    for (unsigned int i=0; i<vertex.size(); i++)
    {
		v3 lightVector =  objectLightPosition - vertex[i];

		tl[i].x = sTangent[i].dotProduct(lightVector); // scalar product
		tl[i].y = tTangent[i].dotProduct(lightVector);
		tl[i].z =   normal[i].dotProduct(lightVector);
    }
}

void OpenGL_Engine::genTangentSpaceSphere(std::vector<v3> const &sTangent, std::vector<v3> const &tTangent, Vertexes const &vertex, Normals	const &normal, matrix44 const matrix, const v3 _camera,	v3List **tangentSpaceLight)
{
	matrix44 inverseModelMatrix;
    inverseModelMatrix = matrix.getInverse();

	v3 camera = inverseModelMatrix*_camera;

	*tangentSpaceLight = new v3List(vertex.size());
	v3List &tl = **tangentSpaceLight;

    // vi4isljaem vektor napravlennij na isto4nik sveta v tangensnom prostranstve kazhdoj ver6ini
    for (unsigned int i=0; i<vertex.size(); i++)
    {
		v3 c = camera - vertex[i];

        c.normalize();

        v3 d = (matrix*(vertex[i]+normal[i]))- matrix*vertex[i];
        d.normalize(); // realnaja normal'

        float pscale = c.dotProduct(d); // cos ugla mezhdu c i nermal

        v3 p = d*pscale;
//        VECTOR3D x = (p - c);

//		tangentSpaceLight[i] = - p - p + c;
		tl[i] = p + p - c;
//        tangentSpaceLight[i] = -(tangentSpaceLight[i]+d);
//		tangentSpaceLight[i] = x-c;
//		tangentSpaceLight[i] = ModelMatrix*vertex[i];

        tl[i].normalize();
    }
}





void OpenGL_Engine::drawDistColor(DrawElement &e, matrix44 const matrix, v3 const light, float const distance)
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
*/

/*bool OpenGL_Engine::drawDiffuse(DrawElement &e, matrix44 const matrix, v3 const light)
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
*/

/*
void OpenGL_Engine::drawBump(DrawElement &e, GLuint normalMap, matrix44 const matrix, v3 const light)
{
	v3List *sTangent, *tTangent, *tangentSpaceLight;

//	getTangentSpace(e.vertex, e.mapcoord, e.triangle, e.normal, &sTangent, &tTangent);
	genTangentSpaceLight(*sTangent, *tTangent, *e.vertex, *e.normal, matrix, light, &tangentSpaceLight);

	glBindTexture(GL_TEXTURE_2D, normalMap);

	glEnable(GL_TEXTURE_2D);

	//Bind normalisation cube map to texture unit 1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glActiveTextureARB(GL_TEXTURE0_ARB);

 	//Send texture coords for normal map to unit 0
//    glTexCoordPointer(2, GL_FLOAT, 8, &e.mapcoord->front());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Send tangent space light vectors for normalisation to unit 1
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(3, GL_FLOAT, 12, &tangentSpaceLight->front());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);

//Set up texture environment to do (tex0 dot tex1)*color
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);

	glActiveTextureARB(GL_TEXTURE1_ARB);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);

	glActiveTextureARB(GL_TEXTURE0_ARB);

    drawFaces(e);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

//Return to standard modulate texenv
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	delete tangentSpaceLight;
}*/

/*
void OpenGL_Engine::drawReflect(DrawElement &e, GLuint cubeMap, matrix44 const matrix, v3 const light)
{
	v3List *sTangent, *tTangent, *tangentSpaceLight;

//	getTangentSpace(e.vertex, e.mapcoord, e.triangle, e.normal, &sTangent, &tTangent);
	
	genTangentSpaceSphere(*sTangent, *tTangent, *e.vertex, *e.normal, matrix, light, &tangentSpaceLight);

	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMap);
	glTexCoordPointer(3, GL_FLOAT, 12, &tangentSpaceLight->front());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    drawFaces(e);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	delete tangentSpaceLight;
}
*/

void OpenGL_Engine::drawFaces(DrawElement &e)
{
	glDrawElements(GL_TRIANGLES, e.triangle->size()*3, GL_UNSIGNED_INT, &(e.triangle->front()));
}

void OpenGL_Engine::drawNormals(DrawElement &e)
{
	aabb &f = e.frame;
	float diag = (f.max-f.min).getLength()*0.05f;

		glBegin(GL_LINES);
		for(unsigned int i=0; i < e.vertex->vertex.size(); i++)
		{
			v3 s = e.vertex->vertex[i];
			v3 d = e.vertex->vertex[i] + e.normal->operator [](i)*diag;

			glVertex3f(s.x, s.y, s.z);
			glVertex3f(d.x, d.y, d.z);
		}
		glEnd();
}

void OpenGL_Engine::drawAABB(DrawElement &e, matrix44 matrix)
{
	vector<v3> v = e.frame.getVertexes();
	aabb c;

	for(int i=0; i<8; i++)
		c.merge(matrix*v[i]);
	

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);

	glVertex3f(c.min.x, c.min.y, c.min.z);	glVertex3f(c.max.x, c.min.y, c.min.z);
	glVertex3f(c.min.x, c.min.y, c.min.z);	glVertex3f(c.min.x, c.max.y, c.min.z);
	glVertex3f(c.min.x, c.min.y, c.min.z);	glVertex3f(c.min.x, c.min.y, c.max.z);

	glVertex3f(c.min.x, c.max.y, c.max.z);	glVertex3f(c.max.x, c.max.y, c.max.z);
	glVertex3f(c.max.x, c.min.y, c.max.z);	glVertex3f(c.max.x, c.max.y, c.max.z);
	glVertex3f(c.max.x, c.max.y, c.min.z);	glVertex3f(c.max.x, c.max.y, c.max.z);

	glVertex3f(c.min.x, c.min.y, c.max.z);	glVertex3f(c.max.x, c.min.y, c.max.z);
	glVertex3f(c.min.x, c.max.y, c.min.z);	glVertex3f(c.max.x, c.max.y, c.min.z);

	glVertex3f(c.min.x, c.min.y, c.max.z);	glVertex3f(c.min.x, c.max.y, c.max.z);
	glVertex3f(c.max.x, c.min.y, c.min.z);	glVertex3f(c.max.x, c.max.y, c.min.z);

	glVertex3f(c.min.x, c.max.y, c.min.z);	glVertex3f(c.min.x, c.max.y, c.max.z);	
	glVertex3f(c.max.x, c.min.y, c.min.z);	glVertex3f(c.max.x, c.min.y, c.max.z);	

	glEnd();

	glPopMatrix();
}


GLuint OpenGL_Engine::getTexture(Image *image)
{
	if(image == NULL) return 0;
	uid id = image->getId();
	if(id == 0) return 0;

	if(registredTextures.find(id) != registredTextures.end())
		return registredTextures[id];

	GLuint glid;
	glGenTextures(1, &glid);
	glBindTexture(GL_TEXTURE_2D, glid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //{ all of the above can be used }

//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int kind;
	if(image->getBpp() == 24) kind = GL_RGB;
	else
	if(image->getBpp() == 32) 
		kind = GL_RGBA;
	else return 0;

	glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image->getWidth(), image->getHeight(),0,
		kind,  GL_UNSIGNED_BYTE , image->getBitmap());

	registredTextures[id] = glid;
//	alog.out("Bind texture %s", id);

	return id;
}


/*
GLuint OpenGL_Engine::getCubeMap(std::string imageName)
{
	if(registedCubeMaps.find(imageName) != registedCubeMaps.end())
		return registedCubeMaps[imageName];


	char *a[] = {"bk", "ft", "lf", "rt", "up", "dn"};
	int b[6] = 
	{
GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB
	};



	for(int i=0; i<6; i++)
		if(a[i]==0)
		{
			alog.out("Cannot find cube map %s", imageName.c_str());
			return 0;
		}

    glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, id);


	for(int i=0; i<6; i++)
	{
		Image *m = (Image*)res->add(Res::image, imageName+a[i]);
		glTexImage2D(b[i],	0, GL_RGBA8, m->getWidth(), m->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m->getBitmap());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glDisable(GL_TEXTURE_CUBE_MAP_ARB);


	registedCubeMaps[imageName] = id;
	alog.out("Bind cubemap %s", imageName.c_str());
	return id;
}
*/

bool OpenGL_Engine::process()
{
	// TODO repair DC 
	for(vector<DrawElement>::iterator it = element.begin(); it != element.end(); it++)
		delete it->triangle;

	element.clear();

	light.clear();
	total.vertex = 0;
	total.triangle = 0;
	total.object = 0;

	for(std::vector<GraphInterface*>::iterator it = objects.begin(); 
			it != objects.end(); it++)
		prepare((*it));

//	if(!ARB_multitexture_supported) 
//		conf->setup("drawBump", 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<DrawElement> elementAlpha;

// В начале выводим только непрозрачные объекты
	for(vector<DrawElement>::iterator it = element.begin(); it != element.end(); it++)
		if(!it->alpha)
			drawElement((*it));
		else
		{
			it->distance = (camera.eye - it->matrix*v3()).getLength();
			elementAlpha.push_back(*it);
		}

// Потом прозрачные в порядке удалённости от камеры: вначале самые дальние

	if(conf->geti("drawAlpha")>0)
	{
		sort(elementAlpha.begin(), elementAlpha.end());

		for(vector<DrawElement>::iterator it = elementAlpha.begin(); it != elementAlpha.end(); it++)
			drawElement((*it));
	}

	elementAlpha.clear();

	glFlush();
	swapBuffers();

	return true;
}



bool OpenGL_Engine::init(std::string _conf)
{
	if(!res->add(Res::config, _conf))
	{
		alog.msg("error graph conf res","Cannot find renderer config file "+_conf);
		return false;
	}

	conf = (Config*)res->get(Res::config, _conf);

	conf->setDefault("window.left", "10");
	conf->setDefault("window.top", "10");

	conf->setDefault("window.width", "800");
	conf->setDefault("window.height", "600");

	conf->setDefault("screen.depth", "32");
	conf->setDefault("screen.width", "800");
	conf->setDefault("screen.height", "600");

	conf->setDefault("fullscreen", "0");

	if (!createWindow())
	{
		lastError = getError();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.01, 1.0e+6);

	glDepthFunc(GL_LESS); 
	glCullFace(GL_BACK);

//    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	initExtensions();

//	SetUpARB_multitexture();
	
//	normalisationCubeMap	= GenerateNormalisationCubeMap();
	//lightCubeMap			= GenerateLightCubeMap();
	//distMap					= generateDistanceLinearMap();

	alog.out("OpenGL engine has been initialized!\n");

	setCaption("Steel Engine");

	return true;
}

bool OpenGL_Engine::deinit()
{
	alog.out("OpenGL engine has been stopped!\n");
	return true;
}


void OpenGL_Engine::processCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
//	gluPerspective( camera.fov, (float)window.width/window.height, front, back );

	conf->setup("camera.aspect", conf->getd("window.width") / conf->getd("window.height"));

	gluPerspective(conf->getd("camera.fov"), conf->getd("camera.aspect"), conf->getd("camera.min_dist"), conf->getd("camera.max_dist"));

    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, 
			camera.center.x, camera.center.y, camera.center.z, 
			camera.up.x, camera.up.y, camera.up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

