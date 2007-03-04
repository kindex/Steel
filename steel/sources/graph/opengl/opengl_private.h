/*id*********************************************************
	File: graph/opengl/opengl_private.h
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Приватные данные для OpenGL движка
 ************************************************************/

#ifndef GRAPH_OPENGL_PRIVATE_H
#define GRAPH_OPENGL_PRIVATE_H

#include "../../libs/opengl/libext.h"
#include "../graph_engine.h"
#include "../../common/containers.h"
#include "opengl_glsl.h"
#include <map>

namespace opengl
{

struct OpenGL_Buffer
{
	enum OpenGL_BuferKind
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
	OpenGL_BuferKind kind;
	char*	object;
};

#define GS(shadow) (static_cast<GraphShadow*>(shadow))

struct LightShadow;
typedef pvector<LightShadow*> LightShadowPVector;
struct GraphShadow : public Shadow // множество треугольников одного материала
{
	PositionKind	positionKind;
	ObjectPosition	position; // global or screen
	ObjectPosition	realPosition; // global, calculated from position anbd parent
	bool 			realPositionCalculated;

	// *** Polyhedra ****
	const FaceMaterialVector*	faceMaterials;
	const Vertexes*				vertexes;
	const Normals*				normals;
	unsigned int				textureCount;
	pvector<const TexCoords*>	texCoords;

	const GLines*				lines;

	AABB		aabb;
	bool		aabbCalculated;
	bool		visible;
	float		distance; // расстояние до камеры

	LightShadowPVector lights; // lights to this onject
	
	GraphShadow(Engine *engine);
	void calculateAABB();
	bool isCrossingLight(const LightShadow*);
	void fill(GameObject *object);

	const TexCoords* getTexCoords(const MaterialStd::TextureStd &texture);
//		bool	operator < (const DrawElement &sec) const { return distance > sec.distance; }
};

struct LightShadow
{
	LightShadow();
 
	v3				position; // global
	Light*			light;
	GameObject*		object;
	GraphShadow*	shadow;
	bool			changed; // position is changed
};

struct BlendingFaces
{
	BlendingFaces() {}
	BlendingFaces(GraphShadow* shadow, Material* material, Faces* faces):
		shadow(shadow),
		material(material),
		faces(faces)
	{}

	GraphShadow*	shadow;
	Material*		material;
	Faces*			faces;
};

struct BlendingTriangle
{
	size_t			vetexCount;
	size_t			vertex[4];
	Material*		material;
	float			distance;
	GraphShadow*	shadow;

	const bool operator < (const BlendingTriangle& second) {return distance > second.distance; }
};
typedef pvector<BlendingTriangle> BlendingTriangleVector;


struct TangentSpaceCache
{
	TexCoords3f t, b;
};

typedef
	std::map<int, TangentSpaceCache>
	tangentCache;

struct Flags
{
	bool lighting;
	bool textures;
	bool blend;
	bool transparent;
	bool bump;
	bool glsl;
	bool shadows;

	bool drawFace;
	bool drawWire;
	bool drawLines;
	bool drawNormals;
	bool drawVertexes;
	bool drawAABB;

	bool useDebugShader;
	int	debugShaderMode;

	std::string shaderStd;
	std::string shaderDebug;
	std::string shaderNoTexture;
	int maxLightsInShader;
};


}

#endif
