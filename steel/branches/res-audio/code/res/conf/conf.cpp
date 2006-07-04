
#include "conf.h"
#include "../../common/utils.h"
using namespace std;

string Config::gets(string key, string _default)
{
	if(var_s.find(key) != var_s.end())
		return var_s[key];
	else
		return _default;
}

double Config::getd(string key, double _default)
{
	if(var_f.find(key) != var_f.end())
		return var_f[key];
	else
		return _default;
}


float Config::getf(string key, float _default)
{
	if(var_f.find(key) != var_f.end())
		return (float)var_f[key];
	else
		return _default;
}

int Config::geti(string key, int _default)
{
	if(var_f.find(key) != var_f.end())
		return (int)var_f[key];
	else
		return _default;
}

v3 Config::getv3(string key, v3 _default)
{
	if(var_v3.find(key) != var_v3.end())
		return var_v3[key];
	else
		return _default;
}

void Config::setup(std::string key, std::string value)
{
	var_s[key] = value;
	var_f[key] = atof(value.c_str());
}

void Config::setup(std::string key, double value)
{
	var_s[key] = FloatToStr(value);
	var_f[key] = value;
}

void Config::setDefault(std::string key, std::string value)
{
	if(var_s.find(key) == var_s.end())
		var_s[key] = value;

	if(var_f.find(key) == var_f.end())
		var_f[key] = atof(value.c_str());
}

bool Config::unload() 
{ 
	var_s.clear();
	var_f.clear();
	var_v3.clear();
	return true; 
}