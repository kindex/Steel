/*id*********************************************************
	File: res/res.cpp
	Unit: res
	Part of: Steel engine
	(C) DiVision, 2003-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Молуль для хранения, загрузки игровых ресурсов и 
		контроля над ними. 
************************************************************/

#include "../steel.h"
#include "res.h"
#include "../common/logger.h"

ResStack resStack;

std::string getext(std::string name)
{
	std::string r;
	for(int i=name.length()-1; i>=0; i--)
		if(name[i]!='.')
			r = name[i] + r;
		else
			break;
	return r;
}

bool ResStack::push(std::string directory)
{
	if(level>100) abort_init("res error", "To many recursive res::add executions (>100)");

	dirs.push_back(directory);
	level++;
	steel::log.push();

	return true;
}

bool ResStack::pop(void)
{
	if(level==0) 
		abort_init("res error", "Stack underflow");
	level--;
	dirs.pop_back();
	steel::log.pop();
	return true;
}

bool ResStack::pushFullPath(std::string path)
{
	std::string baseDirectory;		
	splitPath(path, baseDirectory, path);
	return push(baseDirectory);
}


std::string ResStack::getFullName(const std::string &name)
{
	std::string t = top();
	return getFullPath(name, t);
}


std::string ResStack::top(void)
{
	std::string result;
	if(level > 0)
		result = dirs.back();

	return result;
}


int ResStack::getLevel(void)
{
	return level;
}

// TODO: обрабатывать /../ и /./
std::string getFullPath(const std::string &filename, const std::string &directory)
{
	if((!filename.empty() && filename[0] == '/'))
	{
		std::string result = filename;
		result.erase(0,1);
		return result;
	}
	else if(directory.empty())
		return filename;
	else
		return directory + "/" + filename;
}
