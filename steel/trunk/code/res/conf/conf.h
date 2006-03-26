#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../res.h"
#include <string>
#include <map>

class Material: public Res
{
public:
//string settings
	std::map<std::string, std::string> var_s;
// float-number settings
	std::map<std::string, float> var_f;
};

#endif
