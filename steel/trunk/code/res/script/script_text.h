#ifndef RES__SCRIPT_TEXT_H
#define RES__SCRIPT_TEXT_H


#include "script.h"

class ScriptText: public Script
{
public:
	bool init(const std::string name);
};

Res* createScriptText(const std::string filename);

#endif
