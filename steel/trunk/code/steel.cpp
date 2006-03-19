/*
 System dependant entry point
 */

#include <windows.h>		// Header File For Windows
#include <direct.h>
#include <fstream>
#include <string>

using namespace std;
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