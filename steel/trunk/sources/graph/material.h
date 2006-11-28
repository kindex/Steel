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
	Config *conf;
	bool	blend;
	MaterialType type;
	Material *backup;

public:
	Material(void): conf(NULL), backup(NULL) {}
	Material(MaterialType _type): conf(NULL), backup(NULL), type(_type) {}
	virtual ~Material(void);

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

class MaterialStd: public Material
{
public:

	struct Texture
	{
		Image *image;
		TextureMode mode;
		unsigned int texCoordsUnit;
		v3 texCoordsScale;
		float texCoordsRotation;
		v3 texCoordsTranslation;

		Texture(void): image(NULL), texCoordsUnit(0) {}
		bool InitFromConfig(Config *config);
	};

	bool InitFromConfig(Config *config);
	MaterialStd(void);

//private:
	Texture color_map, color_map2, normal_map;
	color4f color;
};

class MaterialShader: public Material
{
};

class MaterialReflect: public Material
{

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
