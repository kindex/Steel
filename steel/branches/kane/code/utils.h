#ifndef __UTILS_H
#define __UTILS_H

#include <string>
#include <iostream>
//using namespace std;

std::string FloatToStr(double a);
std::string IntToStr(int a);

#define _ std::cout << "DEBUG " << __FUNCTION__ << ":" << __LINE__ << std::endl;

#endif
