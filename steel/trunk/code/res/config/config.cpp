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
#include "config_parser.h"

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

double Config::returnd(const double _default)
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

double Config::getd(std::string path, const double _default)
{
	Config *value = find(path);
	if(value == NULL) 
		return _default;
	else
		return value->returnd(_default);
}

v3 Config::getv3(std::string path, const v3 _default)
{
	Config *value = find(path);
	if (value == NULL)
		return _default;
	else
	{
		switch (value->getType())
		{
		case CONFIG_VALUE_STRUCT:
			return ((ConfigStruct*)value)->returnv3(_default);
//		case CONFIG_VALUE_ARRAY:
		default:
			return _default;
		}
	}
}


std::string	Config::gets(std::string path, const std::string _default)
{
	Config *value = find(path);
	if (value == NULL)
		return _default;
	else
		return value->returns(_default);
}

v3 ConfigStruct::returnv3(const v3 _default)
{
	Config *x = getStructElementRaw("x");	if(x == NULL) return _default;
	Config *y = getStructElementRaw("y");	if(y == NULL) return _default;
	Config *z = getStructElementRaw("z");	if(z == NULL) return _default;
	return v3(
				x->returnf(_default.x), 
				y->returnf(_default.y),
				z->returnf(_default.z)
			);
}


std::string Config::returns(const std::string _default)
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


Config* ConfigStruct::getStructElementRaw(const std::string key)
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

string Config::getIndent(int level)
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
//		getIndent(level) + 
		returns();
}

string ConfigString::Dump(int level)
{
	return 
//		getIndent(level) + 
		"'" + returns() + "'";
}

string ConfigStruct::Dump(int level)
{
	level += 2;
	string res("{\n");
	
	for(std::map<std::string, Config*>::iterator it = set.begin(); it != set.end(); it++)
	{
		res += getIndent(level) + it->first + " = ";
		if(it->second)
			res += it->second->Dump(level + 3 + it->first.length()) + "\n";
		else
			res += "null\n";
	}

	level -= 2;
	return res + getIndent(level) + "}";
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

Config* ConfigStruct::find(std::string path)
{
	if(path.empty())
	{
//		error("res config", "Cannot convert struct to simple type");
		return this;
	}
	int s = 0;
	string var = ConfigParser::getAlpha(path.c_str(), s);
	string ext = (string::size_type)s < path.length()?path.substr(s + 1):"";
	Config *child = getStructElement(var);
	if(child == NULL) return NULL;
	return child->find(ext);
}

Config *ConfigSimple::find(std::string path)
{
	if(!path.empty())
		error("res config", "Cannot split simple type into components");
	return this;
}

// ************************* ConfigArray ***********************
Config* ConfigArray::getArrayElementRaw(const size_t index)
{
	if(index >= set.size())
	{
		error("res config", "Array index out of bound");
		return NULL;
	}
	else
		return set[index];

}

std::string ConfigArray::Dump(int level)
{
	level += 2;
	string res("(\n");
	
	for(svector<Config*>::iterator it = set.begin(); it != set.end(); it++)
	{
		res += getIndent(level);

		if(it != NULL)
			res += (*it)->Dump(level) + "\n";
		else
			res += "null\n";
	}

	level -= 2;
	return res + getIndent(level) + ")";}
	
Config *ConfigArray::find(std::string path)
{
	if(path.empty())
	{
//		error("res config", "Cannot convert struct to simple type");
		return this;
	}
	int s = 0;
	int index = (int)ConfigParser::getNumber(path.c_str(), s);
	string ext = (string::size_type)s < path.length()?path.substr(s + 1):"";
	if(index < 0)
	{
		error("res config", "Array index out of bound (<0)");
		return NULL;
	}
	Config *child = getArrayElementRaw(index);

	if(child == NULL) return NULL;
	return child->find(ext);
}

v3 ConfigArray::returnv3(const v3 _default)
{
	Config *x = getArrayElementRaw(0);	if(x == NULL) return _default;
	Config *y = getArrayElementRaw(1);	if(y == NULL) return _default;
	Config *z = getArrayElementRaw(2);	if(z == NULL) return _default;
	return v3(
				x->returnf(_default.x), 
				y->returnf(_default.y),
				z->returnf(_default.z)
			);
}
