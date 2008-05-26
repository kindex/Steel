/*id*******************************************************************************
	File: res/font/font.h
	Unit: res/font
	Part of: Steel engine
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Font storing
**************************************************************************************/
#pragma once

#include "../../steel.h"
#include "../res.h"
#include "../image/image.h"
#include <string>
#include <map>

class Font: public Res
{
public:
    Font();
    Image& getTexture();

    struct Char
    {
        int x;
        int y;
        int width;
        int height;
        int xoffset;
        int yoffset;
        int xadvance;
    };

    typedef unsigned int charId;
    const Char& getChar(charId id) const;
    int getSize() const;
    int getWidth(const std::string& text) const;

protected:
	std::string name;
    Image* texture;
    int size;
    std::map<charId, Char> chars;
    Char default_char;
};
