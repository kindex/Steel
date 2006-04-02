#ifndef __RES__CONF_H
#define __RES__CONF_H

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

public:
	void setup(std::string key, std::string value);
	void setup(std::string key, double value);
	void setDefault(std::string key, std::string value);

	std::string gets(std::string key, std::string _default = "");
	float		getf(std::string key, float _default = 0.0);
	double		getd(std::string key, double _default = 0.0);
	int			geti(std::string key, int	_default = 0);

	bool isset(std::string key) { return var_s.find(key) != var_s.end(); }

	bool unload() 
	{ 
		var_s.clear();
		var_f.clear();
		return true; 
	}
};

#endif
