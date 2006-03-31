#ifndef __RES__CONF_TEXTFILE_H
#define __RES__CONF_TEXTFILE_H

#include "conf.h"

class ConfigText:public Config
{
	bool init(const std::string name, ResCollection &res);
	bool unload() { return true; }
};

#endif
