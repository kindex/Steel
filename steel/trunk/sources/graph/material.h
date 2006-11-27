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
#include "graph_types.h"
#include "../res/config/config.h"

//тип смешивания двух текстур
typedef	enum
{
	TEXTURE_BLEND_MODE_NONE,
	TEXTURE_BLEND_MODE_REPLACE,
	TEXTURE_BLEND_MODE_ADD,
	TEXTURE_BLEND_MODE_MUL,
	TEXTURE_BLEND_MODE_BLEND
} TextureBlendMode;

// фрмат текстуры
typedef	enum
{
	TEXTURE_FORMAT_NONE,
	TEXTURE_FORMAT_COLOR_MAP,
	TEXTURE_FORMAT_COLOR,
	TEXTURE_FORMAT_REFLECT,
	TEXTURE_FORMAT_SHADER
} TextureFormat;

// одна текстура. Ипользуется при мультитекстутированию
struct Texture
{
	TextureBlendMode	mode;
	TextureFormat		format;

	TextureBlendMode getMode(void) const { return mode; }
	TextureFormat getTextureFormat(void) const { return format; }
};

struct TextureColorMap: public Texture
{
	Image *color_map;
	Image *normal_map;
};

struct TextureReflect: public Texture
{
	Image *cube_map;
};


struct TextureColor: public Texture
{
	color4f color;
};

struct TextureShader: public Texture
{
	Config	*conf;
};


// материал задаёт множество текстур и типы их наложения
// или шейдеры для рендеринга моделей
class Material
{
protected:
	Config *conf;
	bool	blend;
	svector<Texture*> textures;

public:
	Material(void): conf(NULL) {}
	Material(Config *conf): conf(NULL) { InitFromConfig(conf); }
	Material(const std::string &matFileName);
	~Material(void);

	// загружает материал из конфига
	bool InitFromConfig(Config *config);
	// получить текстуру с номером number
	Texture *getTexture(int number) { return textures[number]; }
	bool isBlending(void) { return blend; }
	// получить количество текстур
	int getTextureCount(void) const { return textures.size();}
};

#endif
