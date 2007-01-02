/*id*********************************************************
	File: res/config/config.cpp
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

ConfigTemplate::ConfigTemplate(const string &FullPath)
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


const Config* Config::find(const std::string &path) const
{
	const Config *result = findInThis(path);
	if(result == NULL) result = findInTemplate(path);
	return result;
}

Config* Config::find(const std::string &path)
{
	Config *result = findInThis(path);
	if(result == NULL) result = findInTemplate(path);
	return result;
}


void Config::toggle(const std::string &path)
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
void Config::setValued(const std::string& path, double value)
{
	Config *c = find(path);
	if(c != NULL)
	{
		switch (c->getType())
		{
		case CONFIG_VALUE_NUMBER:
			static_cast<ConfigNumber*>(c)->setValue(value);
			break;
		case CONFIG_VALUE_STRING:
			static_cast<ConfigString*>(c)->setValue(FloatToStr(value));
			break;
		}
	}
}
void Config::setValues(const std::string& path, const std::string& value)
{
	Config *c = find(path);
	if(c != NULL)
	{
		switch (c->getType())
		{
		case CONFIG_VALUE_STRING:
			static_cast<ConfigString*>(c)->setValue(value);
			break;
		case CONFIG_VALUE_NUMBER:
			static_cast<ConfigNumber*>(c)->setValue(atoi(value.c_str()));
			break;
		}
	}
}


const Config* Config::findInTemplate(const std::string &path) const
{
	for(int i = templates.size() - 1; i >= 0; i--)
	{
		if(templates[i].local)// local template
		{
 			const Config *parent = getParent();
			if(parent != NULL)
			{
				const Config *_template = parent->find(templates[i].localPath);
				if(_template != NULL)
				{
					const Config *result = _template->find(path);
					if(result != NULL) return result;
				}
			}
		}
		else // global
		{
			const Config *globalTemplate;
			if(!templates[i].configId.empty())
				globalTemplate = resConfig.add(createPath(getConfigFilePath(), templates[i].configId));
			else
				globalTemplate = getRoot();
			if(globalTemplate != NULL)
			{
				const Config *_template = globalTemplate->find(templates[i].localPath);
				if(_template != NULL)
				{
					const Config *result = _template->find(path);
					if(result != NULL) return result;
				}
			}
		}
	}
	return NULL;
}

Config* Config::findInTemplate(const std::string &path)
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
				globalTemplate = resConfig.add(createPath(getConfigFilePath(), templates[i].configId));
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


const Config* Config::getRoot() const
{
	const Config *a = this;
	const Config *b = this;
	while(a != NULL) 
	{
		b = a;
		a = a->parent;
	}
	return b;
}

Config* Config::getRoot()
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

string Config::genFullId(string someConfigId) const
{
	if(someConfigId.find('#') != string::npos)
		return someConfigId;
	else
		return file + "#" + someConfigId;
}

double ConfigString::returnd(const double _default) const
{
	string result = returns();
	return atof(result.c_str());
}

double Config::getd(const std::string &path, const double _default) const
{
	const Config *value = find(path);
	if(value == NULL) 
		return _default;
	else
		return value->returnd(_default);
}

v3 Config::getv3(const std::string &path, const v3 _default) const
{
	const Config *value = find(path);
	if (value == NULL)
		return _default;
	else
	{
		switch (value->getType())
		{
		case CONFIG_VALUE_STRUCT:
			return static_cast<const ConfigStruct*>(value)->returnv3(_default);
		case CONFIG_VALUE_ARRAY:
			return static_cast<const ConfigArray*>(value)->returnv3(_default);

		default:
			return _default;
		}
	}
}


const std::string	Config::gets(const std::string &path, const std::string &_default) const
{
	const Config *value = find(path);
	if (value == NULL)
		return _default;
	else
		return value->returns(_default);
}

const ConfigArray* Config::getArray(const std::string &path) const
{
	const Config *value = find(path);
	if (value != NULL && value->getType() == CONFIG_VALUE_ARRAY)
		return static_cast<const ConfigArray*>(value);
	else
		return NULL;
}

ConfigArray* Config::getArray(const std::string &path)
{
	Config *value = find(path);
	if (value != NULL && value->getType() == CONFIG_VALUE_ARRAY)
		return static_cast<ConfigArray*>(value);
	else
		return NULL;
}

std::string Config::getConfigFilePath() const
{
	string path, name;
	splitPath(file, path, name);
	return path;
}

std::string Config::getPath(const std::string &path, const std::string &_default)
{
	Config *value = find(path);
	string base;
	string res;
	if (value != NULL && value->getType() == CONFIG_VALUE_STRING)
	{
		res = value->returns(_default);
		base = value->getConfigFilePath();
	}
	else
	{
		res =  _default;
		base = this->getConfigFilePath();
	}

	return createPath(base, res);
}


const v3 ConfigStruct::returnv3(const v3 _default) const
{
	const Config *x = getStructElement("x");	if(x == NULL) return _default;
	const Config *y = getStructElement("y");	if(y == NULL) return _default;
	const Config *z = getStructElement("z");	if(z == NULL) return _default;
	return v3(
				x->returnf(_default.x), 
				y->returnf(_default.y),
				z->returnf(_default.z)
			);
}


const std::string ConfigNumber::returns(const std::string &_default) const
{
	double result = returnd();
	return FloatToStr(result);
}


const Config* ConfigStruct::getStructElement(const std::string &key) const
{
	std::map<std::string, Config*>::const_iterator it = set.find(key);
	if(it == set.end())
		return NULL;
	else
		return it->second;
}

Config* ConfigStruct::getStructElement(const std::string &key)
{
	std::map<std::string, Config*>::iterator it = set.find(key);
	if(it == set.end())
		return NULL;
	else
		return it->second;
}

const string Config::getIndent(int level) const
{
	string res;
	while(level-->0)
	{
		res.push_back(' ');
	}
	return res;
}

const std::string ConfigTemplate::Dump() const
{
	if(local) return localPath;
	else return configId + "#" + localPath;
}

const std::string Config::DumpPrefix(int level) const
{
	if(!templates.empty())
	{
		string res("[");
		for (svector<ConfigTemplate>::const_iterator it = templates.begin(); it != templates.end(); it++)
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

const string ConfigNumber::DumpThis(int level) const
{
	return returns();
}

const string ConfigString::DumpThis(int level) const
{
	return "'" + escape(returns(), '\'') + "'";
}

const string ConfigStruct::DumpThis(int level) const
{
	level += 2;
	string res("{\n");
	
	for(std::map<std::string, Config*>::const_iterator it = set.begin(); it != set.end(); it++)
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

const std::string ConfigNumber::finds(const std::string &path, const std::string &_default) const
{
	if(!path.empty())
	{
		error("res config", "Cannot find variable '" + path + "' in number");
	}
	return gets(_default);
}

const std::string ConfigString::finds(const std::string &path, const std::string &_default) const
{
	if(!path.empty())
	{
		error("res config", "Cannot cast string as struct (variable '" + path + "')");
		return _default;
	}
	return gets(_default);
}

const Config* ConfigStruct::findInThis(const std::string &path) const
{
	if(path.size() < 1)
	{
//		error("res config", "Cannot convert struct to simple type");
		return this;
	}
	
	int s = 0;
	if(path[0] == '.') s++;

	string var = ConfigParser::getAlpha(path.c_str(), s);
	string ext = (string::size_type)s < path.length()?path.substr(s):"";
	const Config *child = getStructElement(var);
	if(child == NULL) return NULL;
	return child->find(ext);
}

Config* ConfigStruct::findInThis(const std::string &path)
{
	if(path.size() < 1)
	{
//		error("res config", "Cannot convert struct to simple type");
		return this;
	}
	
	int s = 0;
	if(path[0] == '.') s++;

	string var = ConfigParser::getAlpha(path.c_str(), s);
	string ext = (string::size_type)s < path.length()?path.substr(s):"";
	Config *child = getStructElement(var);
	if(child == NULL) return NULL;
	return child->find(ext);
}

const Config *ConfigSimple::findInThis(const std::string &path) const
{
	if(!path.empty())
		error("res config", "Cannot split simple type into components (remind path '" + path + "')");
	return this;
}
Config *ConfigSimple::findInThis(const std::string &path)
{
	if(!path.empty())
		error("res config", "Cannot split simple type into components");
	return this;
}

// ************************* ConfigArray ***********************
const Config* ConfigArray::getArrayElement(const size_t index) const
{
	if(index >= set.size())
	{
		error("res config", "Array index out of bound");
		return NULL;
	}
	else
		return set[index];
}

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

const std::string ConfigArray::DumpThis(int level) const
{
	level += 2;
	string res("(\n");
	
	for EACH_CONST(pvector<Config*>, set, it)
	{
		res += getIndent(level);

		if(*it != NULL)
		{
			res += (*it)->Dump(level) + "\n";
		}
		else
		{
			res += "null\n";
		}
	}

	level -= 2;
	return res + getIndent(level) + ")";}
	
const Config *ConfigArray::findInThis(const std::string &path) const
{
	if(path.size()<3)
	{
//		error("res config", "Cannot convert struct to simple type");
		return this;
	}
	int s = 1;
	if(path[0] != '[')
	{
		error("res config", "Error in path ('" + path + "'). Expecting [");
		return this;
	}
	int index = (int)ConfigParser::getNumber(path.c_str(), s);
	if(path[s] != ']')
	{
		error("res config", "Error in path ('" + path + "'). Expecting ]");
		return this;
	}
	string ext = (string::size_type)s < path.length()?path.substr(s + 1):"";
	if(index < 0)
	{
		error("res config", "Array index out of bound (<0)");
		return NULL;
	}
	const Config *child = getArrayElement(index);

	if(child == NULL) return NULL;
	return child->find(ext);
}

Config *ConfigArray::findInThis(const std::string &path)
{
	if(path.size()<3)
	{
//		error("res config", "Cannot convert struct to simple type");
		return this;
	}
	int s = 1;
	if(path[0] != '[')
	{
		error("res config", "Error in path ('" + path + "'). Expecting [");
		return this;
	}
	int index = (int)ConfigParser::getNumber(path.c_str(), s);
	if(path[s] != ']')
	{
		error("res config", "Error in path ('" + path + "'). Expecting ]");
		return this;
	}
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


const v3 ConfigArray::returnv3(const v3 _default) const
{
	const Config *x = getArrayElement(0);	if(x == NULL) return _default;
	const Config *y = getArrayElement(1);	if(y == NULL) return _default;
	const Config *z = getArrayElement(2);	if(z == NULL) return _default;
	return v3(
				x->returnf(_default.x), 
				y->returnf(_default.y),
				z->returnf(_default.z)
			);
}

void ConfigArray::setFilePath(const std::string &_file)
{
	file = _file;
	for EACH(pvector<Config*>, set, it)
	{
		(*it)->setFilePath(_file);
	}
}

void ConfigStruct::setFilePath(const std::string &_file)
{
	file = _file;
	for(std::map<std::string, Config*>::iterator it = set.begin(); it != set.end(); it++)
	{
		it->second->setFilePath(_file);
	}
}

