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

	bool loaded = false;
	if(buffer.find(id) != buffer.end())
		loaded = buffer[id].loaded;

	OpenGLBuffer &buf = buffer[id];

	if(loaded)
	{
		switch(image->getKind())
		{
			case ImageKind::image2d:
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, buf.glid);
				break;
			case ImageKind::cube:
				glEnable(GL_TEXTURE_CUBE_MAP_ARB);
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, buf.glid);
				break;
		}

		buf.lastUsedTime = time;
		buf.usedCnt++;
	}
	else
	{
		glGenTextures(1, &buf.glid);

		int format;
		if(image->getFormat() == ImageFormat::rgb) format = GL_RGB; 
		else if(image->getFormat() == ImageFormat::rgba) format = GL_RGBA;
		else return false;

		switch(image->getKind())
		{
			case ImageKind::image2d:
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, buf.glid);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


				glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, image->getWidth(), image->getHeight(),0,
					format,  GL_UNSIGNED_BYTE , image->getBitmap());
				break;
			case ImageKind::cube:
				glEnable(GL_TEXTURE_CUBE_MAP_ARB);
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, buf.glid);

				int w = image->getWidth();
				int h = image->getHeight();
				int bpp = image->getBpp()/8;

				if(w*6 != h) return false; // 6 images in one

				for(int i=0; i<6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i,	0, 
						GL_RGBA8, w, w, 0, format, GL_UNSIGNED_BYTE, image->getBitmap() + w*w*bpp*i);
				}


				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				break;
		}

		buf.loaded = true;
		buf.loadCnt++;
		buf.kind = OpenGLBuffer::image;
		buf.lastUsedTime = time;
		buf.usedCnt++;
	}
	return true;
}



template<class Class> bool OpenGL_Engine::bind(Class *v, int mode, int mode2, int elCnt)
{
	if(v == NULL) return false;

	if(glGenBuffersARB) // Vertex Buffer Object supportd
	{
		uid	id = v->getId();
		if(id == 0) 
		{
			glBindBufferARB(mode2, 0);
			return false;
		}

		bool loaded = false;
		if(buffer.find(id) != buffer.end())
			loaded = buffer[id].loaded;

		OpenGLBuffer &buf = buffer[id];

		if(loaded)
		{
			glBindBufferARB(mode2, buf.glid);
			if(v->changed)
			{
				glBufferSubDataARB(mode2, 0, elCnt*sizeof(float)*v->data.size(), &v->data.front());
				buf.loadCnt++;
			}

			if(mode)glEnableClientState(mode);

			buf.lastUsedTime = time;
			buf.usedCnt++;
		}
		else
		{
			glGenBuffersARB(1, &buf.glid);

			if(mode) glEnableClientState ( mode );
			glBindBufferARB(mode2, buf.glid);

			GLenum usage = GL_STATIC_DRAW;
			if(v->changed)
				usage = GL_STREAM_DRAW;
			glBufferDataARB(mode2, elCnt*sizeof(float)*v->data.size(), &v->data.front(), usage);

			buf.loaded = true;
			buf.loadCnt++;
			if(mode2 == GL_ARRAY_BUFFER_ARB)
				buf.kind = OpenGLBuffer::array;
			if(mode2 == GL_ELEMENT_ARRAY_BUFFER_ARB)
				buf.kind = OpenGLBuffer::index;

			buf.lastUsedTime = time;
			buf.usedCnt++;
		}
		return true;
	}  
	else
		return false;
}

/*
Сердце Графического движка.
Отвечает за вывод графичесткого элемента.
Графических элемент - это полигоны одного объекта, имеющие общий материал.
*/

