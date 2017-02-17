// ProxyDDraw.cpp : Defines the entry point for the DLL application.
//


#include "stdafx.h"
#include "IGAWin.h"
#include "PlayFlashWindow.h"
#include "z_FileIni.h"


HANDLE g_hevtPlayFlashEvent;

char szBeginFlashFile[256];
char szEndFlashFile[256];
int iwinWidth=0;
int iwinHeith = 0;
bool bIsDown = false;
bool bIsUp = false;
int iBeginPlayTime = 0;
int iEndPlayTime = 0;
int iPlayCount = 0;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				
				TCHAR sziniFilePath[MAX_PATH + 1];
				TCHAR szTemp[256];
				TCHAR szFileAndCmd[1024];
				GetModuleFileName(NULL, sziniFilePath, MAX_PATH);
				(_tcsrchr(sziniFilePath, _T('\\')))[1] = 0;
				strcpy(szTemp, sziniFilePath);
				strcat(sziniFilePath, "InterAdtive\\EMMOA\\Flash\\Flash.ini");
				ZFileIni   ini(sziniFilePath);
				ini.GetString("IGA_PLAYFLASH","BeginFlashFileName",szBeginFlashFile, 256, _T(""));
				ini.GetInt("IGA_PLAYFLASH", "winWidth", iwinWidth, 0 );
				ini.GetInt("IGA_PLAYFLASH", "winHeight", iwinHeith, 0 );
				ini.GetInt("IGA_PLAYFLASH", "BeginPlayTime", iBeginPlayTime, 0 );

				sprintf(szFileAndCmd, "%sInterAdtive\\EMMOA\\Flash\\PlayFlash.exe %sInterAdtive\\EMMOA\\Flash\\%s;%d;%d;%d",szTemp, szTemp,szBeginFlashFile, iBeginPlayTime, iwinWidth, iwinHeith);

 				PROCESS_INFORMATION    processInfo;
				DWORD dwExitCode = 0;
 				memset(&processInfo,0,sizeof(PROCESS_INFORMATION));
 				STARTUPINFO   sa  ;
 				memset(&sa,0,sizeof(STARTUPINFO));
 				sa.cb =sizeof(STARTUPINFO);
 				bool bSuccess = CreateProcess(NULL, szFileAndCmd, NULL,NULL,FALSE,0,NULL,NULL,&sa,&processInfo);

				LARGE_INTEGER litmp;
				LONGLONG qt1,qt2;
				double dft,dff,dfm;
				QueryPerformanceFrequency(&litmp);//获得时钟频率
				dff=(double)litmp.QuadPart;
				QueryPerformanceCounter(&litmp);//获得初始值
				qt1=litmp.QuadPart;

				//WaitForSingleObject((HANDLE)processInfo.dwProcessId, INFINITE );
				if (bSuccess)
				{
					CloseHandle(processInfo.hThread);

					WaitForSingleObject(processInfo.hProcess, INFINITE );

					GetExitCodeProcess(processInfo.hProcess, &dwExitCode );

					CloseHandle( processInfo.hProcess );
				}

				QueryPerformanceCounter(&litmp);//获得终止值
				qt2=litmp.QuadPart;
				dfm=(double)(qt2-qt1);
				dft=dfm/dff;
				ini.WriteInt("IGA_PLAYFLASH", "RealityBeginPlayTime", (int)dft, 0);
				//Sleep((iBeginPlayTime +1) * 1000);
				
				IGAWIN::Init();
			}
			break;
		case DLL_PROCESS_DETACH:
			{		
				IGAWIN::UnInit();

				TCHAR sziniFilePath[MAX_PATH + 1];
				TCHAR szTemp[256];
				TCHAR szFileAndCmd[1024];
				GetModuleFileName(NULL, sziniFilePath, MAX_PATH);
				(_tcsrchr(sziniFilePath, _T('\\')))[1] = 0;
				strcpy(szTemp, sziniFilePath);
				strcat(sziniFilePath, "InterAdtive\\EMMOA\\Flash\\Flash.ini");
				ZFileIni   ini(sziniFilePath);
				ini.GetString("IGA_PLAYFLASH","EndFlashFileName",szEndFlashFile, 256, _T(""));
				ini.GetInt("IGA_PLAYFLASH", "winWidth", iwinWidth, 0 );
				ini.GetInt("IGA_PLAYFLASH", "winHeight", iwinHeith, 0 );
				ini.GetInt("IGA_PLAYFLASH", "EndPlayTime", iEndPlayTime, 0 );

				sprintf(szFileAndCmd, "%sInterAdtive\\EMMOA\\Flash\\PlayFlash.exe %sInterAdtive\\EMMOA\\Flash\\%s;%d;%d;%d",szTemp, szTemp,szEndFlashFile, iEndPlayTime, iwinWidth, iwinHeith);
				PROCESS_INFORMATION    processInfo;
				DWORD dwExitCode = 0;
				memset(&processInfo,0,sizeof(PROCESS_INFORMATION));
				STARTUPINFO   sa  ;
				memset(&sa,0,sizeof(STARTUPINFO));
				sa.cb =sizeof(STARTUPINFO);
				bool bSuccess = CreateProcess(NULL,szFileAndCmd,NULL,NULL,FALSE,0,NULL,NULL,&sa,&processInfo);

				LARGE_INTEGER litmp;
				LONGLONG qt1,qt2;
				double dft,dff,dfm;
				QueryPerformanceFrequency(&litmp);//获得时钟频率
				dff=(double)litmp.QuadPart;
				QueryPerformanceCounter(&litmp);//获得初始值
				qt1=litmp.QuadPart;

				//WaitForSingleObject((HANDLE)processInfo.dwProcessId, INFINITE );
				if (bSuccess)
				{
					CloseHandle(processInfo.hThread);

					WaitForSingleObject(processInfo.hProcess, INFINITE );

					GetExitCodeProcess(processInfo.hProcess, &dwExitCode );

					CloseHandle( processInfo.hProcess );
				}

				QueryPerformanceCounter(&litmp);//获得终止值
				qt2=litmp.QuadPart;
				dfm=(double)(qt2-qt1);
				dft=dfm/dff;
				ini.WriteInt("IGA_PLAYFLASH", "RealityEndPlayTime", (int)dft, 0);
			}
			break;
	}
    return TRUE;
}


