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
	TextFormat format;

public:
	Text(): text(NULL) {}
	char *getText() const { return text; }
	~Text() { if(text) delete [] text; }
};

#endif
