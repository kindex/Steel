/*id*********************************************************
	File: graph/opengl/opengl_engine.h
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
#include "../../libs/opengl/libext.h"		// Header File For The Glaux Library
#endif

#include "../graph_engine.h"
#include "../../input/input.h"
#include "opengl_glsl.h"

class OpenGL_Engine: public GraphEngine
{
// ******************* GRAPH INTERFACE *************************
public:
	bool isSupportingInterface(IN const InterfaceId);
	
	bool setCurrentObject(GameObject*);
	void setPosition(ObjectPosition);
	void setPositionKind(PositionKind);
	void setVertexes(const Vertexes*); // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	void setNormals(const Normals*); // список нормалей в вершинам
	void setLines(const GLines*); // индексы вершин для линий и цвета линий (for debug)
	void setFaceMaterials(const FaceMaterialVector*);// массив индексов вершин, которые образуют треугольники (грани) + материалы
	void setTexCoordsCount(unsigned int);
	void setTexCoords(unsigned int texNumber, const TexCoords*);
	void setAABB(const AABB &box);
	const ProcessInfo& getProcessInfo();

	void addLight(Light*);
	void removeLight(uid);
	void updateLight(uid, Light*);


// ******************* CHILDREN INTERFACE *************************
	void addChild(GameObject* child);
	void deleteChild(GameObject* child);
	void clearChildren();


// ********************* SHADOWS *************************
protected:
	struct OpenGL_Buffer
	{
		enum buffer_kind
		{
			none,
			image,
			array,
			index,
			shader
		};

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
	typedef pvector<LightShadow*> LightShadowPVector;
	struct GraphShadow: public Shadow // множество треугольников одного материала
	{
		PositionKind	positionKind;
		ObjectPosition	position; // global or screen
		ObjectPosition	realPosition; // global, calculated from position anbd parent
		bool 			realPositionCalculated;

		// *** Polyhedra ****
		const FaceMaterialVector *faceMaterials;
		const Vertexes		*vertexes;
		const Normals		*normals;
		unsigned int		 textureCount;
		pvector<const TexCoords*> texCoords;

		const GLines		*lines;

		AABB		aabb;
		bool		aabbCalculated;
		bool		visible;
		float		distance; // расстояние до камеры

		LightShadowPVector lights; // lights to this onject
		

		GraphShadow(Engine *engine);
		void calculateAABB();
		bool isCrossingLight(const LightShadow*);
		void fill(GameObject *object);
		bool cache();

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

		LightShadow();
	};

	map<uid, LightShadow*> lights;
	std::map<uid, OpenGL_Buffer> buffer;

	struct BlendingFaces
	{
		BlendingFaces() {}
		BlendingFaces(GraphShadow* shadow, Material* material, Triangles* triangles) :
			shadow(shadow),
			material(material),
			triangles(triangles)
		{}

		GraphShadow*	shadow;
		Material*		material;
		Triangles*		triangles;
	};

	struct BlendingTriangle
	{
		unsigned int	vertex[3];
		Material*		material;
		float			distance;
		GraphShadow*	shadow;

		const bool operator < (const BlendingTriangle& second) {return distance > second.distance; }
	};
	typedef pvector<BlendingTriangle> BlendingTriangleVector;

	GLuint normalisationCubeMap, lightCubeMap, distMap; // TODO: remove
	Image* zeroNormal;
	Image* black;
	Image* white;
	Image* none;
	Shader shaderStd;
	int maxLightsInShader;

private:
	GraphShadow *currentShadow;
	void addChild(GraphShadow &, GameObject*);


protected:
// ******************* SERVICES *******************
	bool (OpenGL_Engine::*BindTexture)(Image& image, bool enable);
	bool (OpenGL_Engine::*DrawFill_MaterialStd)(OpenGL_Engine::GraphShadow&, const Triangles&, MaterialStd&);
	void (OpenGL_Engine::*DrawTriangles)(OpenGL_Engine::GraphShadow&, const Triangles&, const TexCoords*);
	void (OpenGL_Engine::*CleanupDrawTriangles)();
	void (OpenGL_Engine::*BindTexCoords)(const TexCoords*, const TextureMatrix*);
	void (OpenGL_Engine::*BindTexCoords3f)(const TexCoords3f*);

	void (OpenGL_Engine::*DrawWire)(OpenGL_Engine::GraphShadow&, const Triangles&);
	void (OpenGL_Engine::*DrawLines)(OpenGL_Engine::GraphShadow&);
	void (OpenGL_Engine::*DrawNormals)(OpenGL_Engine::GraphShadow&);
	void (OpenGL_Engine::*DrawVertexes)(OpenGL_Engine::GraphShadow&);
	void (OpenGL_Engine::*DrawAABB)(OpenGL_Engine::GraphShadow&);

// ******************* OpenGL 1.0 *******************
	bool BindTexture_OpenGL10(Image&, bool enable);
	bool DrawFill_MaterialStd_OpenGL10(OpenGL_Engine::GraphShadow&, const Triangles&, MaterialStd&);
	void DrawTriangles_OpenGL10(OpenGL_Engine::GraphShadow&, const Triangles&, const TexCoords*);
	void DrawWire_OpenGL10(OpenGL_Engine::GraphShadow&, const Triangles&);
	void DrawLines_OpenGL10(OpenGL_Engine::GraphShadow&);
	void DrawNormals_OpenGL10(OpenGL_Engine::GraphShadow&);
	void DrawVertexes_OpenGL10(OpenGL_Engine::GraphShadow&);
	void DrawAABB_OpenGL10(OpenGL_Engine::GraphShadow&);

// ******************* OpenGL 1.1 *******************
	bool BindTexture_OpenGL11(Image& image, bool enable);
	void DrawTriangles_OpenGL11(OpenGL_Engine::GraphShadow&, const Triangles&, const TexCoords*);
	void DrawWire_OpenGL11(OpenGL_Engine::GraphShadow&, const Triangles&);
	void DrawLines_OpenGL11(OpenGL_Engine::GraphShadow&);
	void BindTexCoords_OpenGL11(const TexCoords*, const TextureMatrix*);
	void BindTexCoords3f_OpenGL11(const TexCoords3f*);

// ******************* OpenGL 1.3 *******************
	bool DrawFill_MaterialStd_OpenGL13(OpenGL_Engine::GraphShadow&, const Triangles&, MaterialStd&);

	void drawBump(GraphShadow &e, const TexCoords *coords, const matrix34 matrix, const v3 light, uid bufId, int curTexArb, Image *img);
	void getTangentSpace(const Vertexes*, TexCoords const *mapcoord, const FaceMaterialVector *faceMaterials, Normals const *normal, TexCoords3f **sTangent, TexCoords3f **tTangent);
	void genTangentSpaceLight(const TexCoords3f &sTangent, const TexCoords3f &tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light, v3Vector &tangentSpaceLight);

	typedef TexCoords3f tangentSpaceLightBufferedArray;

	struct TangentSpaceCache
	{
		TexCoords3f t, b;
	};

	typedef 
		std::map<int, TangentSpaceCache>
		tangentCache;

	 tangentCache tangentSpaceCache;

// ******************* OpenGL 1.5 *******************
	void DrawTriangles_OpenGL15(OpenGL_Engine::GraphShadow&, const Triangles&, const TexCoords*);
	void CleanupDrawTriangles_OpenGL15();
	void BindTexCoords_OpenGL15(const TexCoords*, const TextureMatrix*);
	void BindTexCoords3f_OpenGL15(const TexCoords3f*);
	int textureMatrixLevel;


	template<class Class> 
	bool BindVBO(Class *v, int mode, int mode2, int elCnt);

	void cleanBuffer(uid bufId);
// ******************* OpenGL 2.0 *******************
	bool DrawFill_MaterialStd_OpenGL20(OpenGL_Engine::GraphShadow&, const Triangles&, MaterialStd&);
	GLSL *BindShader(Shader*);
	void bindTextureToShader(GLSL* program, const char* name, int imageNum, Image* image);
	void unbindTexCoords();

// ******************* OpenGL all *******************
	void DrawFill_Material(OpenGL_Engine::GraphShadow &e, const Triangles* triangles, Material* material);

	bool		focused;
	ProcessInfo info;

public:
// ****************** GRAPH ENGINE ***********************
	OpenGL_Engine();

	void processCamera();
	bool init(Config* _conf, Input *input);
	bool process(IN const ProcessInfo&);
	bool deinit();
	void updateRealPosition(IN OUT GraphShadow* object);

	void prepare(GraphShadow&, matrix34 matrix = matrix34::getIdentity(), GameObject *parent = NULL);
	void process(GraphShadow&, OUT FaceMaterialVector& skippedFaces);
	bool isVisible(AABB box);
	void pushPosition(GraphShadow&);
	void popPosition(GraphShadow&);
	
	GraphShadow* getShadow(GameObject* object) { return (GraphShadow*)Engine::getShadow(object); }
	GraphShadow* getShadow(uid id) { return (GraphShadow*)Engine::getShadow(id); }
	 
	Shadow* getShadowClass(GameObject *object) { return new GraphShadow(this); }


// ****************** WINDOW FUNCTION **********************
	struct WindowInformation {};
	WindowInformation *windowInformation;

	bool isFocusedOpenGL_Window() { return focused; }
	void onResize(int width, int height);

	bool (OpenGL_Engine::*CreateOpenGL_Window)(Input *input);
	bool (OpenGL_Engine::*RepairOpenGL_Window)();
	bool (OpenGL_Engine::*DeleteOpenGL_Window)();
	bool (OpenGL_Engine::*setCaptionOpenGL_Window)(std::string caption);
	bool (OpenGL_Engine::*FlushOpenGL_Window)(); // Swap buffers

#ifdef LIB_SDL
	void UseSDL();

	bool CreateOpenGL_Window_SDL(Input *input);
	bool FlushOpenGL_Window_SDL(); // Swap buffers
	bool DeleteOpenGL_Window_SDL();
	bool setCaptionOpenGL_Window_SDL(std::string caption);
#endif

#if STEEL_OS == OS_WIN32
	void UseWinAPI();

	bool CreateOpenGL_Window_WinAPI(Input *input);
	bool FlushOpenGL_Window_WinAPI(); // Swap buffers
	bool DeleteOpenGL_Window_WinAPI();
	bool RepairOpenGL_Window_WinAPI();
	bool setCaptionOpenGL_Window_WinAPI(std::string caption);
#endif

};

#endif
