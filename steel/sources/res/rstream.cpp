/*id*******************************************************************************
	File: res/rstream.cpp
	Unit: rstream
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Класс и интерфейсом ifstream для загрузки игровых ресурсов
**************************************************************************************/

#include "../steel.h"
#include "rstream.h"
#include "../common/logger.h"
#include "../common/containers/string_vector.h"

#include <fstream>
#include <string>

using namespace std;

//TEMP
#define bufsize 1024
char buf[bufsize];

StringVector dataSearchDirectories;

void addDataSearchDirectories(const std::string& newDataDir)
{
	dataSearchDirectories.push_back(newDataDir);
}

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

void rstream::read(void* dest, int size)
{
	std::ifstream::read((char*)dest, size);
}

static std::string getResFileName(const std::string& s, const std::string& ext, ios_base::openmode _Mode)
{
	for EACH_CONST(StringVector, dataSearchDirectories, dir)
	{
		ifstream f1, f2;

		string name = std::string(*dir) + s + (ext.empty() ? "" : "." + ext);

		f1.open(name.c_str(), _Mode | std::ios::in);
		if (!f1.fail())
		{
			return name;
		}
		else
		{
			name = std::string(*dir) + s;
			f2.open(name.c_str(), _Mode | std::ios::in);
			if (!f2.fail()) 
			{
				return name;
			}
		}
	}
	return "";
}

bool rstream::open(const std::string& s, const std::string& ext, ios_base::openmode _Mode)
{
	std::string name = getResFileName(s, ext, _Mode);
	std::ifstream::open(name.c_str(), _Mode | std::ios::in);

	failed = false;

	if(fail())
	{
		failed = true;
		return false;
	}
	else return true;
}
