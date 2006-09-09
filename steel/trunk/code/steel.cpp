/*id*********************************************************
	File: steel.cpp
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		System dependant entry point for Visual Studio project.
 ************************************************************/

#include "steel.h"

#if STEEL_OS == OS_WIN32
#include <windows.h>
#include <direct.h>
#endif	// STEEL_OS

#include <fstream>
#include <string>

#include "main.h"


/*extern int mainCRTStartup(void);
*/

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
