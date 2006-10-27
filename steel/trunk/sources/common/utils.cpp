/*id*********************************************************
	File: common/utils.cpp
	Unit: utils
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Всевозможные полезные функции

 ************************************************************/

#include "../steel.h"
#include "utils.h"
#include "logger.h"

#include <sstream>

#if STEEL_OS == OS_WIN32
#include <windows.h>
#endif

#include "errno.h"

#ifdef STEEL_USE_GETTEXT
#include <libintl.h>
#endif

using namespace std;

// random float number in interval [0..1)
float frand()
{
	return (float) rand() / (float) RAND_MAX;
}

// random float number in interval [-0.5..+0.5)
float prand()
{
	return (float) rand() / (float) RAND_MAX - 0.5f;
}

// convert string to v3 (vector components are devided with ',' [X,Y,Z])
v3	stov3(string s)
{
	svector<string> v;
	explode(',', s, v);
	v.resize(3);
	v3 r((float)atof(v[0].c_str()), (float)atof(v[1].c_str()), (float)atof(v[2].c_str()));
	return r;
}

// convert float number into string
string FloatToStr(double a)
{
    char s[32];
    sprintf(s,"%2.1f", a);
    return string(s);
}

// convert integer into string
string IntToStr(int a)
{
	std::stringstream res;
	res << a;

	return res.str();
}

// split string into array of strings with delimiter
void explode(char delimiter, const std::string s, svector<string> &res)
{
	string::size_type last, start = 0, len = s.length();
	while(start <= len)
	{
		last = start;
		start = s.find(delimiter, start);

		if(start == string::npos) 
			start = len + 1;
		else
			start++;

		string* T = NULL;

		//int x = errno;
		T = (string*)realloc(T, 32);
		//int y = errno;

		res.push_back(s.substr(last, start - last - 1));
	}
}

// concat array of string into one string
string implode(const char delimiter, const svector<std::string> elements)
{
	if(elements.empty()) return "";

	string res;
	svector<string>::const_iterator it = elements.begin();
	res = *it;
	it++;
	for(; it != elements.end(); it++)
	{
		res += delimiter;
		res += *it;
	}
	return res;
}

// вернуть диреторию, в которой находится файл (выбросить имя файла из полного пути)
std::string getPath(std::string fullpath)
{
	svector<std::string> path;
	explode('/', fullpath, path);
	path.pop_back();

	return  implode('/', path);
}

// split full path to path + filename
void splitPath(std::string fullpath, std::string &path, std::string &filename)
{
	svector<std::string> apath;
	explode('/', fullpath, apath);
	filename = apath.back();
	apath.pop_back();
	path = implode('/', apath);
}

std::string createPath(std::string dir, std::string filename)
{
	if(dir.empty()) return filename;
	else return dir + "/" + filename;
}


// delete file in directory
void deleteFile(std::string dir, std::string file)
{
	if(file.substr(0,1) != ".")
		#if STEEL_OS == OS_WIN32
		DeleteFile( (dir + "\\" + file).c_str());
		#elif STEEL_OS == OS_LINUX
		if ( unlink((dir + "/" + file).c_str())==-1 )
			error("utils io",_("Failed to delete a file ") + dir + "/" + file + ": " + strerror(errno));
		#endif	// STEEL_OS
}


// delete all files in directory with mask
void deleteFiles(string dir, string mask)
{
	#if STEEL_OS == OS_WIN32
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile((dir + "\\" + mask).c_str(), &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE) 
		return;

	deleteFile(dir, FindFileData.cFileName);
    while (FindNextFile(hFind, &FindFileData) != 0) 
		deleteFile(dir, FindFileData.cFileName);
	
	FindClose(hFind);
	
	#elif STEEL_OS == OS_LINUX
	// TODO
	#endif	// STEEL_OS
}

// translate all chars a to b into string s
std::string strtr(const char *s, char a, char b)
{
	std::string res;
	svector<std::string> r;
	explode(a, s, r);
	for(unsigned int i = 0; i < r.size(); i++)
	{
		if(!r[i].empty())
			res += "\n\t\t" + IntToStr(i) + ": " + r[i];
	}
	return res;
}

string escape(string base, char escapedChar)
{
	string res;
	for(string::iterator i = base.begin(); i != base.end(); i++)
	{
		if(*i == escapedChar) res += '\\';
		if(*i == '\\') res += '\\';
		res += *i;
	}
	return res;
}
