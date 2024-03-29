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
#pragma once

#include "../steel.h"
#include "../engine/engine_types.h"
#include "../engine/interface.h"
#include "../math/aabb.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/vector2d.h"
#include "../res/image/image.h"
#include "../res/font/font.h"
#include "../common/svector.h"
#include "../math/sprite.h"

struct color4f
{
	float r;
	float g;
	float b;
	float a;

	void set(float R, float G, float B, float A) {r = R; g = G; b = B; a = A;}
	void set(const v3 rgb) {r = rgb.x; g = rgb.y; b = rgb.z; a = 1.0f;}

	color4f() {}
	color4f(float R, float G, float B, float A): r(R), g(G), b(B), a(A) {}
	color4f(float R, float G, float B): r(R), g(G), b(B), a(1.0f) {}

	const float *getfv() const { return &r; }
	const v3 get() const { return v3(r, g, b); }
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

typedef VertexVector	Normals;

class MaterialStd;

struct FaceMaterial
{
	FaceMaterial() {}
	FaceMaterial(MaterialStd* material, Faces* faces) : material(material), faces(faces) {}

	MaterialStd*	material;
	Faces*			faces;
	std::string		name;
};

typedef svector<FaceMaterial> FaceMaterialVector;

struct GraphText
{
    enum TextAlign
    {
        ALIGN_CENTER,
        ALIGN_LEFT_TOP,
    };

    GraphText(const std::string&    string,
              const ObjectPosition& position,
              const PositionKind    position_kind,
              const v2              size,
              const SpriteAlign     align,
              const TextAlign       text_align,
                    Font*           font,
                    color4f         color) :
        string(string),
        position(position),
        position_kind(position_kind),
        align(align),
        size(size),
        text_align(text_align),
        font(font),
        color(color)
    {}

    GraphText() : 
        position_kind(POSITION_LOCAL),
        position(matrix34::getIdentity()),
        size(1, 1),
        text_align(ALIGN_CENTER),
        font(NULL),
        color(1, 1, 1, 1)
    {}

    std::string    string;
    ObjectPosition position;
    PositionKind   position_kind;
    SpriteAlign    align;
    v2             size;
    TextAlign      text_align;
    Font*          font;
    color4f        color;
};

typedef svector<GraphText> GraphTextVector;
