/*id*******************************************************************************
	File: res/text/text.h
	Unit: res/text
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
        Молуль для хранения ресурса типа текст
**************************************************************************************/
#ifndef __RES__TEXT_H
#define __RES__TEXT_H

#include "../../steel.h"
#include "../res.h"
#include <string>

// формат текста
typedef enum
{
	TEXT_ASCII,
	TEXT_UTF8
}TextFormat;


class Text: public Res
{
protected:
	char *text;
	int length;
	TextFormat format;

public:
	Text(void): text(NULL), length(0) {}

	// сам текст в виде z-terminated string (в конце /0)
	char *getText(void) const
	{ 
		return text; 
	}

	// длина текста без /0
	int getLength(void) const
	{ 
		return length; 
	}

	TextFormat getFormat(void) const
	{
		return TEXT_ASCII;
	}

	char getChar(int index)
	{
		return text[index];
	}

	void Cleanup()
	{
		if(text) { delete [] text; length = 0; text = NULL; }
	}

	~Text(void)
	{
		if(text) delete [] text; 
	}
};

#endif
