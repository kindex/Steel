/*id*********************************************************
	File: res/script/script_text.cpp
	Unit: res/script
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для загрузки таблицы настроек из текстового файла. 
		Значения отделяются табуляуцей.
	TODO
		change logic or name
 ************************************************************/

#include "script_text.h"
#include "../rstream.h"

using namespace std;

bool ScriptText::init(const string name, const std::string dir)
{
	std::string file = (dir.empty()?"":dir+"/") + name;

	rstream f; // no binary acces, only text

	if(!f.open(file)) 
	{
		f.close();
		file = (dir.empty()?"":dir+"/") + name + ".script";
		if(!f.open(file)) 
		{
			log_msg("error res script", string("Res/Material/Script: cannot open file ") + file.c_str());
			return false;
		}
	}

	while(f.good())
	{
		char buf[1024];
		f.getline(buf, 1023);
		std::string s = buf;
		if(s.length()<=2) continue;
		
        if(s[s.length()-1] == '\r') s.resize(s.length()-1);

		if(s.substr(0,2) == "//" || s[0] == ';' || s[0] == '#') continue;

		ScriptLine l;
		l.set(s);
		line.push_back(l);
	}

	return true;


}

