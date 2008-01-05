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
#include "graph_types.h"

class Image;
class Config;

enum MaterialType
{
	MATERIAL_NONE,
	MATERIAL_STD,
	MATERIAL_SHADER
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

enum TextureMode
{
	TEXTURE_MODE_NONE,
	TEXTURE_MODE_ADD,
	TEXTURE_MODE_MUL,
	TEXTURE_MODE_BLEND
};

enum TextureEnvType
{
	TEXTURE_REFLECT_NONE = 0,
	TEXTURE_REFLECT_CUSTOM = 1,
	TEXTURE_REFLECT_MIRROR = 2,
	TEXTURE_REFLECT_SKY = 3
};

struct TextureMatrix
{
	v3 texCoordsScale;
	v3 texCoordsTranslation;
	float texCoordsRotation;
};

struct TextureStd
{
	Image* image;
	unsigned int texCoordsUnit;
	float k;

	TextureStd(): image(NULL), texCoordsUnit(0), k(0.0) {}
	bool InitFromConfig(Config* config);
    Config* getConfig() const;
};


class MaterialStd: public Material
{
public:
	MaterialStd();
	bool InitFromConfig(Config* config);
    Config* getConfig() const;

	struct TextureEnv
	{
		Image* cubeMap;
		TextureEnvType type;
		float k;

		TextureEnv(): cubeMap(NULL) {}
		bool InitFromConfig(Config* config);
	};

//private:
	float specularPower;
	float ambient_k;

	TextureMatrix textureMatrix;

	TextureStd diffuse_map;
	TextureStd diffuse2_map;
	TextureStd normal_map;
	TextureStd specular_map;
	TextureStd emission_map;
	TextureEnv env_map;

	color4f color;
	bool dropShadows;
	bool catchShadows;
};

//тип смешивания двух текстур
enum TextureBlendMode
{
	TEXTURE_BLEND_MODE_NONE,
	TEXTURE_BLEND_MODE_REPLACE,
	TEXTURE_BLEND_MODE_ADD,
	TEXTURE_BLEND_MODE_MUL,
	TEXTURE_BLEND_MODE_BLEND
};


MaterialStd* getMaterialClass(const std::string& _class);
MaterialStd* createMaterial(Config*);
MaterialStd* createMaterial(const std::string&);

#endif
