/*id*********************************************************
	File: common/utils.h
	Unit: utils
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
v3 prand_v3();
v3 frand_v3();

// convert string to v3 (vector components are devided with ',' [X,Y,Z])
v3	stov3(const std::string& s);
// convert float number into string
std::string FloatToStr(double a);
// convert integer into string
std::string IntToStr(int a);

// split string into array of strings with delimiter
void explode(const char delimiter, const std::string& s, OUT svector<std::string>& res);
// concat array of string into one string
std::string implode(const char delimiter, const svector<std::string>& elements);

// вернуть диреторию, в которой находится файл (выбросить имя файла из полного пути)
std::string getPath(std::string fullpath);
// split full path to path + filename
void splitPath(const std::string& fullpath, OUT std::string& path, OUT std::string& filename);
std::string getFileName(const std::string& fullpath);
std::string createPath(const std::string& dir, const std::string& filename);

// delete file in directory
void deleteFiles(const std::string& dir, const std::string& mask);
// delete all files in directory with mask
void deleteFile(const std::string& dir, const std::string& file);
// translate all chars a to b into string s
std::string strtr(const std::string& s, const char a, const char b);

std::string strLineEnum(const char *s, char a, char b);

// escape escapedChar and \ in base string. (Ex: escape(a'b\c, ') => a\'b\\c)
std::string escape(const std::string& base, char escapedChar);

std::string trim(const std::string& string);

#endif
