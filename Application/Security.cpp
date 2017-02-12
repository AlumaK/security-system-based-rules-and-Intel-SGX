#include "Security.h"
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string.h>
#include <AtlConv.h>
#include <AtlBase.h>

#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif


SC_HANDLE g_schServiceAvgsrv;
SC_HANDLE g_schServiceAvgwd;
SC_HANDLE g_schServiceAvgAMPS;
SC_HANDLE g_schServiceAvgIDSAgent;
SC_HANDLE g_schSCManager;

#pragma comment(lib, "ws2_32.lib") //To link the winsock library 

bool init()
{
	g_schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // servicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == g_schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return false;
	}

	wchar_t* svcname = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, "avgsvc", -1, svcname, 4096);



	g_schServiceAvgsrv = OpenService(
		g_schSCManager,         // SCM database 
		_T("avgsvc"),				// name of service 
		SERVICE_ALL_ACCESS);    // full access 

	if (g_schServiceAvgsrv == NULL)
	{
		printf("OpenService failed (%d)\navgsvc\n", GetLastError());
		CloseServiceHandle(g_schSCManager);
		return false;
	}

	g_schServiceAvgwd = OpenService(
		g_schSCManager,         // SCM database 
		_T("avgwd"),		// name of service 
		SERVICE_ALL_ACCESS);    // full access 

	if (g_schServiceAvgwd == NULL)
	{
		printf("OpenService failed (%d)\navgwd\n", GetLastError());
		CloseServiceHandle(g_schSCManager);
		return false;
	}

	g_schServiceAvgAMPS = OpenService(
		g_schSCManager,         // SCM database 
		_T("AvgAMPS"),		// name of service 
		SERVICE_ALL_ACCESS);    // full access 

	if (g_schServiceAvgAMPS == NULL)
	{
		printf("OpenService failed (%d)\nAvgAMPS\n", GetLastError());
		CloseServiceHandle(g_schSCManager);
		return false;
	}

	g_schServiceAvgIDSAgent = OpenService(
		g_schSCManager,         // SCM database 
		_T("AVGIDSAgent"),	// name of service 
		SERVICE_ALL_ACCESS);    // full access 

	if (g_schServiceAvgIDSAgent == NULL)
	{
		printf("OpenService failed (%d)\nAVGIDSAgent\n", GetLastError());
		CloseServiceHandle(g_schSCManager);
		return false;
	}
	return true;
}

bool isAnyPortOpen()
{
	//Start up Winsock…
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error)
		return false;

	//Did we get the right Winsock version?
	if(wsadata.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		return false;
	}

	//Fill out the information needed to initialize a socket…
	SOCKADDR_IN target; //Socket address information

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(135); //Port to connect on
	target.sin_addr.s_addr = inet_addr("localhost"); //Target IP

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s == INVALID_SOCKET)
	{
		return false; //Couldn't create the socket
	}

	//Try connecting...

	if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		printf("Couldn't connect\n");
		return false; //Couldn't connect
	}
	else
	{
		printf("connect successed\n");
		return true; //Success
	}
}




bool isAntiVirusRunning()
{
	SERVICE_STATUS_PROCESS ssp;
	DWORD dwBytesNeeded;
	// Make sure the service is not already stopped.

	if (!QueryServiceStatusEx(
		g_schServiceAvgsrv,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded))
	{
		printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		return false;
	}

	if (ssp.dwCurrentState == SERVICE_STOPPED || ssp.dwCurrentState == SERVICE_STOP_PENDING)
	{
		printf("Service is stopped.\n");
		return false;
	}

	if (!QueryServiceStatusEx(
		g_schServiceAvgwd,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded))
	{
		printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		return false;
	}

	if (ssp.dwCurrentState == SERVICE_STOPPED || ssp.dwCurrentState == SERVICE_STOP_PENDING)
	{
		printf("Service is stopped.\n");
		return false;
	}

	if (!QueryServiceStatusEx(
		g_schServiceAvgAMPS,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded))
	{
		printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		return false;
	}

	if (ssp.dwCurrentState == SERVICE_STOPPED || ssp.dwCurrentState == SERVICE_STOP_PENDING)
	{
		printf("Service is stopped.\n");
		return false;
	}

	if (!QueryServiceStatusEx(
		g_schServiceAvgIDSAgent,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded))
	{
		printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		return false;
	}

	if (ssp.dwCurrentState == SERVICE_STOPPED || ssp.dwCurrentState == SERVICE_STOP_PENDING)
	{
		printf("Service is stopped.\n");
		return false;
	}
	return true;


	/*HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(false);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(false);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		if (_tcscmp(pe32.szExeFile,TEXT("avgui.exe")) == 0)
		{
			printf("avg is running\n");
			return true;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(false);*/
}

