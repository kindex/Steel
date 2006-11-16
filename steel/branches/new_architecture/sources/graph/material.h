/*id*******************************************************************************
	File: res/material/material.h
	Unit: res/material
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
        Молуль для загрузки и хранения материала
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
	TEXTURE_FORMAT_ENV,
	TEXTURE_FORMAT_NORMAL_MAP,
	TEXTURE_FORMAT_BUMP_MAP
} TextureFormat;

// одна текстура. Ипользуется при мультитекстутированию
class Texture
{
public:
	Image *image;

	TextureBlendMode	mode;
	TextureFormat		format;
	color4f				color;
};

// материал задаёт множество текстур и типы их наложения
// или шейдеры для рендеринга моделей
class Material
{
protected:
	Config *conf;
	bool	blend, shader;
	steel::vector<Texture> texture;
	std::string directory;

public:
	Material(void): conf(NULL) {}
	Material(Config *conf): conf(NULL) { InitFromConfig(conf); }
	Material(const std::string &matFileName);
	~Material(void);

	// загружает материал из конфига
	bool InitFromConfig(Config *config);
	// получить текстуру с номером number
	Texture *getTexture(int number) { return &texture[number]; }
	// получить количество текстур
	int getTextureCount(void) const { return texture.size();}
	// указана ли в конфиге материала шейдер для рендеринга
	bool isShader(void) { return shader; }
	// получить час конфиг
	const Config *getConfig(void) const { return conf; };
	// директория, откуда был загружен материал. 
	// Нужна для относительного отсчёта файлов, указанных в конфиге
	std::string getDirectory(void) { return directory;}

};

#endif
