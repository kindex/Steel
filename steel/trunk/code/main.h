/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    Licence:
        “олько дл€ Division
    Description:
		main() создайт игровые классы, коллеццию ресурсов, загружет 
		плагины, содержит главный цикл игры.
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
