
#include "conf.h"

using namespace std;

string Config::gets(string key, string default)
{
	if(var_s.find(key) != var_s.end())
		return var_s[key];
	else
		return default;
}

float Config::getf(string key, float default)
{
	if(var_f.find(key) != var_f.end())
		return var_f[key];
	else
		return default;
}

int Config::geti(string key, int default)
{
	if(var_f.find(key) != var_f.end())
		return (int)var_f[key];
	else
		return default;
}

void Config::setup(std::string key, std::string value)
{
	var_s[key] = value;
	var_f[key] = atof(value.c_str());
}
void Config::setDefault(std::string key, std::string value)
{
	if(var_s.find(key) == var_s.end())
		var_s[key] = value;

	if(var_f.find(key) == var_f.end())
		var_f[key] = atof(value.c_str());
}
