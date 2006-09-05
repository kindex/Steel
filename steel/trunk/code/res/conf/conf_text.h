/*id*********************************************************
	File: res/conf/conf_text.h
	Unit: res/conf
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для загрузки настроект из ini-файла
 ************************************************************/
#ifndef __RES__CONF_TEXTFILE_H
#define __RES__CONF_TEXTFILE_H

#include "../../steel.h"
#include "conf.h"

class ConfigText:public Config
{
public:
	bool init(const std::string name, const std::string dir);
};

#endif
