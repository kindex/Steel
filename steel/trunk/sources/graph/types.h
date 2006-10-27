/*id*********************************************************
	File: graph/types.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Bond [Andrey Bondarenko]
	License:
		Steel Engine License
	Description:
		Графический джижок - базовые графические типы
 ************************************************************/
#ifndef __GRAPH__TYPES_H
#define __GRAPH__TYPES_H

#include "../steel.h"
#include "../interface.h"
#include "../math/aabb.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/vector2d.h"


struct color4f
{
	float r,g,b,a;

	void set(float R, float G, float B, float A) {r = R; g = G; b = B; a = A;}

	color4f(void) {}
	color4f(float R, float G, float B, float A): r(R), g(G), b(B), a(A) {}
};

struct Light
{
	v3 position;
	color4f color;
	float distance, intensivity;
	// cube map, orientation
};

typedef steel::vector<Light>		Lights;

//	kind : 2d only
class TexCoords: public BufferedElement
{
public:
	steel::vector<v2>	data;
};

class TexCoords3f: public BufferedElement
{
public:
	steel::vector<v3> data;
};


class GLine // al-a triangle
{
public:
	unsigned int a[2]; // vertex indexes
	void set(const int A, const int B) { a[0] = A; a[1] = B; }
};

struct GLines
{
	steel::vector<GLine> index;
	steel::vector<color4f> color; // line color
};

typedef Vertexes	Normals;

struct GraphEngineInfo
{
	steel::time curTime;
	steel::time frameLength;
	ModificationTime modificationTime;
	v3 cameraEye;
	v3 cameraDirection;
};



#endif
