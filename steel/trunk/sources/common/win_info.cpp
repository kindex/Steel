/*id*********************************************************
	File: common/win_info.cpp
	Unit: Windows Info
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Модуль определения версии Windows.
 ************************************************************/
#include "../steel.h"

#if STEEL_OS == OS_WIN32
	#include <windows.h>
	#include <winbase.h>
	#include <winnt.h>
	
#undef VER_PLATFORM_WIN32s
#undef VER_PLATFORM_WIN32_WINDOWS
#undef VER_PLATFORM_WIN32_NT
#undef VER_SERVER_NT
#undef VER_WORKSTATION_NT
#undef VER_SUITE_SMALLBUSINESS
#undef VER_SUITE_ENTERPRISE
#undef VER_SUITE_BACKOFFICE
#undef VER_SUITE_COMMUNICATIONS
#undef VER_SUITE_TERMINAL
#undef VER_SUITE_SMALLBUSINESS_RESTRICTED
#undef VER_SUITE_EMBEDDEDNT
#undef VER_SUITE_DATACENTER
#undef VER_SUITE_SINGLEUSERTS
#undef VER_SUITE_PERSONAL
#undef VER_SUITE_BLADE
#undef VER_SUITE_EMBEDDED_RESTRICTED
#undef VER_SUITE_SECURITY_APPLIANCE
#undef VER_SUITE_STORAGE_SERVER
#undef VER_SUITE_COMPUTE_SERVER

#define VER_PLATFORM_WIN32s             0
#define VER_PLATFORM_WIN32_WINDOWS      1
#define VER_PLATFORM_WIN32_NT           2
#define VER_SERVER_NT                       0x80000000
#define VER_WORKSTATION_NT                  0x40000000
#define VER_SUITE_SMALLBUSINESS             0x00000001
#define VER_SUITE_ENTERPRISE                0x00000002
#define VER_SUITE_BACKOFFICE                0x00000004
#define VER_SUITE_COMMUNICATIONS            0x00000008
#define VER_SUITE_TERMINAL                  0x00000010
#define VER_SUITE_SMALLBUSINESS_RESTRICTED  0x00000020
#define VER_SUITE_EMBEDDEDNT                0x00000040
#define VER_SUITE_DATACENTER                0x00000080
#define VER_SUITE_SINGLEUSERTS              0x00000100
#define VER_SUITE_PERSONAL                  0x00000200
#define VER_SUITE_BLADE                     0x00000400
#define VER_SUITE_EMBEDDED_RESTRICTED       0x00000800
#define VER_SUITE_SECURITY_APPLIANCE        0x00001000
#define VER_SUITE_STORAGE_SERVER            0x00002000
#define VER_SUITE_COMPUTE_SERVER            0x00004000
	
#endif

#include "system_info.h"
#include "logger.h"
#include "utils.h"

using namespace std;

