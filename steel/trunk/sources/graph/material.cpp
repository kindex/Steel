/*id*******************************************************************************
	File: graph/material.cpp
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

#include "../steel.h"
#include "material.h"
#include "../res/res_main.h"
#include "../common/utils.h"
using namespace std;

Material::Material(const std::string &matFileName)
{
	InitFromConfig(resConfig.add(matFileName + ".mat"));
}


bool Material::InitFromConfig(Config *_conf)
{
	if(_conf == NULL) return false;
	conf = _conf;

	blend = conf->geti("blend") > 0;

	textures.clear();
	ConfigArray *textureConfig = conf->getArray("textures");

	if (textureConfig != NULL)
	for EACH(ConfigArray, *textureConfig, it)
	{
		Config *texconf = *it;
		string format = texconf->gets("format");

		if(format.empty()) break;

		TextureBlendMode mode;
		string sMode = texconf->gets("mode");
		mode = TEXTURE_BLEND_MODE_NONE;
		if(sMode == "")	mode = TEXTURE_BLEND_MODE_REPLACE;
		if(sMode == "+")mode = TEXTURE_BLEND_MODE_ADD;
		if(sMode == "*")mode = TEXTURE_BLEND_MODE_MUL;
		if(sMode == "blend")mode = TEXTURE_BLEND_MODE_BLEND;

		if(format == "color_map")
		{
			TextureColorMap *texture = new TextureColorMap;
			texture->mode = mode;
			texture->format = TEXTURE_FORMAT_COLOR_MAP;

			texture->color_map = resImage.add("/" + texconf->getPath("color_map"));
			texture->normal_map = resImage.add("/" + texconf->getPath("normal_map"));

			if(texture->color_map != NULL)
			{
				textures.push_back(texture);
				continue;
			}
		}
/*		if(format == "sky")
		{
			m.image = resImage.add(conf->gets("image" + IntToStr(i)));

			if(m.image)
			{
				m.format = TEXTURE_FORMAT_ENV;
				texture.push_back(m);
			}
		}
*/
		if(format == "color")
		{
			TextureColor *texture = new TextureColor;
			texture->mode = mode;
			texture->format = TEXTURE_FORMAT_COLOR_MAP;
			texture->color.set(texconf->getv3("color", v3(1.0f, 0.0f, 0.0f)));
			textures.push_back(texture);
			continue;
		}
	}
	return !textures.empty(); 
}


Material::~Material()
{
	 // TODO: cleanup
/*	for(svector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		if(it->image) resImage.remove(it->image);
	}
	if(conf) resConfig.remove(conf);*/
}
