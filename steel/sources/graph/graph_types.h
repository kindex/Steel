/*id*********************************************************
	File: graph/graph_types.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Графический джижок - базовые графические типы
 ************************************************************/
#ifndef __GRAPH__TYPES_H
#define __GRAPH__TYPES_H

#include "../steel.h"
#include "../engine/engine_types.h"
#include "../engine/interface.h"
#include "../math/aabb.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/vector2d.h"
#include "../res/image/image.h"


struct color4f
{
	float r,g,b,a;

	void set(float R, float G, float B, float A) {r = R; g = G; b = B; a = A;}
	void set(const v3 rgb) {r = rgb.x; g = rgb.y; b = rgb.z; a = 1.0f;}

	color4f() {}
	color4f(float R, float G, float B, float A): r(R), g(G), b(B), a(A) {}

	const float *getfv() const { return &r; }
};

enum LightType
{
	LIGHT_DIFFUSE = 0, // omni?
	LIGHT_CUBE_MAP = 1, // cube texture
	LIGHT_TARGET = 2 // spot
};

struct Light
{
	uid id;
	LightType type;
	v3 position;
	v3 direction; // orientation? angle, target point?
	v3 up; // needed for cube map

	float intensivity; // intensivity * color
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float minDistance; // max distance without rolloff
	float maxDistance;
	v3 color;
	float k;
	float diffuse_k;
	float specular_k;

	bool castShadows;
	float angle; // LIGHT_TARGET light cone angle
	Image* cubeMap; // cube map
	Image* flare;
	float flareSize;

	Light();
};

//	kind : 2d only
class TexCoords: public pvector<v2>, public BufferedElement{};
class TexCoords3f: public pvector<v3>, public BufferedElement{};

class GLine // al-a triangle
{
public:
	unsigned int a[2]; // vertex indexes
	void set(const int A, const int B) { a[0] = A; a[1] = B; }
};

struct GLines
{
	pvector<GLine> index;
	pvector<color4f> color; // line color
};

typedef Vertexes	Normals;

#endif
