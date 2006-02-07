/*
 System dependant entry point
 */

#include <windows.h>		// Header File For Windows
#include <direct.h>
#include <fstream>
#include <string>

using namespace std;
#include "main.h"


HINSTANCE hInstance;

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	::hInstance = hInstance;

	/*ifstream fin("root.cfg");
	string root; // Resource Root directory
	fin >> root;
	chdir(root.c_str());*/

	return main();							// Exit The Program
}
