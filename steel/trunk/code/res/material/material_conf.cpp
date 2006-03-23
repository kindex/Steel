#include "material_conf.h"
#include "../../common/logger.h"
#include <fstream>

bool MaterialConf::init(const std::string name, ResCollection &res)
{
	var_s = new std::map<std::string, std::string>;
	var_f = new std::map<std::string, float>;

	std::string file = "../res/material/" + name + ".mat";

	rstream f(file, 0); // no binary acces, only text
	if(!f.good()) 
	{
		alog.out("Res/Material/Conf: cannot open file %s", file.c_str());
		return false;
	}

	while(f.good())
	{
		char buf[1024];
		f.getline(buf, 1023);
		std::string s = buf;

		if(s.substr(0,2) == "//" || s[0] == ';' || s[0] == '#') continue;

		int d = s.find('=');
		if(d<0) continue;
		std::string key = s.substr(0, d);
		std::string value = s.substr(d+1, s.size()-d-1);

		var_s->operator[](key) = value;
	//	var_f->insert(key, atof(value.c_str()));
	}

	return true;
}
