#ifndef RES__SCRIPT_TEXT_H
#define RES__SCRIPT_TEXT_H


#include "script.h"

class ScriptText: public Script
{
public:
	bool init(const std::string name, ResCollection &res);
};

Res* createScriptText(const std::string filename, ResCollection *res);

#endif
