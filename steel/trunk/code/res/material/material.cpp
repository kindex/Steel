/*id*******************************************************************************
	File: res/material/material.cpp
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

#include "../../steel.h"
#include "material.h"
#include "../res_main.h"
#include "../../common/utils.h"
using namespace std;

bool Material::init(string name, const std::string dir)
{
	conf = resOldConfig.add(name + ".mat", false);
	if(conf == NULL)
	{
		log_msg("error res material", string("Material not found: ") + name);
		resScript.pop();
		return false;
	} 
	directory = dir;

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
			m.image = resImage.add(conf->gets("image" + IntToStr(i)));

			if(m.image)
			{
				m.format = TEXTURE_FORMAT_COLOR_MAP;
				texture.push_back(m);
			}
		}
		if(format == "bump")
		{
			m.image = resImage.add(conf->gets("image" + IntToStr(i)));
			if(m.image)
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
	if(texture.empty()) 
	{
		resScript.pop();
		return false;
	}

	blend = texture[0].mode != TEXTURE_BLEND_MODE_REPLACE;
	resScript.pop();
	return true;
}

Material::~Material()
{
	for(steel::vector<Texture>::iterator it = texture.begin(); it != texture.end(); it++)
	{
		if(it->image) resImage.remove(it->image);
	}
	if(conf) resOldConfig.remove(conf);
}
