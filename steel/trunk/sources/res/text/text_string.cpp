/*id*******************************************************************************
	File: res/text/text_srting.cpp
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

#include "../../steel.h"
#include "text_string.h"

TextString::TextString(const std::string original): Text()
{
	Setup(original);
}

void TextString::Setup(const std::string original)
{
	Cleanup();

	length = original.size() + 1;
	text = new char[length];
	
	strcpy(text, original.c_str());
}
