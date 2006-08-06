#include <conio.h> 
#include <iostream> 
#include <fstream>
#include <string>
#include <windows.h>
#include <winbase.h>

using namespace std;

#define BUFSIZE 1024

LONG RegQueryValueEx(
HKEY hKey, 			// handle to key to query
LPTSTR lpValueName, 	// address of name of value to query
LPDWORD lpReserved, 	// reserved
LPDWORD lpType, 		// address of buffer for value type
LPBYTE lpData, 		// address of data buffer
LPDWORD lpcbData 		// address of data buffer size
);

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

int collectSystemInfo() 
{   
  char szThis[300];
	int pathLenght = GetModuleFileName(NULL, szThis, 300); // http://www.sources.ru/msdn/library/getmodulefilename.shtml
  string fullPathForCurrentApplication;
    for (int i=0; i < pathLenght; i++)
		{
			fullPathForCurrentApplication += szThis[i];
		}
	cout << "Full Path "  << fullPathForCurrentApplication << endl << endl;

  int physicalMemoryInstelled;
  int physicalMemoryAvailable;
  int memoryUsed;
  int totalVirtualMemory;
	
    MEMORYSTATUS memoryStatus;
	
	memset (&memoryStatus, sizeof (MEMORYSTATUS), 0);
	memoryStatus.dwLength = sizeof (MEMORYSTATUS);

	GlobalMemoryStatus (&memoryStatus);
	physicalMemoryInstelled = memoryStatus.dwTotalPhys / 1024;
	physicalMemoryAvailable = memoryStatus.dwAvailPhys / 1024;
	memoryUsed = physicalMemoryInstelled - physicalMemoryAvailable;
	totalVirtualMemory = memoryStatus.dwTotalVirtual / 1024;

	cout << "physicalMemoryInstelled: " << physicalMemoryInstelled << endl;
	cout << "physicalMemoryAvailable: " << physicalMemoryAvailable << endl;
	cout << "memoryUsed: "<< memoryUsed << endl;
	cout << "totalMemoryWithVirtual: " << totalVirtualMemory << endl;
    cout << endl;

   SYSTEM_INFO siSysInfo;

   // Копируем информацию о железе в структуру SYSTEM_INFO.

   GetSystemInfo(&siSysInfo);

   // Отображаем содержимое структуры SYSTEM_INFO.

   printf("Hardware information: \n");
   
   cout << "  OEM ID: " << siSysInfo.dwOemId << endl;

   cout << "  Number of processors: " << siSysInfo.dwNumberOfProcessors << endl;

   cout << "  Page size:  " << siSysInfo.dwPageSize << endl;  

   cout << "  Processor type: " << siSysInfo.dwProcessorType << endl;

 
   cout << "  Minimum application address: 0x" << siSysInfo.lpMinimumApplicationAddress << endl;

   cout << "  Maximum application address: 0x" << siSysInfo.lpMaximumApplicationAddress << endl;

  
   LPTSTR lpszSystemInfo;      // указатель на строку, в которой
                               // будет информация о системе.
   DWORD cchBuff = 256;        // длина имени компьютера или
                               // пользователя.
   TCHAR tchBuffer[BUFSIZE];   // буфер для строки.

   lpszSystemInfo = tchBuffer;

   // Получаем и отображаем имя компьютера.
   if( GetComputerName(lpszSystemInfo, &cchBuff) )
   cout << " Computer name:  " << lpszSystemInfo << endl;
   
   // Получаем и отображаем имя пользователя.
   if( GetUserName(lpszSystemInfo, &cchBuff) )
   cout << " User name:  " << lpszSystemInfo << endl;
    
   // Получаем и отображаем системную директорию.
   if( GetSystemDirectory(lpszSystemInfo, MAX_PATH+1) )
	cout << " System directory:  " << lpszSystemInfo << endl;
   
   // Получаем и отображаем директорию Windows.
   if( GetWindowsDirectory(lpszSystemInfo, MAX_PATH+1) )
	   cout << " Windows directory:  " << lpszSystemInfo << endl;
                   
   getch(); 
   return 0; 
}  