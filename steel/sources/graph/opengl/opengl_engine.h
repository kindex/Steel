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
#include <set>

#if STEEL_OS == OS_LINUX
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#elif STEEL_OS == OS_WIN32
#include <windows.h>		// Header File For Windows
#include "../../libs/opengl/libext.h"
#endif

#include "../graph_engine.h"
#include "opengl_private.h"

class Input;
struct TextureMatrix;
struct Line;

namespace opengl
{

class Shader;
struct ShaderKey
{
	ShaderKey(const std::string& id, StringDict parameters) : id(id), parameters(parameters) {}

	const	std::string	id;
	const	StringDict	parameters;
	bool operator < (const ShaderKey& second) const
	{
		return id < second.id || id == second.id && parameters < second.parameters;
	}
};

typedef std::map<ShaderKey, Shader*> ShaderDict;


class OpenGL_Engine : public GraphEngine
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
private:
	typedef std::map<uid, LightShadow*> LightMap;
	LightMap lights;
	std::map<uid, OpenGL_Buffer> buffer;
	ShaderDict shaders;
	Flags flags;

	GLuint normalisationCubeMap, lightCubeMap, distMap; // TODO: remove
	Image* zeroNormal;
	Image* black;
	Image* white;
	Image* none;

	GraphShadow* currentShadow;

	void addChild(GraphShadow&, GameObject*);

// ******************* SERVICES *******************
	bool (OpenGL_Engine::*BindTexture)(Image& image, bool enable);
	bool (OpenGL_Engine::*DrawFill_MaterialStd)(GraphShadow&, const Faces&, MaterialStd&);
	void (OpenGL_Engine::*DrawTriangles)(GraphShadow&, const Faces&, const TexCoords*);
	void (OpenGL_Engine::*CleanupDrawTriangles)();
	void (OpenGL_Engine::*BindTexCoords)(const TexCoords*, const TextureMatrix*);
	void (OpenGL_Engine::*BindTexCoords3f)(const TexCoords3f*);

	void (OpenGL_Engine::*DrawWire)(GraphShadow&, const Faces&);
	void (OpenGL_Engine::*DrawLines)(GraphShadow&);
	void (OpenGL_Engine::*DrawNormals)(GraphShadow&);
	void (OpenGL_Engine::*DrawVertexes)(GraphShadow&);
	void (OpenGL_Engine::*DrawAABB)(GraphShadow&);

// ******************* OpenGL 1.0 *******************
	bool BindTexture_OpenGL10(Image&, bool enable);
	bool DrawFill_MaterialStd_OpenGL10(GraphShadow&, const Faces&, MaterialStd&);
	void DrawTriangles_OpenGL10(GraphShadow&, const Faces&, const TexCoords*);
	void DrawWire_OpenGL10(GraphShadow&, const Faces&);
	void DrawLines_OpenGL10(GraphShadow&);
	void DrawNormals_OpenGL10(GraphShadow&);
	void DrawVertexes_OpenGL10(GraphShadow&);
	void DrawAABB_OpenGL10(GraphShadow&);

// ******************* OpenGL 1.1 *******************
	bool BindTexture_OpenGL11(Image& image, bool enable);
	void DrawTriangles_OpenGL11(GraphShadow&, const Faces&, const TexCoords*);
	void DrawWire_OpenGL11(GraphShadow&, const Faces&);
	void DrawLines_OpenGL11(GraphShadow&);
	void BindTexCoords_OpenGL11(const TexCoords*, const TextureMatrix*);
	void BindTexCoords3f_OpenGL11(const TexCoords3f*);

// ******************* OpenGL 1.3 *******************
	bool DrawFill_MaterialStd_OpenGL13(GraphShadow&, const Faces&, MaterialStd&);

	void drawBump(GraphShadow& e, const TexCoords* coords, const matrix34 matrix, const v3 light, uid bufId, int curTexArb, Image* img);
	void getTangentSpace(const Vertexes*, const TexCoords* mapcoord, const FaceMaterialVector* faceMaterials, const Normals* normal, TexCoords3f** sTangent, TexCoords3f** tTangent);
	void genTangentSpaceLight(const TexCoords3f& sTangent, const TexCoords3f& tTangent, 	Vertexes const &vertex, Normals	const &normal,	matrix34 const matrix, const v3 light, pvector<v3>& tangentSpaceLight);

