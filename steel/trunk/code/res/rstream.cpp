#include "rstream.h"
#include "../common/logger.h"

#include <fstream>
#include <string>

using namespace std;

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
	ifstream f1, f2;

	string name = std::string("../data/") + s + (ext == ""?"":"." + ext);

	f1.open(name.c_str(), _Mode | std::ios::in);
	if(f1.fail())
	{
		name = std::string("../data/") + s;
		f2.open(name.c_str(), _Mode | std::ios::in);
		if(f2.fail()) 
		{
			failed = true;
			return false;
		}
	}

	std::ifstream::open(name.c_str(), _Mode | std::ios::in);

	failed = false;
	return true;
}
