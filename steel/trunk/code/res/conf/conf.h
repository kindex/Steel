/*id*********************************************************
	File: res/conf/conf.h
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

#ifndef __RES__CONF_H
#define __RES__CONF_H

#include "../../steel.h"
#include "../res.h"
#include <string>
#include <map>

class Config: public Res
{
protected:
//string settings
	std::map<std::string, std::string> var_s;
// double-number settings
	std::map<std::string, double> var_f;
	std::map<std::string, v3> var_v3;

public:
	// change 1->0, 0->1
	void toggle(std::string key) { setup(key, !geti(key)); }

	void setup(std::string key, std::string value);
	void setup(std::string key, double value);
	void setDefault(std::string key, std::string value);

	const std::string gets(const std::string key, const std::string _default = "") const;
	float		getf(const std::string key, const float _default = 0.0) const;
	double getd(const std::string key, const double _default = 0.0) const;
	int			geti(const std::string key, const int	_default = 0) const;
	v3			getv3(const std::string key, const v3 _default = v3(0,0,0)) const;

	bool isset(std::string key) { return var_s.find(key) != var_s.end(); }
	bool unload(void);
};
#endif
