/*id*********************************************************
	File: common/utils.cpp
	Unit: utils
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
#define NOMINMAX
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

// random float number in interval [-0.5..+0.5)
size_t irand(size_t count)
{
	return rand() % count;
}

v3 prand_v3()
{
    return v3(prand(), prand(), prand());
}

v3 frand_v3()
{
    return v3(frand(), frand(), frand());
}


// convert string to v3 (vector components are devided with ',' [X,Y,Z])
v3 stov3(const std::string& s)
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
void explode(IN const char delimiter, const std::string& s, OUT svector<std::string>& res)
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

		res.push_back(s.substr(last, start - last - 1));
	}
}

// concat array of string into one string
std::string implode(const char delimiter, const svector<std::string>& elements)
{
	if (elements.empty())
	{
		return "";
	}

	std::string res;
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
std::string getPath(const std::string& fullpath)
{
	svector<std::string> path;
	explode('/', fullpath, path);
	path.pop_back();

	return  implode('/', path);
}

// split full path to path + filename
void splitPath(const std::string& fullpath, OUT std::string& path, OUT std::string& filename)
{
	svector<std::string> apath;
	explode('/', fullpath, apath);
	filename = apath.back();
	apath.pop_back();
	path = implode('/', apath);
}

std::string getFileName(const std::string& fullpath)
{
	std::string path;
	std::string filename;
	splitPath(strtr(fullpath, '\\', '/'), path, filename);
	return filename;
}

std::string createPath(const std::string& dir, const std::string& filename)
{
	if(!filename.empty() && filename[0] == '/')
		return filename;
	else if(dir.empty()) return filename;
	else return dir + "/" + filename;
}


// delete file in directory
void deleteFile(const std::string& dir, const std::string& file)
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
void deleteFiles(const std::string& dir, const std::string& mask)
{
	#if STEEL_OS == OS_WIN32
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile((dir + "\\" + mask).c_str(), &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE) 
		return;

	deleteFile(dir, FindFileData.cFileName);
    while (FindNextFile(hFind, &FindFileData) != 0)
	{
		deleteFile(dir, FindFileData.cFileName);
	}
	
	FindClose(hFind);
	
	#elif STEEL_OS == OS_LINUX
	// TODO
	#endif	// STEEL_OS
}

// translate all chars a to b into string s
std::string strtr(IN const std::string& s, IN const char a, IN const char b)
{
	std::string res = s;
	size_t size = res.size();

	for (unsigned int i = 0; i < size; i++)
	{
		if(res[i] == a)
		{
			res[i] = b;
		}
	}
	return res;
}

std::string strLineEnum(const char *s, char a, char b)
{
	std::string res;
	svector<std::string> r;
	explode(a, s, r);
	for(unsigned int i = 0; i < r.size(); i++)
	{
		if(!r[i].empty())
		{
			res += "\t" + IntToStr(i) + ": " + r[i];
		}
	}
	return res;
}

string escape(const std::string& base, char escapedChar)
{
	string res;
	for EACH_CONST(std::string, base, i)
	{
		if (*i == escapedChar) res += '\\';
		if (*i == '\\') res += '\\';
		res += *i;
	}
	return res;
}

std::string trim(const std::string& string)
{
	std::string result = string;
	int i = result.find_first_not_of(" ");
	result.erase(0, i);
	i = result.find_last_not_of(" ");
	result.erase(i+1, result.npos);

	return result;
}
