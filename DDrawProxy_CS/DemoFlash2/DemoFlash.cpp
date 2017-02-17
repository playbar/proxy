// DemoFlash.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "IGAWin.h"
#include "z_FileIni.h"

HANDLE   g_hThread = NULL;
DWORD  WINAPI  ThreadProc(LPVOID lp);

BOOL     g_bDestWin  = FALSE;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
    	case DLL_PROCESS_ATTACH:
		{	
			char TmpBuf[256];
			ZFileIni   ini("c:\\flash\\exe.ini"); 
			ini.GetString("game","name",TmpBuf,100,"");

			char  TmpExeName[256];
			::GetModuleFileNameA(NULL,TmpExeName,255);

			if  ( strstr(TmpExeName,TmpBuf) != 0)
			{
	            g_bDestWin  = TRUE;
			    g_hThread = ::CreateThread(NULL,0,ThreadProc,0,0,0);
			   
			}
		}
		break;
	    case DLL_PROCESS_DETACH:
		{	
			if  ( g_bDestWin)
			   IGAWIN::UnInit();				
		}
		break;
	}
    return TRUE;
}

void  OnShowFlash(HWND  hwnd)
{
	IGAWIN::CreateIGAWin(hwnd,hwnd);	

	ZFileIni   ini("c:\\Flash\\flash.ini");	
	
	int  x0 =0 ,y0 = 0,x1 =200,y1 =200;
	ini.GetInt("pos","x0",x0,0);
	ini.GetInt("pos","y0",y0,0);
	ini.GetInt("pos","x1",x1,200);
	ini.GetInt("pos","y1",y1,200);
	
	IGAWIN::ShowFlash(hwnd,x0,y0,x1,y1);
	
}




HANDLE  g_hFindEvent = NULL;
TCHAR   g_szEventName[] = {"FIND_EVNET_ZZF"};
DWORD  WINAPI  ThreadProc(LPVOID lp)
{
	::OutputDebugString("ThreadProc1");
	//g_hFindEvent = ::CreateEvent(NULL,TRUE,FALSE,g_szEventName);

	TCHAR   szWindowName[100];
	ZFileIni   ini("c:\\Flash\\exe.ini");	
	
	int  dwDelay = 0;
	ini.GetInt("game","delayTime",dwDelay,3000);
    ::Sleep(dwDelay);

	IGAWIN::Init();

	while( 1 )
	{
		::OutputDebugString("wait1");
		::Sleep(2000);
      //  ::WaitForSingleObject(g_hFindEvent,INFINITE);
 	    ::OutputDebugString("wait2");


		ini.GetString("game","caption",szWindowName,100,"");
		::OutputDebugString(szWindowName);
	//	HWND hwnd = ::FindWindow(NULL,szWindowName);

		DWORD  dwCurrenProcessID = ::GetCurrentProcessId();		
		HWND hwnd = ::GetForegroundWindow();
		if  ( hwnd != NULL )
		{
			DWORD dwProcessID = 0;
			::GetWindowThreadProcessId(hwnd,&dwProcessID);
			 if  ( dwCurrenProcessID == dwProcessID)
			 {
				   OnShowFlash(hwnd);
				   ::OutputDebugString("wait3");				 
				   break;
			 }
		}
	}
	::CloseHandle(g_hThread);
	::CloseHandle(g_hFindEvent);
	::OutputDebugString("ThreadProc2");
	return 0;
}








