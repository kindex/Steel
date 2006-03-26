#ifndef __RES__MATERIAL_CONF_H
#define __RES__MATERIAL_CONF_H

#include "material.h"

class MaterialConf:public Material
{
	bool init(const std::string name, ResCollection &res);
	bool unload() { return true; }
};

#endif