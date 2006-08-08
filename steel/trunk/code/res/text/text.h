#ifndef __RES__TEXT_H
#define __RES__TEXT_H

#include "../res.h"
#include <string>

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
	char *getText(void) const { return text; }
	int getLength(void) const { return length; }
	~Text(void) { if(text) delete [] text; }
};

#endif
