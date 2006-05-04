
#include "script.h"
#include "../../common/utils.h"
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
	el_v3.push_back(stov3(s));
}

std::string ScriptLine::gets(unsigned int n, std::string defaults)
{ 
	if(n<el_s.size())
		return el_s[n];
	else
		return defaults;
}

float ScriptLine::getf(unsigned int n, float defaults)
{ 
	if(n<el_d.size())
		return (float)el_d[n];
	else
		return defaults;
}

v3	ScriptLine::getv3(unsigned int n, v3 defaults)
{ 
	if(n<el_d.size())
		return el_v3[n];
	else
		return defaults;
}
