/*id*******************************************************************************
	File: res/text/text_srting.h
	Unit: res/text
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Молуль для рагрузки текста из строки (std::string)
**************************************************************************************/
#ifndef __RES__TEXT_STRING_H
#define __RES__TEXT_STRING_H

#include "../../steel.h"
#include "../res.h"
#include "text.h"

class TextString: public Text
{
public:
	TextString(void) {}
	TextString(const std::string original);

	void Setup(const std::string original);
};


#endif
