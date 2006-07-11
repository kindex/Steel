/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		Графический движок реализлванный на фунциях OpenGL. 
		Рисует трехмерный объект в виде 3d-mesh,
		расчитывает динамическое освещение
 ************************************************************/

#ifndef __GRAPH_OPENGL_ENGINE_H
#define __GRAPH_OPENGL_ENGINE_H

#include <string>

#include "../graph_engine.h"
#ifdef __linux
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#else
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "gl\glaux.h"		// Header File For The Glaux Library
#endif

struct OpenGLBuffer
{
	typedef enum 
	{
		none,
		image,
		array,
		index
	} buffer_kind;

	GLuint	glid;
	int		usedCnt, loadCnt;
	steel::time lastUsedTime;
	bool	loaded;
	buffer_kind kind;
};

class OpenGL_Engine: public GraphEngine
{
protected:
	std::map<uid, OpenGLBuffer> buffer;
	GLuint normalisationCubeMap, lightCubeMap, distMap; // TODO: remove
	Image *zeroNormal;

	struct GraphObjectStorage // множество треугольников одного материала
	{
		// инентификатор объекта (uid)
		uid objectId;
		int storageId; // индекс этой структуры (кеша) в массиве stroage
		GraphObject *object;
		
		// время последнего изменения объекта. Если отлично от того, что возвращает PhysicObject::getModificationTime(), то надо обновить кеш.
		ModificationTime modificationTime, childrenModificationTime;

		// список детей объекта (uid)
		steel::svector<uid> children;

		// *** Polyhedra ****
		FaceMaterials *faceMaterials;
		Vertexes	*vertex;
		Normals		*normal;

		GLines		*lines;

		matrix34	matrix;
		aabb		frame;
		bool		blend; // true if blending
		bool		visible;
		coord		distance; // расстояние до камеры

//		bool	operator < (const DrawElement &sec) const { return distance > sec.distance; }
	};

	// data, to store collected information
	steel::vector<GraphObjectStorage> storage;


	typedef
		steel::vector<v3>
		v3List;

	struct tangentSpaceLightBufferedArray: public BufferedElement
	{
		v3List data;
	} ;


	struct TangentSpaceCache
	{
		v3List *s,*t;
	};

	typedef 
		std::map<int, TangentSpaceCache>
		tangentCache;

	 tangentCache tangentSpaceCache;

	 bool focused;
	 steel::time time;

public:
	void processCamera();
	bool init(std::string _conf);
	bool process(steel::time globalTime, steel::time time);
	bool deinit();

	void prepare(GraphObject *object, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), GraphObject *parent = NULL);

	virtual void swapBuffers() = 0;
	virtual bool createWindow() = 0;
	virtual void setCaption(std::string caption) = 0;
	virtual bool isFocused() { return focused; }
	
	bool isVisible(aabb box);

//	void process(GraphObjectStorage &e);
	void process(GraphObjectStorage &e, steel::time globalTime, steel::time time);
	void DrawOpenGL10(GraphObjectStorage &e, Triangles *triangles, Material *material);
	void DrawOpenGL10Lines(GraphObjectStorage &e);
	void DrawOpenGL10Wire(GraphObjectStorage &e, Triangles *triangles);
	
//	void drawFaces(DrawElement &e);
//	void drawNormals(DrawElement &e);
//	void drawVertexes(DrawElement &e);
//	void drawAABB(DrawElement &e, matrix34 matrix);

	
//	void drawBump(DrawElement &e, TexCoords *coords, matrix34 const matrix, v3 const light, uid bufId, int texnum, Image *bump);
//	void drawReflect(DrawElement &e, matrix34 const matrix, v3 const light, uid bufId);

//	bool drawDiffuse(DrawElement &e, matrix34 const matrix, v3 const light);
//	void drawDistColor(DrawElement &e, matrix34 const matrix, v3 const light, float const distance);

//	void getTangentSpace(Vertexes const *vertex, TexCoords const *mapcoord, Triangles const *triangle, Normals const *normal, steel::vector<v3> **sTangent, steel::vector<v3> **tTangent);
//	void genTangentSpaceLight(steel::vector<v3> const &sTangent, steel::vector<v3> const &tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light,	v3List &tangentSpaceLight);
//	void genTangentSpaceSphere(Vertexes const &vertex, Normals	const &normal, matrix34 const matrix, const v3 _camera,	v3List &tangentSpaceLight);

	//void genTangentSpaceSphere(steel::vector<v3> const &sTangent, steel::vector<v3> const &tTangent, Vertexes const &vertex, Normals	const &normal, matrix34 const matrix, const v3 camera,	v3List **tangentSpaceLight);

//	GLuint getCubeMap(std::string imageName);

	bool bindTexture(Image *image);
	template<class Class> bool bind(Class *v, int mode, int mode2, int elCnt);
	void cleanBuffer(uid bufId);
/*	bool bindTexCoords(MapCoord *coord);
	bool bindVertexes(Vertexes *v);*/

	GraphObjectStorage &getStorage(GraphObject *object);
	
		// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	void makeStorageForObject(GraphObject *object);
	void deleteStorageForObject(int sid);
	// создаёт место для хранения дополнительной инормации (storage, кеш объекта) - для детей объекта
	void makeStorageForChildren(GraphObject *object);
	void deleteStorageForChildren(int sid);

	// овновляюет место для хранения дополнительной инормации (storage, кеш объекта) - для одного объекта
	void cacheStorageObject(GraphObjectStorage &objectStorage);

};



#endif
