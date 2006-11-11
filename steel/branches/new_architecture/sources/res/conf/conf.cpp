/*id*********************************************************
	File: res/conf/conf.cpp
	Unit: res/conf
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для хранения настроек
 ************************************************************/

#include "../../steel.h"
#include "conf.h"
#include "../../common/utils.h"
using namespace std;

const string OldConfig::gets(const string key, const string _default) const
{
	std::map<std::string, std::string>::const_iterator it;
	if((it=var_s.find(key)) != var_s.end())
		return it->second;
	else
		return _default;
}

double OldConfig::getd(const string key, const double _default) const
{
	std::map<std::string, double>::const_iterator it;
	if((it=var_f.find(key)) != var_f.end())
		return it->second;
	else
		return _default;
}


float OldConfig::getf(string key, float _default) const
{
	std::map<std::string, double>::const_iterator it;
	if((it=var_f.find(key)) != var_f.end())
		return (float)it->second;
	else
		return _default;
}

int OldConfig::geti(string key, int _default) const
{
	std::map<std::string, double>::const_iterator it;
	if((it=var_f.find(key)) != var_f.end())
		return (int)it->second;
	else
		return _default;
}

v3 OldConfig::getv3(string key, v3 _default) const
{
	std::map<std::string, v3>::const_iterator it;
	if((it=var_v3.find(key)) != var_v3.end())
		return it->second;
	else
		return _default;
}

void OldConfig::setup(std::string key, std::string value)
{
	var_s[key] = value;
	var_f[key] = atof(value.c_str());
}

void OldConfig::setup(std::string key, double value)
{
	var_s[key] = FloatToStr(value);
	var_f[key] = value;
}

void OldConfig::setDefault(std::string key, std::string value)
{
	if(var_s.find(key) == var_s.end())
		var_s[key] = value;

	if(var_f.find(key) == var_f.end())
		var_f[key] = atof(value.c_str());
}

bool OldConfig::unload() 
{ 
	var_s.clear();
	var_f.clear();
	var_v3.clear();
	return true; 
}
