/*id*******************************************************************************
	Unit: Res Text
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
        ������ ��� �������� ������� ���� �����
**************************************************************************************/
#ifndef __RES__TEXT_H
#define __RES__TEXT_H

#include "../res.h"
#include <string>

// ������ ������
typedef enum
{
	TEXT_ASCII,
	TEXT_UTF8
}TextFormat;


class Text: public Res
{
protected:
	unsigned char *text;
	int length;
	TextFormat format;

public:
	Text(void): text(NULL), length(0) {}

	// ��� ����� � ���� z-terminated string (� ����� /0)
	unsigned char *getText(void) const
	{ 
		return text; 
	}

	// ����� ������ ��� /0
	int getLength(void) const
	{ 
		return length; 
	}

	TextFormat gerFormat(void) const
	{
		return TEXT_ASCII;
	}


	~Text(void)
	{
		if(text) delete [] text; 
	}
};

#endif
