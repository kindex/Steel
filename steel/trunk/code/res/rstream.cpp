#include "rstream.h"
#include "../common/logger.h"

#include <fstream>
#include <string>

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
