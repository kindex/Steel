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


//#define OPENGL_WIN

HINSTANCE hInstance;

/*extern int mainCRTStartup(void);

/*int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	::hInstance = hInstance;

	return main(0, NULL);							// Exit The Program
}
*/