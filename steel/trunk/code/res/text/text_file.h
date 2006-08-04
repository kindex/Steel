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
