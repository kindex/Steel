/*id*********************************************************
	File: test/test_main.cpp
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		запускаемы файл тестовй системы steel engine.
 ************************************************************/

#include "../steel.h"
#if TESTING == 1

#include <string>
#include "../common/timer.h"
#include "../common/utils.h"
#include "../common/logger.h"
#include "../common/system_info.h"
#include "../common/win_info.h"

#include "tester.h"

#include "../res/test_res/test_res.h"
#include "../common/test_common/test_svector.h"

std::string commandLine;

#if (STEEL_COMPILER == COMPILER_GCC) && (STEEL_OS == OS_WIN32)
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
// *********************** LOG *********************
	globalTimer.start();

	deleteFiles("..\\log", "*.log");
	steel::log.open("../steel.log");
	log_msg("core", "Command Line: '" + commandLine + "'");
	
	CollectSystemInfo();

	#if STEEL_OS == OS_WIN32
		DetectWindowsVersion();
	#endif

	TEST(Res);
	TEST(svector);

	testInfo.report();

	steel::log.close();
	return testInfo.errorCount;
}

#if STEEL_OS == OS_WIN32
#include <windows.h>
#include <direct.h>
#endif	// STEEL_OS

#include <fstream>
#include <string>

#if STEEL_OS == OS_WIN32

#if STEEL_COMPILER == COMPILER_GCC
HINSTANCE hInstance;

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	::hInstance = hInstance;

	commandLine = lpCmdLine;

	return main1(0, NULL);							// Exit The Program
}

#elif (STEEL_COMPILER == COMPILER_VS8)
HINSTANCE hInstance;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	::hInstance = hInstance;

	return main(0, NULL);
}
#endif	// STEEL_COMPILER

#endif	// STEEL_OS

#endif	// TESTING
