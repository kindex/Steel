/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		main() ������� ������� ������, ��������� ��������, �������� 
		�������, �������� ������� ���� ����.
 ************************************************************/

#ifndef __MAIN_H
#define __MAIN_H

#include "_cpp.h"
#include <string>

#ifdef STEEL_COMPILER_DEVCPP
int main1(int argc, char *argv[]);
#else
int main(int argc, char *argv[]);
#endif

extern std::string commandLine;

#endif
