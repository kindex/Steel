/*id*********************************************************
	File: res/config/config.h
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для хранения настроек
 ************************************************************/

#include "../../steel.h"

#include "config.h"

using namespace std;

ResCollection<Config>	resNewConfig;

char *Config::getValueFromParent(void)
{
	for(size_t i = 0; i < inherits.size(); i++)
	{
		std::string parentName = genFullId(inherits[i]);
		Config *parent;
		if(!(parent = resNewConfig.get(parentName)))
		{
			parent = resNewConfig.add(parentName);
			if(!parent)
			{
				error("res config", "Cannot find inherited config '" + parentName + "' in config '" + getFullId() +"'");
				continue;
			}
		}
		char *result = parent->getValue();
		if(result == NULL) 
			continue;
		else
			return result;
	}

	return NULL;
}

char *Config::getValue(void)
{
	char *result = getValueRaw();
	if(result != NULL) return result;
	
	result = getValueFromParent();
	if(result != NULL) 
		return result;
	else
		return NULL;
}

string Config::genFullId(string someConfigId)
{
	if(someConfigId.find('#') != string::npos)
		return someConfigId;
	else
		return file + "#" + someConfigId;
}

double Config::getd(const double _default)
{
	if(getType() == CONFIG_VALUE_NUMBER)
	{
		double *result = (double*)getValue();
		if(!result)
			return _default;
		else
			return *result;
	}
	if(getType() == CONFIG_VALUE_STRING)
	{
		string *result = (string*)getValue();
		if(!result)
			return _default;
		else
		{
			return atof(result->c_str());
		}
	}
	else
	return _default;
}

std::string Config::gets(const std::string _default)
{
	if(getType() == CONFIG_VALUE_STRING)
	{
		std::string *result = (string*)getValue();
		if(!result)
			return _default;
		else
			return *result;
	}
	else
	if(getType() == CONFIG_VALUE_NUMBER)
	{
		double *result = (double*)getValue();
		if(!result)
			return _default;
		else
			return FloatToStr(*result);
	}
	else
	return _default;
}


Config* ConfigArray::getStructElementRaw(const std::string key)
{
	std::map<std::string, Config*>::iterator it = set.find(key);
	if(it == set.end())
		return NULL;
	else
		return it->second;
}

Config* Config::getStructElement(const std::string key)
{
	Config* result = getStructElementRaw(key);
	if(result != NULL) return result;

	for(size_t i = 0; i < inherits.size(); i++)
	{
		std::string parentName = genFullId(inherits[i]);
		Config *parent;
		if(!(parent = resNewConfig.get(parentName)))
		{
			parent = resNewConfig.add(parentName);
			if(!parent)
			{
				error("res config", "Cannot find inherited config '" + parentName + "' in config '" + getFullId() +"'");
				continue;
			}
		}

		Config *result = parent->getStructElement(key);
		if(!result) 
			continue;
		else
			return result;
	}

	return NULL;
}

string Config::getParagraph(int level)
{
	string res;
	while(level-->0)
	{
		res.push_back(' ');
	}
	return res;
}


string ConfigNumber::Dump(int level)
{
	return 
//		getParagraph(level) + 
		gets();
}

string ConfigString::Dump(int level)
{
	return 
//		getParagraph(level) + 
		"'" + gets() + "'";
}

string ConfigArray::Dump(int level)
{
	level += 2;
	string res("{\n");
	
	for(std::map<std::string, Config*>::iterator it = set.begin(); it != set.end(); it++)
	{
		res += getParagraph(level) + it->first + " = ";
		if(it->second)
			res += it->second->Dump(level + 3 + it->first.length()) + "\n";
		else
			res += "null\n";
	}

	level -= 2;
	return res + getParagraph(level) + "}";
}

std::string ConfigNumber::finds(std::string path, const std::string _default)
{
	if(!path.empty())
		error("res config", "Cannot find variable '" + path + "' in number");
	return gets(_default);
}

std::string ConfigString::finds(std::string path, const std::string _default)
{
	if(!path.empty())
	{
		error("res config", "Cannot cast string as struct (variable '" + path + "')");
		return _default;
	}
	return gets(_default);
}

std::string ConfigArray::finds(std::string path, const std::string _default)
{
	if(path.empty())
	{
		error("res config", "Cannot convert struct to string");
		return _default;
	}
	int s = 0;
	string var = ParseAlpha(path.c_str(), s);
	string ext = s < path.length()?path.substr(s + 1):"";
	Config *child = getStructElement(var);
	if(!child) return _default;
	return child->finds(ext);
	
}

int ConfigArray::getFreeIndex(void)
{
	for(;;)
	{
		string key = IntToStr(freeIndex);
		freeIndex++;
		if(set.find(key) == set.end())
			return freeIndex - 1;
	}
}
