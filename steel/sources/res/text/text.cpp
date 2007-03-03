/*id*******************************************************************************
	File: res/text/text.cpp
	Unit: res/text
	Part of: Steel engine
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Молуль для хранения ресурса типа текст
**************************************************************************************/
#include "text.h"

char* Text::getText() const
{ 
	return text; 
}

int Text::getLength() const// длина текста без /0
{ 
	return length; 
}

TextFormat Text::getFormat() const
{
	return TEXT_ASCII;
}

char Text::getChar(int index)
{
	return text[index];
}

void Text::Cleanup()
{
	if(text) { delete [] text; length = 0; text = NULL; }
}

Text::~Text()
{
	if(text) delete [] text; 
}

std::string Text::getFileName() const
{
	return filename;
}
