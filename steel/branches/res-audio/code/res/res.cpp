/*id*********************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        ������ ��� ��������, �������� ������� �������� � 
		�������� ��� ����. 
************************************************************/

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





/*bool Res::init(string& name)
{
	rstream f("../res/"+name);

	if(!f.good()) return false;
	if(!this->load(f, 0)) return false;
	return !f.bad();
}*/


bool ResStack::push(std::string directory)
{
	if(level>100) abort_init("res error", "To many recursive res::add executions (>100)");

	stack.push(directory);
	level++;
	steel::log.push();

	return true;
}

bool ResStack::pop(void)
{
	if(level==0) 
		abort_init("res error", "Stack underflow");
	level--;
	stack.pop();
	steel::log.pop();
	return true;
}

std::string ResStack::top(void)
{
	if(level==0)
		return std::string("");
	else
		return stack.top();
}


int ResStack::getLevel(void)
{
	return level;
}

// TODO: ������������ /../ � /./
std::string getFullPath(std::string filename, std::string directory)
{
	if((!filename.empty() && filename[0] == '/'))
	{
		filename.erase(0,1);
		return filename;
	}
	else if(directory.empty())
		return filename;
	else
		return directory + "/" + filename;
}