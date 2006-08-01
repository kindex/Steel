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

#include "../../steel.h"

#include "../graph_engine.h"
#if STEEL_OS == OS_LINUX
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#else
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "gl\glaux.h"		// Header File For The Glaux Library
#endif


class OpenGL_Engine: public GraphEngine
{
protected:
	struct OpenGL_Buffer
	{
		typedef enum 
		{
			none,
			image,
			array,
			index
		} buffer_kind;

		int		size;
		GLuint	glid;
		int		usedCnt, loadCnt;
		steel::time lastUsedTime;
		bool	loaded;
		buffer_kind kind;
	};



	std::map<uid, OpenGL_Buffer> buffer;
	GLuint normalisationCubeMap, lightCubeMap, distMap; // TODO: remove
	Image *zeroNormal;


public:
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

protected:
	// data, to store collected information
	steel::vector<GraphObjectStorage> storage;

	// procedure variables
	bool (OpenGL_Engine::*BindTexture)(Image *image);
	void (OpenGL_Engine::*DrawFill)(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawTriangles)(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*BindTexCoords)(TexCoords *coords, int TextureNumber);
	void (OpenGL_Engine::*DrawWire)(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawLines)(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawNormals)(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawVertexes)(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawAABB)(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.0
	bool BindTexture_OpenGL10(Image *image);
	void DrawFill_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
	void DrawTriangles_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void DrawWire_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void DrawLines_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void DrawNormals_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void DrawVertexes_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void DrawAABB_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.1
	bool BindTexture_OpenGL11(Image *image);
	void DrawTriangles_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void DrawWire_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void DrawLines_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);
	void BindTexCoords_OpenGL11(TexCoords *coords, int TextureNumber);

	// OpenGL 1.3
	void OpenGL_Engine::DrawFill_OpenGL13(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.5
	void DrawTriangles_OpenGL15(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void BindTexCoords_OpenGL15(TexCoords *coords, int TextureNumber);


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
	OpenGL_Engine(): 
		BindTexture(NULL), 
		DrawFill(NULL), 
		DrawTriangles(NULL),
		BindTexCoords(NULL),
		DrawLines(NULL), 
		DrawWire(NULL), 
		DrawNormals(NULL),
		DrawVertexes(NULL),
		DrawAABB(NULL) 
		{}

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

	template<class Class> bool BindVBO(Class *v, int mode, int mode2, int elCnt);
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
	virtual void cacheStorageObject(GraphObjectStorage &objectStorage);

};



#endif
