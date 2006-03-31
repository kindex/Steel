#ifndef __RES__CONF_H
#define __RES__CONF_H

#include "../res.h"
#include <string>
#include <map>

class Config: public Res
{
public:
//string settings
	std::map<std::string, std::string> var_s;
// float-number settings
	std::map<std::string, float> var_f;

	void setup(std::string key, std::string value);
	void setDefault(std::string key, std::string value);

	std::string gets(std::string key, std::string _default = "");
	float		getf(std::string key, float _default = 0.0);
	int			geti(std::string key, int	_default = 0);

	bool isset(std::string key) { return var_s.find(key) != var_s.end(); }
};

#endif
