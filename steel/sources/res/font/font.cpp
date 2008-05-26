/*id*******************************************************************************
	File: res/font/font.cpp
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
#include "font.h"

Font::Font() :
    texture(NULL)
{}

Image& Font::getTexture()
{
    return *texture;
}

const Font::Char& Font::getChar(charId id) const
{
    std::map<charId, Char>::const_iterator it = chars.find(id);
    if (it == chars.end())
    {
        return default_char;
    }
    else
    {
        return it->second;
    }
}

int Font::getSize() const
{
    return size;
}

int Font::getWidth(const std::string& text) const
{
    int r = 0;
    for EACH_CONST(std::string, text, it)
    {
        r += getChar(*it).xadvance;
    }
    return r;
}
