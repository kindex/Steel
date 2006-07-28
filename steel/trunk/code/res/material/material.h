#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../image/image.h"
#include "../conf/conf.h"
#include "../script/script.h"
#include "../../graph/types.h"

//namespace MapMode
typedef	enum
{
	TEXTURE_BLEND_MODE_NONE,
	TEXTURE_BLEND_MODE_REPLACE,
	TEXTURE_BLEND_MODE_ADD,
	TEXTURE_BLEND_MODE_MUL,
	TEXTURE_BLEND_MODE_BLEND
} TextureBlendMode;

typedef	enum
{
	TEXTURE_FORMAT_NONE,
	TEXTURE_FORMAT_COLOR_MAP,
	TEXTURE_FORMAT_COLOR,
	TEXTURE_FORMAT_ENV,
	TEXTURE_FORMAT_NORMAL_MAP,
	TEXTURE_FORMAT_BUMP_MAP
} TextureFormat;

class Texture
{
public:
	Image *texture;

	TextureBlendMode	mode;
	TextureFormat	kind;
	color4f				color;
	// color
	// mode
	// shader
};

class Material: public Res
{
public:
	Script* conf;
	bool	blend;
	steel::vector<Texture> map;

	bool init(std::string _conf, const std::string dir);

	~Material();
};

#endif
