/*id*********************************************************
	File: res/script/script_text.h
	Unit: res/script
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для загрузки таблицы настроек из текстового файла. 
		Значения отделяются табуляуцей.
	TODO
		change logic or name
 ************************************************************/

#ifndef RES__SCRIPT_TEXT_H
#define RES__SCRIPT_TEXT_H

#include "../../steel.h"
#include "script.h"

class ScriptText: public Script
{
public:
	bool init(const std::string name, const std::string dir);
};

#endif
