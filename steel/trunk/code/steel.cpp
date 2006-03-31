/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
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

#ifdef COMPILER_DEVCPP
HINSTANCE hInstance;

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	::hInstance = hInstance;

	return main1(0, NULL);							// Exit The Program
}
#endif
