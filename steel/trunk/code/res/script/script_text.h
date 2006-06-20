#ifndef RES__SCRIPT_TEXT_H
#define RES__SCRIPT_TEXT_H


#include "script.h"

class ScriptText: public Script
{
public:
	bool init(const std::string name);
};

Script* createScriptText(const std::string filename);

#endif
