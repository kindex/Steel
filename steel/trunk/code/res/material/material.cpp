#include "material.h"
#include "../res_main.h"
#include "../../common/utils.h"
using namespace std;

bool Material::init(string name, const std::string dir)
{
	conf = resScript.add(name + ".mat", false);
	if(conf == NULL)
	{
		log_msg("error res material", string("Material not found: ") + name);
		resScript.pop();
		return false;
	} 
	int lines = conf->count();
	if(lines == 0) 
	{
		resScript.pop();
		return false;
	}

	texture.clear();

	for(int i=0; i<lines; i++)
	{
		string sMode = conf->gets(i, 0);
		Texture m;
		m.mode = TEXTURE_BLEND_MODE_NONE;
		if(sMode == "")	m.mode = TEXTURE_BLEND_MODE_REPLACE;
		if(sMode == "+")m.mode = TEXTURE_BLEND_MODE_ADD;
		if(sMode == "*")m.mode = TEXTURE_BLEND_MODE_MUL;
		if(sMode == "blend")m.mode = TEXTURE_BLEND_MODE_BLEND;

		string kind = conf->gets(i, 1);
		string file = conf->gets(i, 2);

		if(kind == "color_map")
		{
			m.image = resImage.add( file);
			if(!m.image)
				m.image = resImage.add( dir + "/" + file);

			if(m.image)
			{
				m.format = TEXTURE_FORMAT_COLOR_MAP;
				texture.push_back(m);
			}
		}
		if(kind == "bump")
		{
			m.image = resImage.add( file);
			if(!m.image)
				m.image = resImage.add( dir + "/" + file);

			if(m.image)
			{
				m.format = TEXTURE_FORMAT_BUMP_MAP;
				texture.push_back(m);
			}
		}
		if(kind == "env")
		{
			m.image = resImage.add( file);
			if(!m.image)
				m.image = resImage.add( dir + "/" + file);

			if(m.image)
			{
				m.format = TEXTURE_FORMAT_ENV;
				texture.push_back(m);
			}
		}
		if(kind == "color")
		{
			m.image = NULL;
			m.format = TEXTURE_FORMAT_COLOR;
			m.color.set(conf->getf(i, 2, 0.0f), conf->getf(i, 3, 0.0f), conf->getf(i, 4, 0.0f), conf->getf(i, 5, 1.0f));
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

	resScript.remove(conf);
}
