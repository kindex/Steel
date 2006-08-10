/*id*******************************************************************************
    Unit: Res Material
    Part of: Steel engine
    (C) DiVision, 2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        ћолуль дл€ загрузки и хранени€ материала
**************************************************************************************/

#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../image/image.h"
#include "../conf/conf.h"
#include "../../graph/types.h"

//тип смешивани€ двух текстур
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

// одна текстура. »пользуетс€ при мультитекстутированию
class Texture
{
public:
	Image *image;

	TextureBlendMode	mode;
	TextureFormat		format;
	color4f				color;
};

// материал задаЄт множество текстур и типы их наложени€
// или шейдеры дл€ рендеринга моделей
class Material: public Res
{
protected:
	Config *conf;
	bool	blend, shader;
	steel::vector<Texture> texture;
	std::string directory;

public:
	Material(void): conf(NULL) {}
	~Material(void);

	// загружает материал из *.mat файла
	bool init(std::string _conf, const std::string dir);
	// получить текстуру с номером number
	Texture *getTexture(int number) { return &texture[number]; }
	// получить количество текстур
	int getTextureCount(void) const { return texture.size();}
	// указана ли в конфиге материала шейдер дл€ рендеринга
	bool isShader(void) { return shader; }
	// получить час конфиг
	const Config *getConfig(void) const { return conf; };
	// директори€, откуда был загружен материал. 
	// Ќужна дл€ относительного отсчЄта файлов, указанных в конфиге
	std::string getDirectory(void) { return directory;}

};

#endif
