#include "utils.h"

using namespace std;


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

vector<string> explode(char delimiter, string s)
{
	vector<string> res;
	string::size_type last, start = 0, len = s.length();
	while(start<=len)
	{
		last = start;
		start = s.find(delimiter, start);

		if(start == string::npos) 
			start = len + 1;
		else
			start++;

		res.push_back(s.substr(last, start - last - 1));
	}
	return res;
}


string implode(char delimiter, vector<string> elements)
{
	if(elements.empty()) return "";

	string res;
	vector<string>::iterator it = elements.begin();
	res = *it;
	it++;
	for(; it != elements.end(); it++)
	{
		res += delimiter;
		res += *it;
	}
	return res;
}
