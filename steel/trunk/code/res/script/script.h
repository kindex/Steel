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
	std::vector<v3> el_v3; // element v3
public:
	std::string gets(unsigned int n, std::string defaults = "");
	double	getd(unsigned int n) { return el_d[n];}
	float	getf(unsigned int n, float defaults = 0.0f);
	v3		getv3(unsigned int n, v3 defaults = v3(0.0f, 0.0f, 0.0f));

	// ������� ��� ������
	std::string getstr() { return str; }
	// ������� ��� ������ ������� � n-���� ��������
//	std::string getstr(int n) { return el_s.g; }

	int count() { return el_s.size(); }

	void set(std::string const _str) { str = _str; parse(); }
	void parse(); // convert from str to array of string element
	void add(std::string s);
};

/*
������ �������. ������ ������ - ��������, ���������� \t
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
	ScriptLine &get(unsigned int n) { return line[n]; }
};

#endif
