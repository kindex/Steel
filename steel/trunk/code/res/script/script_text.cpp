#include "script_text.h"
using namespace std;

Res* createScriptText(const std::string filename)
{
	ScriptText *o = new ScriptText;
	if(o->init(filename)) 
		return o;
	else
		return NULL;
}


bool ScriptText::init(const string name)
{
	std::string file = name;

	rstream f; // no binary acces, only text

	if(!f.open(file)) 
	{
		f.close();
		file = name + ".script";
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

