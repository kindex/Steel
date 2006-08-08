
#include "conf_text.h"
#include "../../common/logger.h"
#include "../../common/utils.h"
#include "../rstream.h"

using namespace std;

bool ConfigText::init(const std::string name, const std::string dir)
{
	std::string file = dir + "/" + name;

	rstream f(file, "conf"); // no binary acces, only text
	if(!f.good()) 
	{
		log_msg("res conf error", "Cannot open file " + file);
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
		var_v3[key] = stov3(value);
	}

	return true;
}
