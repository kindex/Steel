#ifndef RES__SCRIPT_H
#define RES__SCRIPT_H

#include "../res.h"
#include <string>

class ScriptLine
{
protected:
	std::string str;
	steel::vector<std::string> el_s; // element string
	steel::vector<double> el_d; // element double
	steel::vector<v3> el_v3; // element v3
public:
	std::string gets(unsigned int n, std::string defaults = "");
	double	getd(unsigned int n) { return el_d[n];}
	float	getf(unsigned int n, float defaults = 0.0f);
	v3		getv3(unsigned int n, v3 defaults = v3(0.0f, 0.0f, 0.0f));

	// вернуть всю строку
	std::string getstr() { return str; }
	// вернуть всю строку начиная с n-того элемента
//	std::string getstr(int n) { return el_s.g; }

	int count() { return (int)el_s.size(); }

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
	steel::vector<ScriptLine> line;
public:
	int count() { return (int)line.size(); }
	int count(unsigned int n) { return line[n].count(); }

	std::string gets(unsigned int n, unsigned int m) { return line[n].gets(m); } 
	double	getd(unsigned int n, unsigned int m) { return line[n].getd(m); } 
	float	getf(unsigned int n, unsigned int m, float defaults = 0.0f) { return line[n].getf(m, defaults); } 

	bool unload() { return true; } 
	ScriptLine &get(unsigned int n) { return line[n]; }
};

#endif
