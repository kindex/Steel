#ifndef __RES__CONF_TEXTFILE_H
#define __RES__CONF_TEXTFILE_H

#include "conf.h"

class ConfigText:public Config
{
public:
	bool init(const std::string name, ResCollection &res);
};

Res* createConfigText(const std::string filename, ResCollection *res);

#endif
