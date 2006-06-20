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
