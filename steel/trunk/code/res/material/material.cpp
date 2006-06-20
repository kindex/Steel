#include "material.h"
#include "../res_main.h"
#include "../../common/utils.h"
using namespace std;

Material* createMaterial(const std::string filename)
{
	Material *m = new Material;
	if(m->init(filename))
	{
		return m;
	}
	else
	{
		delete m;
		return NULL;
	}
}

bool Material::init(string name)
{
	conf = resScript.add( name + ".mat");
	if(conf == NULL)
	{
		log_msg("error res material", string("Material not found: ") + name);
		return false;
	} 
	int lines = conf->count();
	if(lines == 0) return false;

	map.clear();

	steel::vector<string> path = explode('/', name);
	string filename = path.back();
	path.pop_back();
	string dir = implode('/', path);

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
	if(map.empty()) return false;

	blend = map[0].mode != MapMode::replace;
	return true;
}

