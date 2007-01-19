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
#include "config_parser.h"

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
	Config* newValue = parse(token[1]);
	if (newValue == NULL)
	{
		log_msg("script", "cannot parse '" + token[1] + "'");
		return false;
	}
	else
	{
		setup(conf, token[0], newValue);
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

void setup(PConfig& conf, const std::string& path, Config* newValue)
{
	// TODO: cleanup old config
	if (path.empty() || path == ".")
	{
		conf = newValue;
	}
	else
	{
/*		site_t a = path.find_first_of(".[");
		if (a != path.npos)
		{
			if (a == 0 && path[a] == '[') ...
			if (path[a] == '.') // struct
		}TODO: split path*/
		
		if (conf->getType() == CONFIG_VALUE_STRUCT)
		{
			static_cast<ConfigStruct*>(conf)->setValue(path, newValue);
		}
	}
}
