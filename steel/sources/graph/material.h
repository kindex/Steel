/*id*******************************************************************************
	File: graph/material.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Формат хранения материала
**************************************************************************************/

#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../steel.h"
#include "../res/image/image.h"
#include "../res/text/text.h"
#include "graph_types.h"
#include "../res/config/config.h"

enum MaterialType
{
	MATERIAL_NONE,
	MATERIAL_STD,
	MATERIAL_SHADER,
};

enum BlendType
{
	BLEND_NONE = 0,
	BLEND_ADDITIONAL_TEXTURE = 1,
	BLEND_WHITE = 2,
	BLEND_BLACK = 3
};

// материал задаёт множество текстур и типы их наложения
// или шейдеры для рендеринга моделей
class Material
{
public:
	Material(): conf(NULL), reserve(NULL)  { id = id = objectIdGenerator.genUid(); }
	Material(MaterialType _type): conf(NULL), reserve(NULL), type(_type) {}
	virtual ~Material();

	virtual uid	 getId()				{ return id; }
	// загружает материал из конфига
	virtual bool InitFromConfig(Config *config) abstract;
	// получить текстуру с номером number
	MaterialType getMaterialType() const { return type; }

public: //protected:
	uid				id;
	Config*			conf;
	BlendType		blend;
	MaterialType	type;
	Material*		reserve;
};

typedef enum
{
	TEXTURE_MODE_NONE,
	TEXTURE_MODE_ADD,
	TEXTURE_MODE_MUL,
	TEXTURE_MODE_BLEND
} TextureMode;

typedef enum
{
	TEXTURE_REFLECT_NONE,
	TEXTURE_REFLECT_MIRROR,
	TEXTURE_REFLECT_SKY
} TextureReflectType;

struct TextureMatrix
{
	v3 texCoordsScale;
	v3 texCoordsTranslation;
	float texCoordsRotation;
};


class MaterialStd: public Material
{
public:
	MaterialStd();
	bool InitFromConfig(Config *config);

	struct TextureStd
	{
		Image *image;
		unsigned int texCoordsUnit;
		float k;

		TextureStd(): image(NULL), texCoordsUnit(0), k(0.0) {}
		bool InitFromConfig(Config *config);
	};

	struct TextureReflect
	{
		Image *image;
		TextureReflectType type;

		TextureReflect(): image(NULL) {}
		bool InitFromConfig(Config *config);
	};

//private:
	float specularPower;

	TextureMatrix textureMatrix;

	TextureStd diffuse_map, diffuse2_map, normal_map, specular_map, emission_map;
	color4f color;
};

struct Shader
{
	uid		id;
	Text	*vertexShader;
	Text	*fragmentShader;

	Shader(): vertexShader(NULL), fragmentShader(NULL) { id = id = objectIdGenerator.genUid(); }

	virtual uid	 getId()				{ return id; }
};

class MaterialShader: public Material
{
	Shader shader;

	bool InitFromConfig(Config *config);
};

//тип смешивания двух текстур
typedef	enum
{
	TEXTURE_BLEND_MODE_NONE,
	TEXTURE_BLEND_MODE_REPLACE,
	TEXTURE_BLEND_MODE_ADD,
	TEXTURE_BLEND_MODE_MUL,
	TEXTURE_BLEND_MODE_BLEND
} TextureBlendMode;

class MaterialCustom: public Material
{

};

Material *getMaterialClass(std::string _class);
Material *createMaterial(Config*);
Material *createMaterial(std::string);

#endif
