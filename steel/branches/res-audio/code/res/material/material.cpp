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

	map.clear();

	for(int i=0; i<lines; i++)
	{
		string sMode = conf->gets(i, 0);
		Map m;
		m.mode = MapMode::none;
		if(sMode == "")	m.mode = MapMode::replace;
		if(sMode == "+")m.mode = MapMode::add;
		if(sMode == "*")m.mode = MapMode::mul;
		if(sMode == "blend")m.mode = MapMode::blend;

		string kind = conf->gets(i, 1);
		string file = conf->gets(i, 2);

		if(kind == "color_map")
		{
			m.texture = resImage.add( file);
			if(!m.texture)
				m.texture = resImage.add( dir + "/" + file);

			if(m.texture)
			{
				m.kind = MapKind::color_map;
				map.push_back(m);
			}
		}
		if(kind == "bump")
		{
			m.texture = resImage.add( file);
			if(!m.texture)
				m.texture = resImage.add( dir + "/" + file);

			if(m.texture)
			{
				m.kind = MapKind::bump_map;
				map.push_back(m);
			}
		}
		if(kind == "env")
		{
			m.texture = resImage.add( file);
			if(!m.texture)
				m.texture = resImage.add( dir + "/" + file);

			if(m.texture)
			{
				m.kind = MapKind::env;
				map.push_back(m);
			}
		}
		if(kind == "color")
		{
			m.texture = NULL;
			m.kind = MapKind::color;
			m.color.set(conf->getf(i, 2, 0.0f), conf->getf(i, 3, 0.0f), conf->getf(i, 4, 0.0f), conf->getf(i, 5, 1.0f));
			map.push_back(m);
		}
	}
	if(map.empty()) 
	{
		resScript.pop();
		return false;
	}

	blend = map[0].mode != MapMode::replace;
	resScript.pop();
	return true;
}

Material::~Material()
{
	for(steel::vector<Map>::iterator it = map.begin(); it != map.end(); it++)
	{
		if(it->texture) resImage.remove(it->texture);
	}

	resScript.remove(conf);
}
