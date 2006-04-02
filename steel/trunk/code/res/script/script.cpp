
#include "script.h"
using namespace std;

void ScriptLine::parse()
{
	int s = 0;
	for(;;)
	{
		int n = str.find('\t', s);
		if(n == string::npos)
			break;

		add(str.substr(s, n-s));

		s = n+1;
	}
	add(str.substr(s, str.length()-s));
}

void ScriptLine::add(std::string s)
{
	el_s.push_back(s);
	el_d.push_back(atof(s.c_str()));
}
