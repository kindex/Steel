/*id*******************************************************************************
	File: res/font/font_text.h
	Unit: res/font
	Part of: Steel engine
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        BMFont text file support
**************************************************************************************/
#pragma once

#include "../../steel.h"
#include "../res.h"
#include "font.h"

class FontText: public Font
{
public:
	bool init(const std::string& name, const std::string dir);
};
