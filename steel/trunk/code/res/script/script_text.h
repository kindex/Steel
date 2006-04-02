#ifndef RES__SCRIPT_TEXT_H
#define RES__SCRIPT_TEXT_H


#include "script.h"

class ScriptText: public Script
{
	bool init(const std::string name, ResCollection &res);
};

#endif
