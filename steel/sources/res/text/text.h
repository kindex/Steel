/*id*******************************************************************************
	File: res/text/text.h
	Unit: res/text
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
enum TextFormat
{
	TEXT_ASCII,
	TEXT_UTF8
};


class Text: public Res
{
public:
	Text(): text(NULL), length(0) {}

	// сам текст в виде z-terminated string (в конце /0)
	char *getText() const;
	int getLength() const;// длина текста без /0
	TextFormat getFormat() const;
	char getChar(int index);
	void Cleanup();
	~Text();
	std::string getFileName() const;

protected:
	std::string filename;
	char *text;
	int length;
	TextFormat format;
};

#endif
