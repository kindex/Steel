
#include "../steel.h"
#include "utils.h"

#if STEEL_OS == OS_WIN32
#include <windows.h>
#endif

#include "errno.h"

#ifdef STEEL_USE_GETTEXT
#include <libintl.h>
#endif

using namespace std;

float frand()
{
	return (float) rand() / (float) RAND_MAX;
}

float prand()
{
	return (float) rand() / (float) RAND_MAX - 0.5f;
}


v3	stov3(string s)
{
	steel::vector<string> v = explode(',', s);
	v.resize(3);
	v3 r((float)atof(v[0].c_str()), (float)atof(v[1].c_str()), (float)atof(v[2].c_str()));
	return r;
}

string FloatToStr(double a)
{
    char s[32];
    sprintf(s,"%2.1f", a);
    return string(s);
}

string IntToStr(int a)
{
    char s[32];

    sprintf(s,"%i", a);
    return string(s);
}

steel::vector<string> explode(char delimiter, const std::string s)
{
	steel::vector<string> res;
	string::size_type last, start = 0, len = s.length();
	while(start<=len)
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
	return res;
}


string implode(const char delimiter, const steel::vector<std::string> elements)
{
	if(elements.empty()) return "";

	string res;
	steel::vector<string>::const_iterator it = elements.begin();
	res = *it;
	it++;
	for(; it != elements.end(); it++)
	{
		res += delimiter;
		res += *it;
	}
	return res;
}

std::string getPath(std::string fullpath)
{
	steel::vector<std::string> path = explode('/', fullpath);
	path.pop_back();

	return  implode('/', path);
}

void splitPath(std::string fullpath, std::string &path, std::string &filename)
{
	steel::vector<std::string> apath = explode('/', fullpath);
	filename = apath.back();
	apath.pop_back();
	path = implode('/', apath);
}

void deleteFile(std::string dir, std::string file)
{
	if(file.substr(0,1) != ".")
		#if STEEL_OS == OS_WIN32
		DeleteFile( (dir + "\\" + file).c_str());
		#elif STEEL_OS == OS_LINUX
		if ( unlink((dir + "/" + file).c_str())==-1 )
			#ifdef STEEL_USE_GETTEXT
			error(_("Failed to delete a file ") + dir + "/" + file,strerror(errno));
			#else	// STEEL_USE_GETTEXT
			error("Failed to delete a file " + dir + "/" + file,strerror(errno));
			#endif	// STEEL_USE_GETTEXT
		#endif	// STEEL_OS
}

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

std::string strtr(const char *s, char a, char b)
{
	std::string res;
	steel::vector<std::string> r = explode(a, s);
	for(unsigned int i = 0; i < r.size(); i++)
	{
		if(!r[i].empty())
			res += "\n\t\t" + IntToStr(i) + ": " + r[i];
	}
	return res;
}
