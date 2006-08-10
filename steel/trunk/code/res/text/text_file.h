/*id*******************************************************************************
    Unit: Res Text
    Part of: Steel engine
    (C) DiVision, 2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        Молуль для рагрузки текста из текстового файла
**************************************************************************************/
#ifndef __RES__TEXT_FILE_H
#define __RES__TEXT_FILE_H

#include "../res.h"
#include "text.h"

class TextFile: public Text
{
public:
	bool init(const std::string name, const std::string dir);
};

#endif