	typedef TexCoords3f tangentSpaceLightBufferedArray;

	tangentCache tangentSpaceCache;

// ******************* OpenGL 1.5 *******************
	void DrawTriangles_OpenGL15(GraphShadow&, const Faces&, const TexCoords*);
	void CleanupDrawTriangles_OpenGL15();
	void BindTexCoords_OpenGL15(const TexCoords*, const TextureMatrix*);
	void BindTexCoords3f_OpenGL15(const TexCoords3f*);
	int textureMatrixLevel;

	template<class Class> 
	bool BindVBO(Class*, int mode, int mode2, int elCnt);

	void cleanBuffer(uid bufId);
// ******************* OpenGL 2.0 *******************
	bool DrawFill_MaterialStd_OpenGL20(GraphShadow&, const Faces&, MaterialStd&);
	Shader* bindShader(const std::string&, const StringDict& parameters);
	Shader* loadShader(const std::string&, const StringDict& parameters);

	void unbindTexCoords();
	void SetupStdShader_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material, Shader& shader);
	void StdShaderGetLightCount(GraphShadow& e, const Faces& faces, MaterialStd& material, OUT size_t& lightCount, OUT size_t& targetLightCount);
	void SetupDebugShader_OpenGL20(GraphShadow& e, const Faces& faces, MaterialStd& material, Shader& shader);
	friend class Shader;
	Shader* program;
	StringDict currentShaderVariables;
// ******************* OpenGL all *******************
	void collectInformationFromObjects();
	void render();
	void renderTransparent();
	void renderNoShadows();
	void renderFlares();
	void renderCatchShadows();
	void prepareShadowEdges();
	void renderDebug();
	void castShadow(const LightShadow& light);
	void setupVariables();
	void prepare(GraphShadow&, matrix34 matrix = matrix34::getIdentity(), GameObject *parent = NULL);
	void drawObject(GraphShadow&, OUT FaceMaterialVector& skippedFaces);
	void updateRealPosition(IN OUT GraphShadow* object);
	bool isVisible(AABB box);
	void pushPosition(GraphShadow&);
	void popPosition(GraphShadow&);
	size_t rayTrace(const Line& lineSegment, bool shadowed);

	bool		focused;
	ProcessInfo info;

public:
// ****************** GRAPH ENGINE ***********************
	OpenGL_Engine();

	void processCamera();
	bool init(Config* _conf, Input* input);
	bool process(IN const ProcessInfo&);
	bool deinit();
	
	GraphShadow* getShadow(GameObject* object) { return (GraphShadow*)Engine::getShadow(object); }
	GraphShadow* getShadow(uid id) { return (GraphShadow*)Engine::getShadow(id); }
	 
	Shadow*		getShadowClass(GameObject *object) { return new GraphShadow(this); }


// ****************** WINDOW FUNCTION **********************
	struct WindowInformation {};
	WindowInformation* windowInformation;

	bool isFocusedOpenGL_Window() { return focused; }
	void onResize(int width, int height);
	bool setCaption(const std::string& caption);

	bool (OpenGL_Engine::*CreateOpenGL_Window)(Input* input);
	bool (OpenGL_Engine::*RepairOpenGL_Window)();
	bool (OpenGL_Engine::*DeleteOpenGL_Window)();
	bool (OpenGL_Engine::*setCaptionOpenGL_Window)(const std::string& caption);
	bool (OpenGL_Engine::*FlushOpenGL_Window)(); // Swap buffers

#ifdef LIB_SDL
	void UseSDL();

	bool CreateOpenGL_Window_SDL(Input* input);
	bool FlushOpenGL_Window_SDL(); // Swap buffers
	bool DeleteOpenGL_Window_SDL();
	bool setCaptionOpenGL_Window_SDL(const std::string& caption);
#endif

#if STEEL_OS == OS_WIN32
	void UseWinAPI();

	bool CreateOpenGL_Window_WinAPI(Input* input);
	bool FlushOpenGL_Window_WinAPI(); // Swap buffers
	bool DeleteOpenGL_Window_WinAPI();
	bool RepairOpenGL_Window_WinAPI();
	bool setCaptionOpenGL_Window_WinAPI(const std::string& caption);
#endif

}; // class OpenGL_Engine

} // namespace opengl

#endif
