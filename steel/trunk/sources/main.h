/*id*********************************************************
	File: main.h
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		main() создает игровые классы, коллекцию ресурсов, загружает 
		плагины, содержит главный цикл игры.
 ************************************************************/

#ifndef __MAIN_H
#define __MAIN_H
#ifndef LIB_STEEL


#include <string>

#if (STEEL_COMPILER == COMPILER_GCC) && (STEEL_OS == OS_WIN32)
int main1(int argc, char *argv[]);
#else
int main(int argc, char *argv[]);
#endif

bool test(void);

extern std::string commandLine;

#endif
#endif
