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

	shader = !conf->gets("vertexShader").empty() && !conf->gets("fragmentShader").empty();

	texture.clear();

	for(int i=0; ; i++)
	{
		Texture m;
		string format = conf->gets("format" + IntToStr(i));

		if(format.empty()) break;

		string sMode = conf->gets("mode" + IntToStr(i));
		m.mode = TEXTURE_BLEND_MODE_NONE;
		if(sMode == "")	m.mode = TEXTURE_BLEND_MODE_REPLACE;
		if(sMode == "+")m.mode = TEXTURE_BLEND_MODE_ADD;
		if(sMode == "*")m.mode = TEXTURE_BLEND_MODE_MUL;
		if(sMode == "blend")m.mode = TEXTURE_BLEND_MODE_BLEND;

		if(format == "color_map")
		{
			string imageName = "/" + conf->getPath("image" + IntToStr(i));

			m.image = resImage.add(imageName);

			if(m.image != NULL)
			{
				m.format = TEXTURE_FORMAT_COLOR_MAP;
				texture.push_back(m);
			}
		}
		if(format == "bump")
		{
			m.image = resImage.add(conf->gets("image" + IntToStr(i)));
			if(m.image != NULL)
			{
				m.format = TEXTURE_FORMAT_BUMP_MAP;
				texture.push_back(m);
			}
		}
		if(format == "env")
		{
			m.image = resImage.add(conf->gets("image" + IntToStr(i)));
//			if(!m.image) m.image = resImage.add( dir + "/" + file);

			if(m.image)
			{
				m.format = TEXTURE_FORMAT_ENV;
				texture.push_back(m);
			}
		}

		if(format == "color")
		{
			m.image = NULL;
			m.format = TEXTURE_FORMAT_COLOR;
//			m.color = onf->getf(i, 2, 0.0f), conf->getf(i, 3, 0.0f), conf->getf(i, 4, 0.0f), conf->getf(i, 5, 1.0f));
			texture.push_back(m);
		}
	}
	if(!texture.empty())
	{
		blend = texture[0].mode != TEXTURE_BLEND_MODE_REPLACE;
	}
	return !texture.empty(); 
}


Material::~Material()
{
	for(steel::vector<Texture>::iterator it = texture.begin(); it != texture.end(); it++)
	{
		if(it->image) resImage.remove(it->image);
	}
	if(conf) resConfig.remove(conf);
}