#if STEEL_OS == OS_WIN32
void DetectWindowsVersion() 
{ 
	//	cout << "File open pattern by Def." << endl; 
	    
	OSVERSIONINFOEX VerInfo; //http://forum.ixbt.com/topic.cgi?id=40:38
	ZeroMemory(&VerInfo, sizeof(OSVERSIONINFOEX));   
	VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	GetVersionEx ((OSVERSIONINFO *) &VerInfo);

	string specialVersionInfo;
	string rawVersion;
	int n = VerInfo.dwMajorVersion;
	int m = VerInfo.dwMinorVersion;

	specialVersionInfo = VerInfo.szCSDVersion;

	//cout << "szCSDVersion : " << specialVersionInfo << endl;

	// cout << "dwOSVersionInfoSize : " << VerInfo.dwOSVersionInfoSize << endl;

	//rawVersion = string(VerInfo.dwMajorVersion)+"+"+string(VerInfo.dwMinorVersion);

	//cout << rawVersion;
	//cout << "dwMajorVersion : " << VerInfo.dwMajorVersion << endl;
	//cout << "dwMinorVersion : " << VerInfo.dwMinorVersion << endl;

	//cout << "BuildNumber : " << VerInfo.dwBuildNumber << endl;

	//cout << "PlatformId : ";// << VerInfo.dwPlatformId << endl;

	log_msg("windows_info",  "System Type : ");
	if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32s) 
		{
			log_msg("windows_info",  " VER_PLATFORM_WIN32s");
		};
	//#define VER_PLATFORM_WIN32s             0

	if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) 
	{
		log_msg("windows_info",  " VER_PLATFORM_WIN32_WINDOWS");
		if(n == 4 && m == 0)
		{
			log_msg("windows_info",  " Microsoft Windows 95 OSR2 ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Microsoft Windows 95 OSR2 
		if(n == 4 && m == 10)
		{
			log_msg("windows_info",  " Microsoft Windows 98 SE ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Microsoft Windows 98 SE 
		if(n == 4 && m == 90)
		{
			log_msg("windows_info",  "  Microsoft Windows Me ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Microsoft Windows Me
	};
	//#define VER_PLATFORM_WIN32_WINDOWS      1

	if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) 
	{
		log_msg("windows_info",  " VER_PLATFORM_WIN32_NT ");
		if(n == 3 && m == 51)
		{
			log_msg("windows_info",  "  Windows NT 3.51 ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}  //Windows NT 3.51

		if(n == 5 && m == 0)
		{
			log_msg("windows_info",  "  Windows 2000 ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Windows 2000
		if(n == 5 && m == 1)
		{
			log_msg("windows_info",  "  Windows XP ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Windows XP
		if(n == 5 && m == 2)
		{
			log_msg("windows_info",  "  Windows .NET Server ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Windows .NET Server 
		if(n == 5 && m == 3)
		{
			log_msg("windows_info",  "  Windows Longhorn ReleaseCandidates ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Windows Longhorn RC

		if(n == 6 && m >= 0)
		{
			log_msg("windows_info",  "  Windows Vista ");
			log_msg("windows_info",  "    With " + specialVersionInfo);
			log_msg("windows_info",  "    Build : " + IntToStr(VerInfo.dwBuildNumber));
		}		// Windows Vista
	};
	//#define VER_PLATFORM_WIN32_NT           2
	//cout << endl;

	//cout << "wServicePackMajor : " << VerInfo.wServicePackMajor << endl;
	//cout << "wServicePackMinor : " << VerInfo.wServicePackMinor << endl;

	//cout << "Product Type : "<< endl;
	log_msg("windows_info",  "Product Type : ");
	if (VerInfo.wProductType == VER_NT_WORKSTATION) 
	{
		log_msg("windows_info",  "  VER_NT_WORKSTATION");
		log_msg("windows_info",  "    Comment: The operating system is Windows Vista, Windows XP Professional,");
		log_msg("windows_info",  "             Windows XP Home Edition, Windows 2000 Professional,");
		log_msg("windows_info",  "             or Windows NT Workstation 4.0.");
	}
	if (VerInfo.wProductType == VER_NT_DOMAIN_CONTROLLER) 
	{
		log_msg("windows_info",  "  VER_NT_DOMAIN_CONTROLLER");
		log_msg("windows_info",  "    Comment: The system is a domain controller.");
	}
	if (VerInfo.wProductType == VER_NT_SERVER) 
	{
		log_msg("windows_info",  "  VER_NT_SERVER");
		log_msg("windows_info",  "    Comment: The system is a server.");
	}

	//#define VER_NT_WORKSTATION              0x0000001
		//The operating system is Windows Vista, Windows XP Professional, Windows XP Home Edition, Windows 2000 Professional, or Windows NT Workstation 4.0.
	//#define VER_NT_DOMAIN_CONTROLLER        0x0000002
		//The system is a domain controller.
	//#define VER_NT_SERVER                   0x0000003
		//The system is a server. 
		//Note that a server that is also a domain controller is reported as VER_NT_DOMAIN_CONTROLLER, not VER_NT_SERVER.


	//cout << "Suite Mask : "<< endl;
	log_msg("windows_info",  "Suite Type : ");
	if (VerInfo.wSuiteMask == VER_SUITE_SMALLBUSINESS) 
	{
	//VER_SUITE_SMALLBUSINESS = $00000001;
	// Microsoft Small Business Server was once installed on the system, 
	// but may have been upgraded to another version of Windows. 

		log_msg("windows_info",  "  VER_SUITE_SMALLBUSINESS");
		log_msg("windows_info",  "    Comment: Microsoft Small Business Server was once installed on the system,");
		log_msg("windows_info",  "             but may have been upgraded to another version of Windows.");

	};
	//#define VER_SUITE_SMALLBUSINESS             0x00000001

	if (VerInfo.wSuiteMask == VER_SUITE_ENTERPRISE) 
	{	
		//VER_SUITE_ENTERPRISE = $00000002; //
		//Windows Server "Longhorn", Enterprise Edition, Windows Server 2003,
		//Enterprise Edition, Windows 2000 Advanced Server, or Windows NT 
		//Server 4.0 Enterprise Edition is installed.

		log_msg("windows_info",  "  VER_SUITE_ENTERPRISE");
		log_msg("windows_info",  "    Comment: Windows Server Longhorn, Enterprise Edition, Windows Server 2003,");
		log_msg("windows_info",  "             Enterprise Edition, Windows 2000 Advanced Server, or Windows NT");
		log_msg("windows_info",  "             Server 4.0 Enterprise Edition is installed.");
	};
	//#define VER_SUITE_ENTERPRISE                0x00000002

	if (VerInfo.wSuiteMask == VER_SUITE_BACKOFFICE) 
	{
	//VER_SUITE_BACKOFFICE = $00000004; //
		//Microsoft BackOffice components are installed.
		log_msg("windows_info",  "  VER_SUITE_BACKOFFICE");
		log_msg("windows_info",  "    Comment: Microsoft BackOffice components are installed.");
	};
	//#define VER_SUITE_BACKOFFICE                0x00000004

	if (VerInfo.wSuiteMask == VER_SUITE_COMMUNICATIONS) 
	{
		log_msg("windows_info",  "  VER_SUITE_COMMUNICATIONS");
	};
	//#define VER_SUITE_COMMUNICATIONS            0x00000008

	if (VerInfo.wSuiteMask == VER_SUITE_TERMINAL) 
	{
	//VER_SUITE_TERMINAL  = $00000010; //
	//Terminal Services is installed. This value is always set. 
	//If VER_SUITE_TERMINAL is set but VER_SUITE_SINGLEUSERTS 
	//is not set, the system is running in application server mode.
		log_msg("windows_info",  "  VER_SUITE_TERMINAL");
		log_msg("windows_info",  "    Comment: Terminal Services is installed. This value is always set.");
	};
	//#define VER_SUITE_TERMINAL                  0x00000010

	if (VerInfo.wSuiteMask == VER_SUITE_SMALLBUSINESS_RESTRICTED) 
	{
	//VER_SUITE_SMALLBUSINESS_RESTRICTED = $00000020; //
	//Microsoft Small Business Server is installed with the restrictive 
	//client license in force. 
		log_msg("windows_info",  "  VER_SUITE_SMALLBUSINESS_RESTRICTED");
		log_msg("windows_info",  "    Comment: Microsoft Small Business Server is installed with the restrictive");
		log_msg("windows_info",  "             client license in force.");
	};
	//#define VER_SUITE_SMALLBUSINESS_RESTRICTED  0x00000020

	if (VerInfo.wSuiteMask == VER_SUITE_EMBEDDEDNT) 
	{
	//VER_SUITE_EMBEDDEDNT = $00000040; //
	//Windows XP Embedded is installed.
		log_msg("windows_info",  "  VER_SUITE_EMBEDDEDNT");
		log_msg("windows_info",  "    Comment: Windows XP Embedded is installed.");
	};
	//#define VER_SUITE_EMBEDDEDNT                0x00000040

	if (VerInfo.wSuiteMask == VER_SUITE_DATACENTER) 
	{
	//VER_SUITE_DATACENTER = $00000080; //
	//Windows Server "Longhorn", Datacenter Edition, Windows Server 2003, 
	//Datacenter Edition or Windows 2000 Datacenter Server is installed.
		log_msg("windows_info",  "  VER_SUITE_DATACENTER");
		log_msg("windows_info",  "    Comment: Windows Server Longhorn, Datacenter Edition, Windows Server 2003,");
		log_msg("windows_info",  "             Datacenter Edition or Windows 2000 Datacenter Server is installed.");
	};
	//#define VER_SUITE_DATACENTER                0x00000080

	if (VerInfo.wSuiteMask == VER_SUITE_SINGLEUSERTS) 
	{
	//VER_SUITE_SINGLEUSERTS  = $00000100; //
	//Remote Desktop is supported, but only one interactive session is 
	//supported. This value is set unless the system is running in 
	//application server mode.
		log_msg("windows_info",  "  VER_SUITE_SINGLEUSERTS");
		log_msg("windows_info",  "    Comment: Remote Desktop is supported, but only one interactive session is");
		log_msg("windows_info",  "             supported. This value is set unless the system is running in ");
		log_msg("windows_info",  "             application server mode.");
	};
	//#define VER_SUITE_SINGLEUSERTS              0x00000100

	if (VerInfo.wSuiteMask == VER_SUITE_PERSONAL) 
	{
	//VER_SUITE_PERSONAL  = $00000200; //
	//Windows Vista Home Premium, Windows Vista Home Basic, 
	//or Windows XP Home Edition is installed.
		log_msg("windows_info",  "  VER_SUITE_PERSONAL");
		log_msg("windows_info",  "    Comment: Windows Vista Home Premium, Windows Vista Home Basic,");
		log_msg("windows_info",  "             or Windows XP Home Edition is installed.");

	};
	//#define VER_SUITE_PERSONAL                  0x00000200

	if (VerInfo.wSuiteMask == VER_SUITE_BLADE) 
	{
	//VER_SUITE_BLADE      = $00000400; //
	//Windows Server 2003, Web Edition is installed.
		log_msg("windows_info",  "  VER_SUITE_BLADE");
		log_msg("windows_info",  "    Comment: Windows Server 2003, Web Edition is installed.");
	};
	//#define VER_SUITE_BLADE                     0x00000400

	if (VerInfo.wSuiteMask == VER_SUITE_EMBEDDED_RESTRICTED) 
	{
	//	cout << "  VER_SUITE_EMBEDDED_RESTRICTED";
		log_msg("windows_info",  "  VER_SUITE_EMBEDDED_RESTRICTED");
	};
	//#define VER_SUITE_EMBEDDED_RESTRICTED       0x00000800

	if (VerInfo.wSuiteMask == VER_SUITE_SECURITY_APPLIANCE) 
	{
	//	cout << "  VER_SUITE_SECURITY_APPLIANCE";
		log_msg("windows_info",  "  VER_SUITE_SECURITY_APPLIANCE");
	};
	//#define VER_SUITE_SECURITY_APPLIANCE        0x00001000

	if (VerInfo.wSuiteMask == VER_SUITE_STORAGE_SERVER) 
	{
	//VER_SUITE_STORAGE_SERVER = $00002000; //
	//Windows Shadow Server 2003 R2 or Windows Shadow Server 2003 is installed.
		log_msg("windows_info",  "  VER_SUITE_STORAGE_SERVER");
		log_msg("windows_info",  "    Comment: Windows Shadow Server 2003 R2 or Windows Shadow Server 2003 is installed.");
	};
	//#define VER_SUITE_STORAGE_SERVER            0x00002000

	if (VerInfo.wSuiteMask == VER_SUITE_COMPUTE_SERVER) 
	{
	//VER_SUITE_COMPUTE_SERVER = $00004000; //
	//Windows Server 2003, Compute Cluster Edition is installed.
		log_msg("windows_info",  "  VER_SUITE_COMPUTE_SERVER");
		log_msg("windows_info",  "    Comment: Windows Server 2003, Compute Cluster Edition is installed.");
	};
	//#define VER_SUITE_COMPUTE_SERVER            0x00004000

	//VER_SUITE_PERSONAL 
	//VER_NT_WORKSTATION 

	//	int nIndex;
	//	nIndex = GetSystemMetrics(SM_CXSMICON);
	    
	//	cout << "SystemMetrics : " << (nIndex) << endl;
} 
#endif // STEEL_OS == OS_WIN32
