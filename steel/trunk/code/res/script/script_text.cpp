#include "script_text.h"

using namespace std;

bool ScriptText::init(const string name, ResCollection &res)
{
	std::string file = "../res/" + name + ".script";

	rstream f(file); // no binary acces, only text
	if(!f.good()) 
	{
		alog.msg("error res script", string("Res/Material/Script: cannot open file ") + file.c_str());
		return false;
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

