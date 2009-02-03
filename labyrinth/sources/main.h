/*id*********************************************************
	Unit: core
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		main() создайт игровые классы, коллеццию ресурсов, загружет 
		плагины, содержит главный цикл игры.
 ************************************************************/

#ifndef _DEMO_MAIN_H_
#define _DEMO_MAIN_H_

#include <steel.h>
#include <string>

#if (STEEL_OS == WIN32) && (STEEL_COMPILER == GCC)
int main1(int argc, char *argv[]);
extern std::string commandLine;
#else
int main(int argc, char *argv[]);
#endif

bool test();

#endif
