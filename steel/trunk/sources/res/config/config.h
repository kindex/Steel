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

#ifndef __RES__CONFIG_H
#define __RES__CONFIG_H

#include "../../steel.h"

#include "../res.h"
#include <string>
#include <map>

typedef enum
{
	CONFIG_VALUE_NONE,
	CONFIG_VALUE_NULL,
	CONFIG_VALUE_NUMBER,
	CONFIG_VALUE_STRING,
	CONFIG_VALUE_STRUCT,
	CONFIG_VALUE_ARRAY
} ConfigValueType;

struct ConfigTemplate
{
	ConfigTemplate(void): local(true) {}
	ConfigTemplate(const std::string FullPath);

	std::string Dump(void);

	std::string localPath;
	std::string configId;
	bool local;

};

class Config: public Res
{
public:
	Config(void): valueIsSet(false), parent(NULL){}
	Config(ConfigValueType _type, bool _valueIsSet): type(_type), valueIsSet(_valueIsSet), parent(NULL) {}

	// common
	virtual ConfigValueType getType(void) const { return type; }
	virtual std::string getShortId(void) const { return id; }
	virtual std::string getFullId(void) const { return file + "#" + id; }

	// Config Base types
	virtual double		returnd(const double _default = 0.0f) { return _default; }
	virtual float		returnf(const float _default = 0.0f) { return (float)returnd((double)_default); } 
	virtual std::string returns(const std::string _default = std::string()) { return _default; }

	// search for config subelement (struct, array)
	virtual Config *find(const std::string path);
	virtual void toggle(const std::string path);
	virtual void setValued(const std::string path, double value);

	// Steel types
	double	getd (const std::string path, const double _default = 0.0f);
	float	getf (const std::string path, const float  _default = 0.0f) { return (float)getd(path, (double)_default); } 
	int		geti (const std::string path, const int	 _default = 0) { return (int)getd(path, (double)_default); } 
	v3		getv3(const std::string path, const v3	 _default = v3(0.0f, 0.0f, 0.0f));
	std::string	gets(const std::string path, const std::string _default = std::string());

	// formated output
	virtual std::string Dump(int level = 0) { return DumpPrefix(level) + DumpThis(level); }

	friend class ConfigParser;

protected:
	virtual Config* findInThis(const std::string path) = 0;
	virtual Config* findInTemplate(const std::string path);

	virtual std::string genFullId(std::string someConfigId);
	Config* getParent(void) { return parent; }
	void setParent(Config *_parent) { parent = _parent; }

	std::string getIndent(int level);
	virtual std::string DumpPrefix(int level = 0);
	virtual std::string DumpThis(int level = 0) = 0;
	void setFile(std::string _file) { file = _file; }
	Config *getRoot(void);

	bool valueIsSet;
	ConfigValueType type;
	std::string file, id;
	Config *parent;
	svector<ConfigTemplate> templates;
};


class ConfigNull: public Config
{
public:
	ConfigNull(void): Config(CONFIG_VALUE_NULL, false) {}

	Config		*findInThis(const std::string path) { return NULL; }
	std::string  DumpThis(int level = 0) { return "null"; }
};

class ConfigSimple: public Config
{
public:
	ConfigSimple(ConfigValueType _type, bool _valueIsSet): Config(_type, _valueIsSet) {}

	Config *findInThis(const std::string path);
};


class ConfigNumber: public ConfigSimple
{
public:
	ConfigNumber(void): ConfigSimple(CONFIG_VALUE_NUMBER, false) { }
	ConfigNumber(double newValue): ConfigSimple(CONFIG_VALUE_NUMBER, true), value(newValue) {}
	
	void		setValue(double newValue) { value = newValue; }
	
	double		returnd(const double _default = 0.0f) { return value; }
	std::string ConfigNumber::returns(const std::string _default = std::string());

	std::string DumpThis(int level);
	std::string finds(const std::string path, const std::string _default = "");

protected:
	double value;
};

class ConfigString: public ConfigSimple
{
public:
	ConfigString(void): ConfigSimple(CONFIG_VALUE_STRING, false) {}
	ConfigString(const std::string newValue): ConfigSimple(CONFIG_VALUE_STRING, true), value(newValue) {}

	void		setValue(const std::string newValue) { value = newValue; }

	std::string returns(const std::string _default = std::string()) { return value; }
	double		returnd(const double _default = 0.0);
	std::string finds(const std::string path, const std::string _default = "");

	std::string DumpThis(int level);

protected:
	std::string value;
};

class ConfigCompound: public Config
{
public:
	ConfigCompound(ConfigValueType _type): Config(_type, true) {}
};


class ConfigStruct: public ConfigCompound
{
public:
	ConfigStruct(void): ConfigCompound(CONFIG_VALUE_STRUCT) { }

	void setValue(const std::string key, Config *value) { set[key] = value; }
	
	Config *findInThis(const std::string path);
	v3		returnv3(const v3 _default = v3(0.0f, 0.0f, 0.0f));
	Config* getStructElement(const std::string key);

	std::string DumpThis(int level);

protected:
	std::map<std::string, Config*> set;
};

class ConfigArray: public ConfigCompound
{
public:
	ConfigArray(void): ConfigCompound(CONFIG_VALUE_ARRAY) {}
	
	void push(Config* newValue) { set.push_back(newValue); }

	Config *findInThis(const std::string path);
	v3 returnv3(const v3 _default = v3(0.0f, 0.0f, 0.0f));
	Config* getArrayElement(const size_t index);
	int getArraySize(const int index) { return set.size(); }

	std::string DumpThis(int level);

protected:

	svector<Config*> set;
};




#endif
