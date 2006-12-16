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
#include "gl/libext.h"		// Header File For The Glaux Library
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

	void addChild(GameObject* child);
	void deleteChild(GameObject* child);
	void clearChildren(void);

	void addLight(Light*);
	void removeLight(uid);
	void updateLight(uid, Light*);


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

	struct LightShadow;
	struct GraphShadow: public Shadow // множество треугольников одного материала
	{
		ObjectPosition	position; // global or screen
		PositionKind	positionKind;

		// *** Polyhedra ****
		const FaceMaterials *faceMaterials;
		const Vertexes		*vertexes;
		const Normals		*normals;
		unsigned int		 textureCount;
		svector<const TexCoords*> texCoords;

		const GLines		*lines;

		aabb		frame;
		bool		blend; // true if blending
		bool		visible;
		float		distance; // расстояние до камеры

		svector<LightShadow*> lights; // lights to this onject

		GraphShadow(Engine *engine): Shadow(engine), faceMaterials(NULL), vertexes(NULL), normals(NULL), 
			lines(NULL), position(ObjectPosition::getIdentity()), textureCount(0) {}
		void fill(GameObject *object);
		bool cache(void);

		const TexCoords* getTexCoords(const MaterialStd::TextureStd &texture);
//		bool	operator < (const DrawElement &sec) const { return distance > sec.distance; }
	};

	struct LightShadow
	{
		v3 position;
		Light *light;
		GameObject *object;
		GraphShadow *shadow;
		bool changed; // position

		LightShadow(void): light(NULL), object(NULL) {}
	};

	map<uid, LightShadow*> lights;

	void addChild(GraphShadow &, GameObject*);

	std::map<uid, OpenGL_Buffer> buffer;

	GLuint normalisationCubeMap, lightCubeMap, distMap; // TODO: remove
	Image *zeroNormal;
	Shader shaderStd;


protected:
	// procedure variables
	bool (OpenGL_Engine::*BindTexture)(Image *image, bool enable);
	bool (OpenGL_Engine::*DrawFill_MaterialStd)(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, MaterialStd *material, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawTriangles)(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*CleanupDrawTriangles)(void);
	void (OpenGL_Engine::*BindTexCoords)(const TexCoords *coords);
	void (OpenGL_Engine::*BindTexCoords3f)(const TexCoords3f *coords);

	void (OpenGL_Engine::*DrawWire)(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawLines)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawNormals)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawVertexes)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);
	void (OpenGL_Engine::*DrawAABB)(OpenGL_Engine::GraphShadow &e, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.0
	bool BindTexture_OpenGL10(Image *image, bool enable);
	bool DrawFill_MaterialStd_OpenGL10(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, MaterialStd *material, GraphEngine::GraphTotalInfo &total);
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
	bool DrawFill_MaterialStd_OpenGL13(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, MaterialStd *material, GraphEngine::GraphTotalInfo &total);

	// OpenGL 1.5
	void DrawTriangles_OpenGL15(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, const TexCoords *coords, GraphEngine::GraphTotalInfo &total);
	void CleanupDrawTriangles_OpenGL15(void);
	void BindTexCoords_OpenGL15(const TexCoords *coords);
	void BindTexCoords3f_OpenGL15(const TexCoords3f *coords);

	// OpenGL 2.0
	bool DrawFill_MaterialStd_OpenGL20(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, MaterialStd *material, GraphEngine::GraphTotalInfo &total);
	GLSL *BindShader(Shader *);
	void bindTextureToShader(GLSL *program, const char *name, int imageNum, Image *image);

	// Uni
	void DrawFill_Material(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);


	// Stuff to delete

	typedef
		svector<v3>
		v3List;

	typedef TexCoords3f tangentSpaceLightBufferedArray;

	struct Tangents: public BufferedElement
	{
		svector<v3> data;
		Tangents(): BufferedElement(), data(0) {}
	};

	struct TangentSpaceCache
	{
		Tangents t, b;
	};

	typedef 
		std::map<int, TangentSpaceCache>
		tangentCache;

	 tangentCache tangentSpaceCache;

	 bool focused;
	 steel::time time;

public:
	OpenGL_Engine(void);

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
	void getTangentSpace(const Vertexes*, TexCoords const *mapcoord, const FaceMaterials *faceMaterials, Normals const *normal, Tangents **sTangent, Tangents **tTangent);
	void genTangentSpaceLight(const Tangents &sTangent, const Tangents &tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light,	v3List &tangentSpaceLight);

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

template<class Class> bool OpenGL_Engine::BindVBO(Class *v, int mode, int mode2, int elCnt)
{
	if(v == NULL || v->data.empty()) return false;

	if(GL_EXTENSION_VBO) // Vertex Buffer Object supportd
	{
		uid	id = v->getId();
		if(id == 0) 
		{
			if(mode) glEnableClientState(mode);
			glBindBufferARB(mode2, 0);
			return false;
		}

		bool loaded = false;
		if(buffer.find(id) != buffer.end())
			loaded = buffer[id].loaded;

		OpenGL_Buffer &buf = buffer[id];

		if(loaded && buf.size != v->data.size())
		{
			glDeleteBuffersARB(1, &buf.glid);
			buf.loaded = false;
			loaded = false;
		}

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
//			buf.temp = false;
		}
		else
		{
			glGenBuffersARB(1, &buf.glid);

			if(mode > 0) glEnableClientState ( mode );
			glBindBufferARB(mode2, buf.glid);

			GLenum usage = GL_STATIC_DRAW;
			if(v->changed)
				usage = GL_STREAM_DRAW;

			glBufferDataARB(mode2, elCnt*sizeof(float)*v->data.size(), &v->data.front(), usage);
			buf.size = v->data.size();
	
			buf.loaded = true;
			buf.loadCnt++;
			if(mode2 == GL_ARRAY_BUFFER_ARB)
				buf.kind = OpenGL_Buffer::array;
			if(mode2 == GL_ELEMENT_ARRAY_BUFFER_ARB)
				buf.kind = OpenGL_Buffer::index;

//			buf.temp = false;
			buf.lastUsedTime = time;
			buf.usedCnt++;
		}
		return true;
	}  
	else
		return false;
}


#endif
