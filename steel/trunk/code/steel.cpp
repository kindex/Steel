/*id*********************************************************
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

#include <windows.h>
#include <direct.h>
#include <fstream>
#include <string>

#include "main.h"
#include "_cpp.h"


//#define OPENGL_WIN


/*extern int mainCRTStartup(void);
*/

#ifdef STEEL_COMPILER_DEVCPP
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
#endif

#ifdef STEEL_COMPILER_VS8
#ifdef STEEL_OPENGL_WIN
HINSTANCE hInstance;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	::hInstance = hInstance;

	return main(0, NULL);
}
#endif
#endif
