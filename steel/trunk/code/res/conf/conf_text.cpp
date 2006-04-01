
#include "conf_text.h"
#include "../../common/logger.h"
#include <fstream>

using namespace std;

bool ConfigText::init(const std::string name, ResCollection &res)
{
	std::string file = "../res/" + name + ".conf";

	rstream f(file); // no binary acces, only text
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
		if(s.length()<=2) continue;
		
        if(s[s.length()-1] == '\r') s.resize(s.length()-1);

		if(s.substr(0,2) == "//" || s[0] == ';' || s[0] == '#') continue;

		int d = s.find('=');
		if(d<0) continue;
		string key = s.substr(0, d);
		string value = s.substr(d+1, s.size()-d-1);

		var_s[key] = value;
		var_f[key] = atof(value.c_str());
	}

	return true;
}
