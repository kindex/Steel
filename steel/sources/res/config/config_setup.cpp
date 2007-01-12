/*id*********************************************************
	File: res/config/config.h
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс для хранения настроек
 ************************************************************/
#include "../../steel.h"

#include "config_setup.h"

using namespace std;

bool executeCommand(Config* conf, std::string command)
{
	if(command.empty()) return true;
	log_msg("console", "ExecCommand: '" + command + "'");

	// var=value

	svector<string> token;
	explode('=', command, token);
	if(token.size() != 2) return false;

	token[0] = trim(token[0]);
	token[1] = trim(token[1]);
	Config* oldValue = conf->find(token[0]);
	if (oldValue == NULL)
	{
		log_msg("script", "'" + token[0] + "' not found");
		return false;
	}
	else
	{
		oldValue->setValues(token[0], token[1]);
		return true;
	}
}


bool executeScript(Config* conf, std::string script)
{
	log_msg("console", "ExecScript: '" + script + "'");

	svector<string> lines;
	explode(';', script, lines);
	for(svector<string>::const_iterator it = lines.begin(); it != lines.end(); it++)
	{
		if(!executeCommand(conf, *it))
		{
			return false;
		}
	}
	return true;
}
