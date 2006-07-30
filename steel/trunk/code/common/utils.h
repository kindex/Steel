#ifndef __UTILS_H
#define __UTILS_H

#include "../_cpp.h"
#include "../math/vector3d.h"
#include <string>
#include "steel_vector.h"

//Returns random value [0..1)
float frand();
//Returns random value [-0.5..+0.5)
float prand();

// string to v3, string format X,Y,Z
v3	stov3(std::string s);

std::string FloatToStr(double a);
std::string IntToStr(int a);

// split string into vector
steel::vector<std::string> explode(char delimiter, const std::string s);
// unsplit vector to string
std::string implode(const char delimiter, const steel::vector<std::string> elements);

std::string getPath(std::string fullpath);

void splitPath(std::string fullpath, std::string &path, std::string &filename);

void deleteFiles(std::string dir, std::string mask);
void deleteFile(std::string dir, std::string file);

// Change all a symbols in string s to symbol b
std::string strtr(const char *s, char a, char b);


#endif
