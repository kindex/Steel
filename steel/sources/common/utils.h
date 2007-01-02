/*id*********************************************************
	File: common/utils.h
	Unit: utils
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Всевозможные полезные функции
	TODO
		deleteFiles
			Написать реализацию под Линукс [Kane]
 ************************************************************/
#ifndef __UTILS_H
#define __UTILS_H

#include "../steel.h"
#include "../math/vector3d.h"
#include <string>
#include "svector.h"

// random float number in interval [0..1)
float frand();
// random float number in interval [-0.5..+0.5)
float prand();
// convert string to v3 (vector components are devided with ',' [X,Y,Z])
v3	stov3(std::string s);
// convert float number into string
std::string FloatToStr(double a);
// convert integer into string
std::string IntToStr(int a);

// split string into array of strings with delimiter
void explode(IN const char delimiter, IN const std::string& s, OUT svector<std::string>& res);
// concat array of string into one string
std::string implode(const char delimiter, const svector<std::string>& elements);

// вернуть диреторию, в которой находится файл (выбросить имя файла из полного пути)
std::string getPath(std::string fullpath);
// split full path to path + filename
void splitPath(std::string fullpath, std::string &path, std::string &filename);
std::string createPath(const std::string& dir, const std::string& filename);

// delete file in directory
void deleteFiles(std::string dir, std::string mask);
// delete all files in directory with mask
void deleteFile(std::string dir, std::string file);
// translate all chars a to b into string s
std::string strtr(const char *s, char a, char b);
// escape escapedChar and \ in base string. (Ex: escape(a'b\c, ') => a\'b\\c)
std::string escape(std::string base, char escapedChar);

std::string trim(std::string string);

#endif
