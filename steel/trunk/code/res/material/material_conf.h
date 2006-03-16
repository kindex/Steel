#ifndef __RES__MATERIAL_CONF_H
#define __RES__MATERIAL_CONF_H

#include "material.h"

class MaterialConf:public Material
{
	bool init(const std::string name, ResLocatorArray &loadBefore, ResLocatorArray &loadAfter);
	bool unload() { return true; }
};

#endif