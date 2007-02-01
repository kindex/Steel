/*id*********************************************************
	File: res/config/config.h
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
	ConfigTemplate(): local(true) {}
	ConfigTemplate(const std::string &FullPath);

	const std::string Dump() const;

	std::string localPath;
	std::string configId;
	bool local;

};
class ConfigArray;

class Config;
typedef Config* PConfig;
typedef std::map<std::string, Config*> PConfigDict;
class Config: public Res
{
public:
	Config(): valueIsSet(false), parent(NULL){}
	Config(ConfigValueType _type, bool _valueIsSet): type(_type), valueIsSet(_valueIsSet), parent(NULL) {}

	// common
	virtual ConfigValueType getType() const { return type; }
	virtual std::string		getShortId() const { return id; }
	virtual std::string		getFullId() const { return file + "#" + id; }

	// Config Base types
	virtual double		returnd(const double _default = 0.0f) const { return _default; }
	virtual float		returnf(const float _default = 0.0f) const { return (float)returnd((double)_default); } 
	virtual const std::string returns(const std::string& _default = std::string()) const { return _default; }

	// search for config subelement (struct, array)
	virtual const	Config* find(const std::string& path) const;
	virtual			Config* find(const std::string& path);
	virtual void toggle(const std::string& path);
	virtual void setValued(const std::string& path, double value);
	virtual void setValues(const std::string& path, const std::string& value);

	// Steel types
			double		getd (const std::string& path, const double _default = 0.0)		const;
			float		getf (const std::string& path, const float	_default = 0.0f)	const { return (float)getd(path, (double)_default); } 
			int			geti (const std::string& path, const int	_default = 0)		const { return   (int)getd(path, (double)_default); } 
			bool		getb (const std::string& path, const bool	_default = false)	const { return        geti(path, _default?1:0) > 0; } 
			v3			getv3(const std::string& path, const v3		_default = v3(0.0f, 0.0f, 0.0f)) const;
	const	std::string	gets(const std::string& path, const std::string &_default = std::string()) const;
	const	ConfigArray*getArray(const std::string& path) const;
			ConfigArray*getArray(const std::string &path);
	// полный путь к файлу относительно конфига (включая шаблоны)
			std::string getPath(const std::string &path, const std::string &_default = std::string());

	// formated output
	virtual std::string Dump(int level = 0) { return DumpPrefix(level) + DumpThis(level); }
	
	// директория, из которой был загружен конфиг
	std::string getConfigFilePath() const;
	virtual void setFilePath(const std::string &_file) { file = _file; }

	friend class ConfigParser;

protected:
	virtual const	Config* findInThis(const std::string& path) const abstract;
	virtual			Config* findInThis(const std::string& path) abstract;
	virtual const	Config* findInTemplate(const std::string& path) const;
	virtual			Config* findInTemplate(const std::string& path);
	virtual	const	Config* findInParent(const std::string& path) const;
	virtual			Config* findInParent(const std::string& path);

	virtual std::string genFullId(std::string someConfigId) const;
	const Config* getParent() const { return parent; }
	Config* getParent() { return parent; }
	void setParent(Config *_parent) { parent = _parent; }

	const std::string getIndent(int level) const;
	virtual const std::string DumpPrefix(int level = 0) const;
	virtual const std::string DumpThis(int level = 0) const abstract;
	void setFile(std::string _file) { file = _file; }
	const Config *getRoot() const;
	Config *getRoot();

	bool valueIsSet;
	ConfigValueType type;
	std::string file, id;
	Config *parent;
	svector<ConfigTemplate> templates;
};


class ConfigNull: public Config
{
public:
	ConfigNull(): Config(CONFIG_VALUE_NULL, false) {}

	const	Config* findInThis(const std::string &path) const { return NULL; }
	Config* findInThisForce(const std::string &path);
	Config* findInThis(const std::string &path) { return NULL; }
	const std::string  DumpThis(int level = 0)  const { return "null"; }
};

class ConfigSimple: public Config
{
public:
	ConfigSimple(ConfigValueType _type, bool _valueIsSet): Config(_type, _valueIsSet) {}

	const Config* findInThis(const std::string &path) const;
	const Config* findInThisForce(const std::string &path) const;
	Config *findInThis(const std::string &path);
};


class ConfigNumber: public ConfigSimple
{
public:
	ConfigNumber(): ConfigSimple(CONFIG_VALUE_NUMBER, false) { }
	ConfigNumber(double newValue): ConfigSimple(CONFIG_VALUE_NUMBER, true), value(newValue) {}
	
	void	setValue(double newValue) { value = newValue; }
	
	double	returnd(const double _default = 0.0f) const { return value; }
	const	std::string ConfigNumber::returns(const std::string &_default = std::string()) const;

	const	std::string DumpThis(int level) const;
	const	std::string finds(const std::string &path, const std::string &_default = "") const;

protected:
	double value;
};

class ConfigString: public ConfigSimple
{
public:
	ConfigString(): ConfigSimple(CONFIG_VALUE_STRING, false) {}
	ConfigString(const std::string &newValue): ConfigSimple(CONFIG_VALUE_STRING, true), value(newValue) {}

	void	setValue(const std::string &newValue) { value = newValue; }

	const	std::string returns(const std::string &_default = std::string()) const { return value; }
	double	returnd(const double _default = 0.0) const;
	const	std::string finds(const std::string &path, const std::string &_default = "") const;

	const	std::string DumpThis(int level) const;

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
	ConfigStruct(): ConfigCompound(CONFIG_VALUE_STRUCT) { }

	void setValue(const std::string &key, Config *value) { set[key] = value; }
	
	const Config *findInThis(const std::string &path) const;
		  Config *findInThis(const std::string &path);
	const v3		returnv3(const v3 _default = v3(0.0f, 0.0f, 0.0f)) const;
 	const Config* getStructElement(const std::string &key) const;
		  Config* getStructElement(const std::string &key);

	const std::string DumpThis(int level) const;
	void setFilePath(const std::string &_file);
	std::string findKey(const Config* value) const;

protected:
	PConfigDict set;
};

class ConfigArray: public ConfigCompound
{
public:
	ConfigArray(): ConfigCompound(CONFIG_VALUE_ARRAY) {}
	
	void push(Config* newValue) { set.push_back(newValue); }

	const Config *findInThis(const std::string &path) const;
	Config *findInThis(const std::string &path);
	const v3 returnv3(const v3 _default = v3(0.0f, 0.0f, 0.0f)) const;
	const Config* getArrayElement(const size_t index) const;
	Config* getArrayElement(const size_t index);
	size_t size() const { return set.size(); }

	const std::string DumpThis(int level) const;
	void setFilePath(const std::string &_file);

// iterators
	typedef pvector<Config*>::iterator iterator;
	typedef pvector<Config*>::const_iterator const_iterator;
	pvector<Config*>::const_iterator begin() const { return set.begin(); }
	pvector<Config*>::const_iterator end() const { return set.end(); }
	pvector<Config*>::iterator begin() { return set.begin(); }
	pvector<Config*>::iterator end() { return set.end(); }
protected:

	pvector<Config*> set;
};




#endif
