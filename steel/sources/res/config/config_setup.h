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
#ifndef __RES__CONFIF_SETUP_H__
#define __RES__CONFIF_SETUP_H__

#include "../../steel.h"
#include "config.h"

void setup(PConfig& conf, const std::string& path, Config* newValue);
bool executeScript(Config* conf, const std::string& script);
bool executeCommand(Config* conf, const std::string& command);

#endif
