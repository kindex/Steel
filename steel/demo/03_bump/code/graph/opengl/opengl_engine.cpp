#include "opengl_engine.h"

#include <iostream>

#include "../../utils.h"
#include "../../common/logger.h"

#include "../../res/image/image.h"
#include "extensions.h"

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



void OpenGL_Engine::getTangentSpace(Vertexes const *vertex, MapCoords const *mapcoord, Triangles const *triangle, Normals	const *normal,
									std::vector<v3> **sTangent, std::vector<v3> **tTangent)
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
        s[i].LoadZero();
        t[i].LoadZero();
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
        s[i].Normalize();
        t[i].Normalize();
    }
	*sTangent = S;	tangentSpaceCacheS[(int)vertex] = S;
	*tTangent = S;	tangentSpaceCacheT[(int)vertex] = T;
};

void OpenGL_Engine::genTangentSpaceLight(
	std::vector<v3> const &sTangent, 
	std::vector<v3> const &tTangent, 
	Vertexes const &vertex, 
	Normals	const &normal,
	MATRIX4X4 const matrix, const v3 light,
	v3List **tangentSpaceLight)
{
	MATRIX4X4 inverseModelMatrix;
    inverseModelMatrix = matrix.GetInverse();

	v3 objectLightPosition = inverseModelMatrix*light;

	*tangentSpaceLight = new v3List(vertex.size());
	v3List &tl = **tangentSpaceLight;

    // vi4isljaem vektor napravlennij na isto4nik sveta v tangensnom prostranstve kazhdoj ver6ini
    for (unsigned int i=0; i<vertex.size(); i++)
    {
		v3 lightVector =  light - vertex[i];

		tl[i].x = sTangent[i] * lightVector; // scalar product
		tl[i].y = tTangent[i] * lightVector;
		tl[i].z =   normal[i] * lightVector;
    }
}


void OpenGL_Engine::drawBump(DrawElement &e, GLuint normalMap, MATRIX4X4 const matrix, v3 const light)
{
	v3List *sTangent, *tTangent, *tangentSpaceLight;

	getTangentSpace(e.vertex, e.mapcoord, e.triangle, e.normal, &sTangent, &tTangent);
	
	genTangentSpaceLight(*sTangent, *tTangent, *e.vertex, *e.normal, matrix, light, &tangentSpaceLight);

	glBindTexture(GL_TEXTURE_2D, normalMap);

	glEnable(GL_TEXTURE_2D);

	//Bind normalisation cube map to texture unit 1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
    glEnable(GL_TEXTURE_3D);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glActiveTextureARB(GL_TEXTURE0_ARB);

 	//Send texture coords for normal map to unit 0
    glTexCoordPointer(2, GL_FLOAT, 8, &e.mapcoord->front());
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
    glDisable(GL_TEXTURE_3D);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

//Return to standard modulate texenv
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	delete tangentSpaceLight;
}


void OpenGL_Engine::drawFaces(DrawElement &e)
{
	glDrawElements(GL_TRIANGLES, e.triangle->size()*3, GL_UNSIGNED_INT, &(e.triangle->front()));
}

void OpenGL_Engine::drawNormals(DrawElement &e)
{
		glBegin(GL_LINES);
		for(unsigned int i=0; i < e.vertex->size(); i++)
		{
			v3 s = e.vertex->operator [](i);
			v3 d = e.vertex->operator [](i) + e.normal->operator [](i)*5;

			glVertex3f(s.x, s.y, s.z);
			glVertex3f(d.x, d.y, d.z);
		}
		glEnd();
}

void OpenGL_Engine::drawElement(DrawElement &e)
{
	glLoadMatrixf(e.matrix.entries);
	if(e.triangle && e.vertex)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 12 /* sizeof(v3)*/ , &e.vertex->front());

		Material *m = (Material*)res->get(Res::material, e.material);
		if(m == NULL)
		{
			alog.out("Cannot find material '%s'", e.material.c_str());
			return;
		}

		GLuint colorMap = 0;

		if(m->var_s->find("color_map") != m->var_s->end())
			colorMap = getTexture(m->var_s->operator []("color_map"));

		GLuint normalMap = 0;
		if(m->var_s->find("normal_map") != m->var_s->end())
			normalMap = getTexture(m->var_s->operator []("normal_map"));

	    bool blend = false;

		if(normalMap)
		{
			drawBump(e, normalMap, e.matrix, light[0].pos);
		}

		if(e.mapcoord && normalMap)
		{
			glDepthFunc(GL_LEQUAL); // For blending
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
//           glBlendFunc(GL_ONE, GL_ONE);
			glEnable(GL_BLEND);
			blend = true;
		}

		if(e.mapcoord && colorMap>0)
		{
		    glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, colorMap);
			glTexCoordPointer(2, GL_FLOAT, 2*4, &e.mapcoord->front());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		drawFaces(e);
		if(e.mapcoord)
		{
		    glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (blend)
		{
            glDisable(GL_BLEND);
            blend = false;
		}

		glDisableClientState(GL_VERTEX_ARRAY);
	}
}


GLuint OpenGL_Engine::getTexture(std::string imageName)
{
	if(registredTextures.find(imageName) != registredTextures.end())
		return registredTextures[imageName];

	res->add(Res::image, imageName);

	Image *a = (Image*)res->get(Res::image, imageName);

	if(a == NULL)
	{
		res->add(Res::normalMap, imageName);
		a = (Image*)res->get(Res::normalMap, imageName);
	}

	if(a == NULL)
	{
		alog.out("Cannot find image %s", imageName.c_str());
		return 0;
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //{ all of the above can be used }

//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, a->width, a->height,0,
		GL_RGB,  GL_UNSIGNED_BYTE , a->bitmap);

	registredTextures[imageName] = id;
	alog.out("Bind texture %s", imageName.c_str());

	return id;
}

bool OpenGL_Engine::process()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for(vector<DrawElement>::iterator it = elements.begin(); it != elements.end(); it++)
		drawElement((*it));

	glFlush();
	swapBuffers();

	return true;
}



bool OpenGL_Engine::init()
{
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

	SetUpARB_multitexture();
	makeNormalisationCubeMap();

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

	gluPerspective(90.0, 1.0, 0.01, 1.0e+6);


    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, 
			camera.center.x, camera.center.y, camera.center.z, 
			camera.up.x, camera.up.y, camera.up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void OpenGL_Engine::makeNormalisationCubeMap()
{
	glGenTextures(1, &normalisationCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	GenerateNormalisationCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}