/*id*******************************************************************************
	File: graph/material.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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

typedef enum
{
	MATERIAL_NONE,
	MATERIAL_STD,
	MATERIAL_SHADER,
} MaterialType;


// материал задаёт множество текстур и типы их наложения
// или шейдеры для рендеринга моделей
class Material
{
public:
//protected:
	uid		id;
	Config *conf;
	bool	blend;
	MaterialType type;
	Material *backup;

public:
	Material(void): conf(NULL), backup(NULL)  { id = id = objectIdGenerator.genUid(); }
	Material(MaterialType _type): conf(NULL), backup(NULL), type(_type) {}
	virtual ~Material(void);

	virtual uid	 getId(void)				{ return id; }
	// загружает материал из конфига
	virtual bool InitFromConfig(Config *config) = 0;
	// получить текстуру с номером number
	bool isBlending(void) const { return blend; }
	MaterialType getMaterialType(void) const { return type; }
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

class MaterialStd: public Material
{
public:

	struct TextureStd
	{
		Image *image;
		TextureMode mode;
		unsigned int texCoordsUnit;
		v3 texCoordsScale;
		float texCoordsRotation;
		v3 texCoordsTranslation;

		TextureStd(void): image(NULL), texCoordsUnit(0) {}
		bool InitFromConfig(Config *config);
	};

	struct TextureReflect
	{
		Image *image;
		TextureReflectType type;

		TextureReflect(void): image(NULL) {}
		bool InitFromConfig(Config *config);
	};

	bool InitFromConfig(Config *config);
	MaterialStd(void);

//private:
	float specularPower;
	float speculark;
	float diffusek;
	float emissionk;

	TextureStd diffuse_map, diffuse2_map, normal_map, specular_map, emission_map;
	TextureReflect reflect_map;
	color4f color;
};

struct Shader
{
	uid		id;
	Text	*vertexShader;
	Text	*fragmentShader;

	Shader(void): vertexShader(NULL), fragmentShader(NULL) { id = id = objectIdGenerator.genUid(); }

	virtual uid	 getId(void)				{ return id; }
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
