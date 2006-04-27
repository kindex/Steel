#ifndef RES__SCRIPT_H
#define RES__SCRIPT_H

#include "../res.h"
#include <string>
#include <vector>

class ScriptLine
{
protected:
	std::string str;
	std::vector<std::string> el_s; // element string
	std::vector<double> el_d; // element double
public:
	std::string gets(unsigned int n) { return el_s[n];}
	double getd(unsigned int n) { return el_d[n];}
	
	float getf(unsigned int n, float defaults = 0.0f) 
	{ 
		if(n<el_d.size())
			return (float)el_d[n];
		else
			return defaults;
	}

	std::string getstr() { return str; }

	int count() { return el_s.size(); }

	void set(std::string const _str) { str = _str; parse(); }
	void parse(); // convert from str to array of string element
	void add(std::string s);
};

/*
Массив строчек. Каждая строка - значения, разделённые \t
*/

class Script: public Res
{
protected:
	std::vector<ScriptLine> line;
public:
	int count() { return line.size(); }
	int count(unsigned int n) { return line[n].count(); }

	std::string gets(unsigned int n, unsigned int m) { return line[n].gets(m); } 
	double	getd(unsigned int n, unsigned int m) { return line[n].getd(m); } 
	float	getf(unsigned int n, unsigned int m, float defaults = 0.0f) { return line[n].getf(m, defaults); } 

	bool unload() { return true; } 
};

#endif
