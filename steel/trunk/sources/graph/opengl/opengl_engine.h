/*id*********************************************************
	File: graph/opengl/opengl_engine.h
	Unit: opengl
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

#include "../../steel.h"

#include <string>

#if STEEL_OS == OS_LINUX
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#elif STEEL_OS == OS_WIN32
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "gl\glaux.h"		// Header File For The Glaux Library
#endif

#include "../graph_engine.h"
#include "../../input/input.h"
#include "opengl_glsl.h"

class OpenGL_Engine: public GraphEngine
{
protected:
	GameObject *currentObject;
	struct GraphShadow;
	GraphShadow *currentShadow;

public: // interface realization
	bool setCurrentObject(GameObject*);
	void setPosition(ObjectPosition);
	void setPositionKind(PositionKind);
	void setVertexes(const Vertexes*); // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	void setNormals(const Normals*); // список нормалей в вершинам
	void setLines(const GLines*); // индексы вершин для линий и цвета линий (for debug)
	void setFaceMaterials(const FaceMaterials*);// массив индексов вершин, которые образуют треугольники (грани) + материалы
	void setTexCoordsCount(unsigned int);
	void setTexCoords(unsigned int texNumber, const TexCoords*);
	void setLights(const Lights*);

	void addChild(GameObject* child);
	void deleteChild(GameObject* child);
	void clearChildren(void);


protected:
	struct OpenGL_Buffer
	{
		typedef enum 
		{
			none,
			image,
			array,
			index,
			shader
		} buffer_kind;

		int		size;
		GLuint	glid;
		int		usedCnt, loadCnt;
		steel::time lastUsedTime;
		bool	loaded;
		buffer_kind kind;
		char	*object;
	};

#define GS(shadow) (static_cast<GraphShadow*>(shadow))

	struct GraphShadow: public Shadow // множество треугольников одного материала
	{
		ObjectPosition	position; // global or screen
		PositionKind	positionKind;

		// *** Polyhedra ****
		const FaceMaterials *faceMaterials;
		const Vertexes		*vertexes;
		const Normals		*normals;
		const Lights		*lights;
		unsigned int textureCount;
		svector<const TexCoords*> texCoords;

		const GLines		*lines;

		aabb		frame;
		bool		blend; // true if blending
		bool		visible;
		float		distance; // расстояние до камеры

		GraphShadow(Engine *engine): Shadow(engine), faceMaterials(NULL), vertexes(NULL), normals(NULL), lights(NULL),
			lines(NULL), position(ObjectPosition::getIdentity() ) {}
		void fill(GameObject *object);
		bool cache(void);
//		bool	operator < (const DrawElement &sec) const { return distance > sec.distance; }
	};

	void addChild(GraphShadow &, GameObject*);

	std::map<uid, OpenGL_Buffer> buffer;

	Lights lights;
	GLuint normalisationCubeMap, lightCubeMap, distMap; // TODO: remove
	Image *zeroNormal;


protected:
	// procedure variables
	bool (OpenGL_Engine::*BindTexture)(Image *image, bool enable);
	void (OpenGL_Engine::*DrawFill)(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawTriangles)(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total);

	void (OpenGL_Engine::*BindTexCoords)(const TexCoords *coords);
	void (OpenGL_Engine::*BindTexCoords3f)(const TexCoords3f *coords);

	void (OpenGL_Engine::*DrawWire)(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawLines)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawNormals)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawVertexes)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawAABB)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.0
	bool BindTexture_OpenGL10(Image *image, bool enable);
	void DrawFill_OpenGL10(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
	void DrawTriangles_OpenGL10(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void DrawWire_OpenGL10(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void DrawLines_OpenGL10(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void DrawNormals_OpenGL10(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void DrawVertexes_OpenGL10(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void DrawAABB_OpenGL10(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.1
	bool BindTexture_OpenGL11(Image *image, bool enable);
	void DrawTriangles_OpenGL11(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void DrawWire_OpenGL11(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void DrawLines_OpenGL11(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void BindTexCoords_OpenGL11(const TexCoords *coords);
	void BindTexCoords3f_OpenGL11(const TexCoords3f *coords);

	// OpenGL 1.3
	void DrawFill_OpenGL13(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.5
	void DrawTriangles_OpenGL15(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void BindTexCoords_OpenGL15(const TexCoords *coords);
	void BindTexCoords3f_OpenGL15(const TexCoords3f *coords);

	// OpenGL 2.0
	void DrawFill_OpenGL20(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
	GLSL *BindShader(Material *shader);

	// Stuff to delete

	typedef
		steel::vector<v3>
		v3List;

	typedef TexCoords3f tangentSpaceLightBufferedArray;


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
		BindTexCoords3f(NULL),
		DrawWire(NULL),
		DrawLines(NULL),  
		DrawNormals(NULL),
		DrawVertexes(NULL),
		DrawAABB(NULL),

		
		windowInformation(NULL),

		CreateOpenGL_Window(NULL),
		RepairOpenGL_Window(NULL),
		DeleteOpenGL_Window(NULL),
		setCaptionOpenGL_Window(NULL),
		FlushOpenGL_Window(NULL),
		currentObject(NULL), currentShadow(NULL) 
		{}

	virtual void processCamera(void);
	virtual bool init(Config* _conf, Input *input);
	virtual bool process(steel::time globalTime, steel::time time);
	virtual bool deinit(void);

	void prepare(GraphShadow *shadow, steel::time globalTime, steel::time time, matrix34 matrix = matrix34::getIdentity(), GameObject *parent = NULL);

	bool isVisible(aabb box);

	void process(GraphShadow *e, steel::time globalTime, steel::time time);
	
//	void drawFaces(DrawElement &e);
//	void drawNormals(DrawElement &e);
//	void drawVertexes(DrawElement &e);
//	void drawAABB(DrawElement &e, matrix34 matrix);

	
	void drawBump(GraphShadow &e, const TexCoords *coords, const matrix34 matrix, const v3 light, uid bufId, int curTexArb, Image *img);
	void getTangentSpace(const Vertexes*, TexCoords const *mapcoord, const FaceMaterials *faceMaterials, Normals const *normal, steel::vector<v3> **sTangent, steel::vector<v3> **tTangent);
	void genTangentSpaceLight(const steel::vector<v3> &sTangent, const steel::vector<v3> &tTangent, const Vertexes &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light,	v3List &tangentSpaceLight);

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

	GraphShadow* getShadow(GameObject *object) { return (GraphShadow*)Engine::getShadow(object); }
	GraphShadow* getShadow(uid id) { return (GraphShadow*)Engine::getShadow(id); }
	 
	Shadow* getShadowClass(GameObject *object) { return new GraphShadow(this); }

	void onResize(int width, int height);

	struct WindowInformation
	{
	};

	WindowInformation *windowInformation;
	// Window management functions
	bool isFocusedOpenGL_Window(void) { return focused; }

	bool (OpenGL_Engine::*CreateOpenGL_Window)(Input *input);
	bool (OpenGL_Engine::*RepairOpenGL_Window)(void);
	bool (OpenGL_Engine::*DeleteOpenGL_Window)(void);
	bool (OpenGL_Engine::*setCaptionOpenGL_Window)(std::string caption);
	bool (OpenGL_Engine::*FlushOpenGL_Window)(void); // Swap buffers

#ifdef LIB_SDL
	void UseSDL(void);

	bool CreateOpenGL_Window_SDL(Input *input);
	bool FlushOpenGL_Window_SDL(void); // Swap buffers
	bool DeleteOpenGL_Window_SDL(void);
	bool setCaptionOpenGL_Window_SDL(std::string caption);
#endif

#if STEEL_OS == OS_WIN32
	void UseWinAPI(void);

	bool CreateOpenGL_Window_WinAPI(Input *input);
	bool FlushOpenGL_Window_WinAPI(void); // Swap buffers
	bool DeleteOpenGL_Window_WinAPI(void);
	bool RepairOpenGL_Window_WinAPI(void);
	bool setCaptionOpenGL_Window_WinAPI(std::string caption);
#endif

};



#endif
