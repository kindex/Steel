/*id*********************************************************
	Unit: System Info
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
        Steel Engine License
    Description:
		Модуль определения необходимых параметров системы.
 ************************************************************/

#include "../steel.h"

#if STEEL_OS == OS_WIN32
#include <windows.h>
#include <winbase.h>

#include "system_info.h"
#include "logger.h"
#include "utils.h"

#define BUFSIZE 1024

LONG RegQueryValueEx(
	HKEY hKey, 			// handle to key to query
	LPTSTR lpValueName, 	// address of name of value to query
	LPDWORD lpReserved, 	// reserved
	LPDWORD lpType, 		// address of buffer for value type
	LPBYTE lpData, 		// address of data buffer
	LPDWORD lpcbData 		// address of data buffer size
);

using namespace std;

/*
 double CPUSpeed(void) 
  { 
    WORD dwTimerHi, dwTimerLo; 
    asm
     { 
        DW 0x310F 
        mov dwTimerLo, EAX 
        mov dwTimerHi, EDX 
      } 
    Sleep (500); 
    asm
     { 
        DW 0x310F 
        sub EAX, dwTimerLo 
        sub EDX, dwTimerHi 
        mov dwTimerLo, EAX 
        mov dwTimerHi, EDX 
     } 
    return dwTimerLo/(1000.0*500); 
  }
*/

void CollectSystemInfo(void)
{   
	char szThis[300];
	int pathLenght = GetModuleFileName(NULL, szThis, 300); // http://www.sources.ru/msdn/library/getmodulefilename.shtml
	string fullPathForCurrentApplication;
	for (int i=0; i < pathLenght; i++)
	{
		fullPathForCurrentApplication += szThis[i];
	}
//	cout << "Full Path "  << fullPathForCurrentApplication << endl << endl;
	log_msg("system_info",  "Full Path '" + fullPathForCurrentApplication + "'");

	int physicalMemoryInstelled;
	int physicalMemoryAvailable;
	int memoryUsed;
	int totalVirtualMemory;
	
	MEMORYSTATUS memoryStatus;
	
	memset(&memoryStatus, sizeof (MEMORYSTATUS), 0);
	memoryStatus.dwLength = sizeof (MEMORYSTATUS);

	GlobalMemoryStatus (&memoryStatus);
	physicalMemoryInstelled = memoryStatus.dwTotalPhys / 1024;
	physicalMemoryAvailable = memoryStatus.dwAvailPhys / 1024;
	memoryUsed = physicalMemoryInstelled - physicalMemoryAvailable;
	totalVirtualMemory = memoryStatus.dwTotalVirtual / 1024;

	//cout << "physicalMemoryInstelled: " << physicalMemoryInstelled << endl;
	log_msg("system_info",  "physical Memory Instelled: " + IntToStr(physicalMemoryInstelled));
	//cout << "physicalMemoryAvailable: " << physicalMemoryAvailable << endl;
	log_msg("system_info",  "physical Memory Available: " + IntToStr(physicalMemoryAvailable));
	//cout << "memoryUsed: "<< memoryUsed << endl;
	log_msg("system_info",  "total Memory Used: " + IntToStr(memoryUsed));
	//cout << "totalMemoryWithVirtual: " << totalVirtualMemory << endl;
	log_msg("system_info",  "total Memory Virtual: " + IntToStr(totalVirtualMemory));
    //cout << endl;

	SYSTEM_INFO siSysInfo;

   // Копируем информацию о железе в структуру SYSTEM_INFO.

	GetSystemInfo(&siSysInfo);

   // Отображаем содержимое структуры SYSTEM_INFO.
//   cout << "  OEM ID: " << siSysInfo.dwOemId << endl;
	log_msg("system_info", "OEM ID: " + IntToStr(siSysInfo.dwOemId));

//   cout << "  Number of processors: " << siSysInfo.dwNumberOfProcessors << endl;
	log_msg("system_info", "Number of processors: " + IntToStr(siSysInfo.dwNumberOfProcessors));

//   cout << "  Page size:  " << siSysInfo.dwPageSize << endl;  
	log_msg("system_info", "Page size:  " + IntToStr(siSysInfo.dwPageSize));

//   cout << "  Processor type: " << siSysInfo.dwProcessorType << endl;
	log_msg("system_info", "Processor type: " + IntToStr(siSysInfo.dwProcessorType));

//   cout << "  Minimum application address: 0x" << siSysInfo.lpMinimumApplicationAddress << endl;
//  log_msg("system_info",  "  Minimum application address: 0x" + IntToStr(siSysInfo.lpMinimumApplicationAddress));

//   cout << "  Maximum application address: 0x" << siSysInfo.lpMaximumApplicationAddress << endl;
// log_msg("system_info",  "  Maximum application address: 0x" + IntToStr(siSysInfo.lpMaximumApplicationAddress));
  
   LPTSTR lpszSystemInfo1;      // указатель на строку, в которой
                               // будет информация о системе.
   LPTSTR lpszSystemInfo2;
   LPTSTR lpszSystemInfo3;
   LPTSTR lpszSystemInfo4;

   DWORD cchBuff = 256;
   DWORD cchBuff2 = 128;  // длина имени компьютера или
                               // пользователя.
   TCHAR tchBuffer[BUFSIZE];
   TCHAR tchBuffer2[512];   // буфер для строки.

    lpszSystemInfo1 = tchBuffer2;
	lpszSystemInfo2 = tchBuffer2;
	lpszSystemInfo3 = tchBuffer;
	lpszSystemInfo4 = tchBuffer;

   // Получаем и отображаем имя компьютера.
   if( GetComputerName(lpszSystemInfo1, &cchBuff) )
   //cout << " Computer name:  " << lpszSystemInfo << endl;
   log_msg("system_info",  "Computer name:  " + string(lpszSystemInfo1));
   
   // Получаем и отображаем имя пользователя.
   if( GetUserName(lpszSystemInfo2, &cchBuff2) )
   //cout << " User name:  " << lpszSystemInfo << endl;
   log_msg("system_info",  "User name:  " + string(lpszSystemInfo2));

   // Получаем и отображаем системную директорию.
   if( GetSystemDirectory(lpszSystemInfo3, MAX_PATH+1) )
   //cout << " System directory:  " << lpszSystemInfo << endl;
   log_msg("system_info",  "System directory:  " + string(lpszSystemInfo3));
   
   // Получаем и отображаем директорию Windows.
   if( GetWindowsDirectory(lpszSystemInfo4, MAX_PATH+1) )
   //cout << " Windows directory:  " << lpszSystemInfo << endl;
   log_msg("system_info",  "Windows directory:  " + string(lpszSystemInfo4));
}
#endif	// STEEL_OS
