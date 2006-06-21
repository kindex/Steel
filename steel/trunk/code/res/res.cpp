/*id*********************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        Молуль для хранения, загрузки игровых ресурсов и 
		контроля над ними. 
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

//TEMP
#define bufsize 1024
char buf[bufsize];

void rstream::skip(int n)
{
	while(n>0)
	{
		int m =n;
		if(m > bufsize) m = bufsize;
		read(buf, m);
		n -= m;
	}
}

/*rstream::rstream(string s)
{
	f = fopen(s.c_str(), "rb");
	if(f)
		ok = true;
	else
		ok = false;
}*/

void rstream::read(void *dest, int size)
{
	std::ifstream::read((char*)dest, size);
}

bool rstream::open(std::string s, std::string ext, ios_base::openmode _Mode) 
{ 
	std::string r = std::string("../res/") + s + (ext == ""?"":"." + ext);

	std::ifstream::open(r.c_str(), _Mode | std::ios::in);
	if(fail())
	{
		std::ifstream::open((std::string("../res/") + s).c_str(), _Mode | std::ios::in);
		if(fail())
			return false;
	}
	return true;
}

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

std::string getFullPath(std::string filename, std::string directory)
{
	if((!filename.empty() && filename[0] == '/') || directory.empty())
		return filename;
	else
		return directory + "/" + filename;
}
