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
#include "config_collection.h"

using namespace std;

ResCollection<Config>	resNewConfig;

ConfigTemplate::ConfigTemplate(const string FullPath)
{
	string::size_type it = FullPath.find('#');
	if(it == string::npos)
	{
		local = true;
		localPath = FullPath;
	}
	else
	{
		local = false;
		configId = FullPath.substr(0, it);
		localPath = FullPath.substr(it + 1);
	}
}


Config* Config::find(const std::string path)
{
	Config *result = findInThis(path);
	if(result == NULL) result = findInTemplate(path);
	return result;
}

void Config::toggle(const std::string path)
{
	Config *c = find(path);
	if(c != NULL)
	{
		if(c->getType() == CONFIG_VALUE_NUMBER)
		{
			double v = c->returnd(0.0);
			if(v == 0.0) v = 1.0; else v = 0.0;
			((ConfigNumber*)c)->setValue(v);
		}
	}
}

// TODO: trace path, create if none
void Config::setValued(const std::string path, double value)
{
	Config *c = find(path);
	if(c != NULL)
		if(c->getType() == CONFIG_VALUE_NUMBER)
			((ConfigNumber*)c)->setValue(value);
}


Config* Config::findInTemplate(const std::string path)
{
	for(int i = templates.size() - 1; i >= 0; i--)
	{
		if(templates[i].local)// local template
		{
			Config *parent = getParent();
			if(parent != NULL)
			{
				Config *_template = parent->find(templates[i].localPath);
				if(_template != NULL)
				{
					Config *result = _template->find(path);
					if(result != NULL) return result;
				}
			}
		}
		else // global
		{
			Config *globalTemplate;
			if(!templates[i].configId.empty())
				globalTemplate = resConfig.get(templates[i].configId);
			else
				globalTemplate = getRoot();
			if(globalTemplate != NULL)
			{
				Config *_template = globalTemplate->find(templates[i].localPath);
				if(_template != NULL)
				{
					Config *result = _template->find(path);
					if(result != NULL) return result;
				}
			}
		}
	}
	return NULL;
}

Config* Config::getRoot(void)
{
	Config *a = this;
	Config *b = this;
	while(a != NULL) 
	{
		b = a;
		a = a->parent;
	}
	return b;
}


string Config::genFullId(string someConfigId)
{
	if(someConfigId.find('#') != string::npos)
		return someConfigId;
	else
		return file + "#" + someConfigId;
}

double ConfigString::returnd(const double _default)
{
	string result = returns();
	return atof(result.c_str());
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
	Config *x = getStructElement("x");	if(x == NULL) return _default;
	Config *y = getStructElement("y");	if(y == NULL) return _default;
	Config *z = getStructElement("z");	if(z == NULL) return _default;
	return v3(
				x->returnf(_default.x), 
				y->returnf(_default.y),
				z->returnf(_default.z)
			);
}


std::string ConfigNumber::returns(const std::string _default)
{
	double result = returnd();
	return FloatToStr(result);
}


Config* ConfigStruct::getStructElement(const std::string key)
{
	std::map<std::string, Config*>::iterator it = set.find(key);
	if(it == set.end())
		return NULL;
	else
		return it->second;
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

std::string ConfigTemplate::Dump(void)
{
	if(local) return localPath;
	else return configId + "#" + localPath;
}

std::string Config::DumpPrefix(int level)
{
	if(!templates.empty())
	{
		string res("[");
		for(svector<ConfigTemplate>::const_iterator it = templates.begin(); it != templates.end(); it++)
		{
			if(it != templates.begin())
				res += ", ";
			res += it->Dump();
		}
		res += "] ";
		return res;
	}
	else
		return string();
}

string ConfigNumber::DumpThis(int level)
{
	return returns();
}

string ConfigString::DumpThis(int level)
{
	return "'" + escape(returns(), '\'') + "'";
}

string ConfigStruct::DumpThis(int level)
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

Config* ConfigStruct::findInThis(std::string path)
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

Config *ConfigSimple::findInThis(std::string path)
{
	if(!path.empty())
		error("res config", "Cannot split simple type into components");
	return this;
}

// ************************* ConfigArray ***********************
Config* ConfigArray::getArrayElement(const size_t index)
{
	if(index >= set.size())
	{
		error("res config", "Array index out of bound");
		return NULL;
	}
	else
		return set[index];
}

std::string ConfigArray::DumpThis(int level)
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
	
Config *ConfigArray::findInThis(std::string path)
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
	Config *child = getArrayElement(index);

	if(child == NULL) return NULL;
	return child->find(ext);
}

v3 ConfigArray::returnv3(const v3 _default)
{
	Config *x = getArrayElement(0);	if(x == NULL) return _default;
	Config *y = getArrayElement(1);	if(y == NULL) return _default;
	Config *z = getArrayElement(2);	if(z == NULL) return _default;
	return v3(
				x->returnf(_default.x), 
				y->returnf(_default.y),
				z->returnf(_default.z)
			);
}

