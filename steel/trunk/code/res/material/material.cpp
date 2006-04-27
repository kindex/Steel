#include "material.h"
#include "../../common/utils.h"

using namespace std;

Res* createMaterial(const std::string filename, ResCollection *res)
{
	Material *m = new Material;
	if(m->init(filename, res))
	{
		return m;
	}
	else
	{
		delete m;
		return NULL;
	}
}

bool Material::init(string name, ResCollection *res)
{
	conf = (Script*)res->add(Res::script, name + ".mat");
	if(conf == NULL)
	{
		alog.msg("error res material", string("Material not found: ") + name);
		return false;
	} 
	int lines = conf->count();
	
	map.clear();

	vector<string> path = explode('/', name);
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

		string kind = conf->gets(i, 1);
		string file = conf->gets(i, 2);

		if(kind == "color_map")
		{
			m.texture = (Image*)res->add(Res::image, file);
			if(!m.texture)
				m.texture = (Image*)res->add(Res::image, dir + "/" + file);

			if(m.texture)
			{
				m.kind = MapKind::color_map;
				map.push_back(m);
			}
		}
		if(kind == "color")
		{
			m.texture = NULL;
			m.kind = MapKind::color;
			m.color = v4(conf->getf(i, 2, 0.0f), conf->getf(i, 3, 0.0f), conf->getf(i, 4, 0.0f), conf->getf(i, 5, 1.0f));
			map.push_back(m);
		}

	}
	return true;
}

