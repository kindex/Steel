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

class Config: public Res
{
protected:
	ConfigValueType type;
	std::string file, id;
	svector<std::string> inherits;
	bool _default;

public:
	virtual ConfigValueType getType(void) const { return type; }
	virtual std::string getShortId(void) const { return id; }
	virtual std::string getFullId(void) const { return file + "#" + id; }
	virtual std::string genFullId(std::string someConfigId);

	virtual char *getValueRaw(void) { return NULL; }
	virtual char *getValue(void);
	virtual char *getValueFromParent(void);

	// Config Base types
	double returnd(const double _default = 0.0f);
	float returnf(const float _default = 0.0f) { return (float)returnd((double)_default); } 
	std::string returns(const std::string _default = "");

	// Steel types
	double	getd (std::string path, const double _default = 0.0f);
	float	getf (std::string path, const float  _default = 0.0f) { return (float)getd(path, (double)_default); } 
	int		geti (std::string path, const int	 _default = 0) { return (int)getd(path, (double)_default); } 
	v3		getv3(std::string path, const v3	 _default = v3(0.0f, 0.0f, 0.0f));
	std::string	gets(std::string path, const std::string _default = std::string());


	virtual Config *find(std::string path) = 0;

	virtual Config* getStructElement(const std::string key);
	virtual Config* getStructElementRaw(const std::string key) { return NULL; }

	std::string getIndent(int level);
	virtual std::string Dump(int level = 0) = 0;
};

class ConfigSimple: public Config
{
public:
	Config *find(std::string path);

};


class ConfigNumber: public ConfigSimple
{
protected:
	double value;

public:
	ConfigNumber(void) { type = CONFIG_VALUE_NUMBER; }
	void setValue(double newValue) { value = newValue; }
	char *getValueRaw(void) { return (char*)&value; }
	std::string Dump(int level);
	std::string finds(std::string path, const std::string _default = "");
};

class ConfigString: public ConfigSimple
{
protected:
	std::string value;

public:
	ConfigString(void){ type = CONFIG_VALUE_STRING; }

	void setValue(std::string newValue) { value = newValue; }
	char *getValueRaw(void) { return (char*)&value; }
	std::string Dump(int level);
	std::string finds(std::string path, const std::string _default = "");
};

class ConfigCompound: public Config
{
};


class ConfigStruct: public ConfigCompound
{
protected:
	std::map<std::string, Config*> set;

public:
	ConfigStruct(void){ type = CONFIG_VALUE_STRUCT; }

	Config* getStructElementRaw(const std::string key);
	void setValue(const std::string key, Config *value) { set[key] = value; }
	std::string Dump(int level);
	
	Config *find(std::string path);
	v3 returnv3(const v3 _default = v3(0.0f, 0.0f, 0.0f));
};

class ConfigArray: public ConfigCompound
{
protected:
	svector<Config*> set;

public:
	ConfigArray(void){ type = CONFIG_VALUE_ARRAY; }

	Config* getArrayElementRaw(const size_t index);
	int getArraySizeRaw(const int index) { return set.size(); }
	std::string Dump(int level);
	
	Config *find(std::string path);
	v3 returnv3(const v3 _default = v3(0.0f, 0.0f, 0.0f));
	void push(Config* newValue) { set.push_back(newValue); }
};


#define parse_error(keys, msg) (log_msg((keys), (msg)))

Config*			ParseConfig(const char* body, int &i);
Config*			ParseValue(const char* body, int &i);
std::string		ParseAlpha(const char* body, int &i);
void			SkipSpaces(const char* body, int &i);
ConfigStruct*	ParseStruct(const char* body, int &i);
ConfigArray*	ParseArray(const char* body, int &i);
double			ParseNumber(const char* body, int &i);


#endif