#include "ProxyIDtirectDraw.h"

//接管DirectDrawCreate函数
void  _stdcall Hook_IDirectDraw(LPVOID  *lplpDD)
{
	::OutputDebugString("Hook_IDirectDraw");
	*lplpDD = new IGA_IDirectDraw((IDirectDraw*)*lplpDD);	
}
VOID  _declspec(naked) IGA2DStartup(GUID FAR *lpGUID,LPDIRECTDRAW FAR *lplpDD,VOID FAR *pUnkOuter )
{
	OutputDebugString("IGA2DStartup");
	_asm 
	{
		PUSH  lplpDD		
		CALL  Hook_IDirectDraw	


		POPAD		
		POP   ESI 		
		POP   EBP		

		RETN  12
	}	
}


//接管DirectDrawCreateEx函数
void  _stdcall Hook_IDirectDrawEx(REFIID  iid,LPVOID  *lplpDD)
{
	::OutputDebugString("Hook_IDirectDrawEx");
	if ( ::IsEqualIID(iid, Proxy_IDirectDraw  )) 
	{		 
		*lplpDD =  new IGA_IDirectDraw((IDirectDraw*)*lplpDD);
		::OutputDebugString("IDirectDraw");
	}
	else if ( ::IsEqualIID(iid, Proxy_IDirectDraw2 )) 
	{
		*lplpDD = new IGA_IDirectDraw2((IDirectDraw2*)*lplpDD);		                         
		::OutputDebugString("IDirectDraw2");
	}
	else if ( ::IsEqualIID(iid, Proxy_IDirectDraw4)) 
	{
		*lplpDD = new IGA_IDirectDraw4((IDirectDraw4*)*lplpDD);		
		::OutputDebugString("IDirectDraw4");
	}
	else if ( ::IsEqualIID(iid, Proxy_IDirectDraw7)) 
	{
		*lplpDD = new IGA_IDirectDraw7((IDirectDraw7*)*lplpDD);		
		::OutputDebugString("IDirectDraw7");
	}
}

VOID  _declspec(naked) IGA2DStartupEx( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,VOID FAR *pUnkOuter )
{
	OutputDebugString("IGA2DStartupEx");
	_asm 
	{
		PUSH  lplpDD
		PUSH  iid
	    CALL  Hook_IDirectDrawEx	

    
		POPAD
		MOV   EAX , 0
		POP   EDI 
		POP   ESI
		POP   EBX
		POP   EBP		

		RETN  16
	}	
}		
    