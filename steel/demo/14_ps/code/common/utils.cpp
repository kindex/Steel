#include "utils.h"
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

steel::vector<string> explode(char delimiter, string s)
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

		int x = errno;
		T = (string*)realloc(T, 32);
		int y = errno;

		res.push_back(s.substr(last, start - last - 1));
	}
	return res;
}


string implode(char delimiter, steel::vector<string> elements)
{
	if(elements.empty()) return "";

	string res;
	steel::vector<string>::iterator it = elements.begin();
	res = *it;
	it++;
	for(; it != elements.end(); it++)
	{
		res += delimiter;
		res += *it;
	}
	return res;
}
