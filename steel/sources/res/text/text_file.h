/*id*******************************************************************************
	File: res/text/text_file.h
	Unit: res/text
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Молуль для рагрузки текста из текстового файла
**************************************************************************************/
#ifndef __RES__TEXT_FILE_H
#define __RES__TEXT_FILE_H

#include "../../steel.h"
#include "../res.h"
#include "text.h"

class TextFile: public Text
{
public:
	bool init(const std::string& name, const std::string dir);
};

char* readfile(std::string file);


#endif
