#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../image/image.h"
#include "../conf/conf.h"
//#include "../script/script.h"
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
	Image *image;

	TextureBlendMode	mode;
	TextureFormat		format;
	color4f				color;
	// color
	// mode
	// shader
};

class Material: public Res
{
protected:
	Config *conf;
	bool	blend, shader;
	steel::vector<Texture> texture;
	std::string directory;

public:
	Material(): conf(NULL) {}
	~Material();

	bool init(std::string _conf, const std::string dir);
	Texture *getTexture(int number) { return &texture[number]; }
	int getTextureCount(void) const { return texture.size();}
	bool isShader(void) { return shader; }
	Config *getConfig(void)  { return conf; };
	std::string getDirectory(void) { return directory;}

};

#endif
