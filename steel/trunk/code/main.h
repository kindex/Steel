/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    Licence:
        ������ ��� Division
    Description:
		main() ������� ������� ������, ��������� ��������, �������� 
		�������, �������� ������� ���� ����.
 ************************************************************/

#ifndef __MAIN_H
#define __MAIN_H

#include "_cpp.h"
#include <string>

#ifdef COMPILER_DEVCPP
int main1(int argc, char *argv[]);
#else
int main(int argc, char *argv[]);
#endif

extern std::string commandLine;

#endif