void OpenGL_Engine::drawElement(DrawElement &e)
{
//  загружает матрицу преобразрвания для объекта (перенос, масштаб, поворот) в глобальых координатах
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glLoadMatrixf(e.matrix.a);

	if(e.triangle && e.vertex && !e.vertex->data.empty() && !e.triangle->data.empty())// если есть полигоны и вершины
	{
		Material *m = e.material; // получаем материал
		if(m != NULL  && conf->geti("drawTexture", 1))
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			// загружаем вершины объекта

			if(bind(e.vertex, GL_VERTEX_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
				glVertexPointer(3, GL_FLOAT, 0, 0);
			else
			{
				glEnable(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data.front());
			}

			// загружаем нормали объекта
			if(e.normal && !e.normal->data.empty())
			{
				if(bind(e.normal, GL_NORMAL_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
					glNormalPointer(GL_FLOAT, 0, 0);
				else
				{
					glEnable(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT, 0, &e.normal->data[0]);
				}
			}

			int texCount = m->map.size();

			// идём по всем картам
			for(int i=0; i<texCount; i++)
			{
				Map map = m->map[i]; // текущая текстура

				if(glActiveTextureARB)
				{
					glActiveTextureARB(GL_TEXTURE0_ARB + i);
					glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
				}
				else if(i>0) break;

				if(map.kind == MapKind::color_map) // обычная текстура
				{	// загружем текстуру
					bindTexture(map.texture); // 2D texture (auto detect from Image)
					// загружаем тектурные координаты
					TexCoords *coords = e.object->getTexCoords(i);
					if(coords && !coords->data.empty())
					{
						if(bind(coords, GL_TEXTURE_COORD_ARRAY, GL_ARRAY_BUFFER_ARB, 2))
							glTexCoordPointer(2, GL_FLOAT, 0,0);
						else
						{
							glEnable(GL_TEXTURE_COORD_ARRAY);
							glTexCoordPointer(2, GL_FLOAT, 0, &coords->data[0]);
						}
					}
				}
				if(map.kind == MapKind::env) // карта отражения
				{ // загружаем текстуру
					bindTexture(map.texture); // Cube texture (auto detect from Image)
					// TODO: в этом месте тектурные координаты должны генерированться сами
					//  шейдером или еще чем-либо
					glTexGeni ( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB );
				    glTexGeni ( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB );
					glTexGeni ( GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB );

					glEnable  ( GL_TEXTURE_GEN_S );
					glEnable  ( GL_TEXTURE_GEN_T );
					glEnable  ( GL_TEXTURE_GEN_R );
				}

				if(map.kind == MapKind::color) // цвет RGBA
					glColor4f(map.color.x, map.color.y, map.color.z, map.color.w);

				// режим мультитекстурирования
				GLint mode = GL_REPLACE;
				if(map.mode == MapMode::mul && i>0) mode = GL_BLEND;
				if(glActiveTextureARB)
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
			}

			if(e.blend)
			{
				glEnable(GL_BLEND);
				
				if(m->map[0].kind == MapKind::color_map && m->map[0].texture->getBpp() == 24) // RGB
				{
					if(m->map[0].mode == MapMode::add)	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
					if(m->map[0].mode == MapMode::mul)	glBlendFunc(GL_DST_COLOR, GL_ZERO);
				}else
//				if(m->map[0].texture->getBpp() == 32) // Alpha
				{
					if(m->map[0].mode == MapMode::add)	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA); // ?
					if(m->map[0].mode == MapMode::mul)	glBlendFunc(GL_DST_ALPHA, GL_ZERO);
				}

//				glDepthFunc(GL_ALWAYS);
				glDepthMask(0);
			}
// -------------------------------------------------------------------------------
			// загружаем и ресуем треугольники
			if(bind(e.triangle, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 3))
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, 0);
			else
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, &e.triangle->data.front());

// -------------------------------------------------------------------------------
			// откат настроек
			glPopAttrib();
		}

		if(conf->geti("drawWire"))
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			if(e.material && !e.material->map.empty() &&e.material->map[0].kind == MapKind::color) // цвет RGBA
					glColor4f(e.material->map[0].color.x, e.material->map[0].color.y, e.material->map[0].color.z, e.material->map[0].color.w);
			else
				glColor4f(1,1,1,1);

			// загружаем вершины объекта
			if(bind(e.vertex, GL_VERTEX_ARRAY, GL_ARRAY_BUFFER_ARB, 3))
				glVertexPointer(3, GL_FLOAT, 0, 0);
			else
			{
				glEnable(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, &e.vertex->data[0]);
			}
			glPolygonMode(GL_FRONT, GL_LINE);  		// Draw Polygons As Wireframes
			glPolygonMode(GL_BACK, GL_LINE); 
			glDepthFunc(GL_LEQUAL); // For blending

			if(bind(e.triangle, 0, GL_ELEMENT_ARRAY_BUFFER_ARB, 3))
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, 0);
			else
				glDrawElements(GL_TRIANGLES, e.triangle->data.size()*3, GL_UNSIGNED_INT, &e.triangle->data[0]);

			glPolygonMode(GL_FRONT, GL_FILL);  

			glPopAttrib ();
		}
	}

	if(conf->geti("drawNormals", 0))
		drawNormals(e);
	
	if(conf->geti("drawVertexes", 0))
		drawVertexes(e);

	if(conf->geti("drawAABB", 0))
		drawAABB(e, e.matrix);
	glPopMatrix();
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

/*void OpenGL_Engine::drawFaces(DrawElement &e)
{
	glDrawElements(GL_TRIANGLES, e.triangle->size()*3, GL_UNSIGNED_INT, &(e.triangle->front()));
}*/

void OpenGL_Engine::drawNormals(DrawElement &e)
{
	if(e.normal)
	{
		glColor3f(0,0,1);
		aabb &f = e.frame;
		float diag = (f.max-f.min).getLength()*0.05f;
		
//		if(diag<EPSILON) diag = 0.01;

		glBegin(GL_LINES);
		for(unsigned int i=0; i < e.vertex->data.size(); i++)
		{
			v3 s = e.vertex->data[i];
			v3 d = e.vertex->data[i] + e.normal->data[i]*diag;

			glVertex3f(s.x, s.y, s.z);
			glVertex3f(d.x, d.y, d.z);
		}
		glEnd();
	}
}

void OpenGL_Engine::drawVertexes(DrawElement &e)
{
	glPointSize(5);
	glColor3f(0.5,1,1);

	glBegin(GL_POINTS);
	for(unsigned int i=0; i < e.vertex->data.size(); i++)
	{
		v3 &s = e.vertex->data[i];

		glVertex3f(s.x, s.y, s.z);
	}
	glEnd();
}


void OpenGL_Engine::drawAABB(DrawElement &e, matrix44 matrix)
{
	aabb &c = e.frame;

	glColor3f(1.0f, 0.8f, 0.8f);

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



bool OpenGL_Engine::process()
{
	// TODO repair DC 
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

	GLbitfield clear = 0;
	if(conf->geti("clearColor", 1))	clear |= GL_COLOR_BUFFER_BIT;
	if(conf->geti("clearDepth", 1))	clear |= GL_DEPTH_BUFFER_BIT;
	if(clear)
		glClear(clear);

/*
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glLightfv( GL_LIGHT0, GL_POSITION, v4( camera.center ));*/


	std::vector<DrawElement> elementAlpha;

// В начале выводим только непрозрачные объекты
	for(vector<DrawElement>::iterator it = element.begin(); it != element.end(); it++)
		if(!it->blend)
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

	if(conf->geti("swapBuffers", 1))
	{
		glFlush(); // TODO: flush in thread
		swapBuffers();
	}

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

	clear();

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

