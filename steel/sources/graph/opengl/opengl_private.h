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
#include "../../common/containers/string_dict.h"
#include <map>

struct TextureStd;
class MaterialStd;

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

struct ShadowEdge
{
	ShadowEdge(int trg, int a, int b) : faceCount(1) 
	{ 
		faces[0] = trg; 
		vertex[0] = a; 
		vertex[1] = b;
	}

	size_t	faceCount;
	size_t	vertex[2];
	int		faces[2]; // >=0 - triangle, <0 - quad
};

struct EdgeVector : public pvector<ShadowEdge>
{
	EdgeVector() : trianglesId(-1), quadsId(-1) {}

	uid trianglesId;
	uid quadsId;
};

typedef svector<EdgeVector> EdgeVectorVector;

#define GS(shadow) (static_cast<GraphShadow*>(shadow))

struct LightShadow;
typedef pvector<LightShadow*> LightShadowPVector;
struct GraphShadow : public Shadow // множество треугольников одного материала
{
	PositionKind	positionKind;
	ObjectPosition	position; // global or screen
	ObjectPosition	realPosition; // global, calculated from position and parent
	bool 			realPositionCalculated;

	// *** Polyhedra ****
	const FaceMaterialVector*	faceMaterials;
	EdgeVectorVector			edges;
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
	
	GraphShadow(Engine* engine);
	void calculateAABB();
	bool isCrossingLight(const LightShadow*);
	void fill(GameObject* object);
	void calculateEgdes();

	const TexCoords* getTexCoords(const TextureStd& texture);
//		bool	operator < (const DrawElement &sec) const { return distance > sec.distance; }
};

struct BlendingFaces
{
	BlendingFaces() {}
	BlendingFaces(GraphShadow* shadow, MaterialStd* material, Faces* faces):
		shadow(shadow),
		material(material),
		faces(faces)
	{}

	GraphShadow*	shadow;
	MaterialStd*	material;
	Faces*			faces;
};

struct BlendingTriangle
{
	size_t			vetexCount;
	size_t			vertex[4];
	MaterialStd*	material;
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
	struct
	{
		bool transparent;
		bool simpleLighting;
		bool shadowLighting;
		bool shadowDebug;
		uid	onlyLight;
	} current;

	bool shadows;
	bool lighting;
	bool textures;
	bool blending;
	bool bump;
	bool glsl;

	bool drawFace;
	bool drawWire;
	bool drawLines;
	bool drawNormals;
	bool drawVertexes;
	bool drawAABB;

	bool useDebugShader;
	size_t debugShaderMode;

	std::string shaderStd;
	std::string shaderDebug;
	std::string shaderNoTexture;
	size_t maxLightsInShader;
};

struct LightShadow
{
	LightShadow();
 
	v3				position; // global
	Light*			light;
	GameObject*		object;
	GraphShadow*	shadow;
	bool			changed; // position is changed
	bool			needToBeRendered(const Flags& flags) const;
};

}

#endif
