#ifndef __UTILS_H
#define __UTILS_H

#include "../_cpp.h"

#include <string>
#include <vector>

std::string FloatToStr(double a);
std::string IntToStr(int a);

// split string into vector
std::vector<std::string> explode(char delimiter, std::string s);
// unsplit vector to string
std::string implode(char delimiter, std::vector<std::string> elements);


#endif
