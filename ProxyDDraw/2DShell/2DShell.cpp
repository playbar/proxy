// 2DShell.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#define  _2D_CORE_NAME   "emmoa2d.iaf"
static HINSTANCE  s_h2DCoreDll         = NULL;
static void*      s_pFunIGA2DStartup   = NULL;
static void*      s_pFunIGA2DStartupEx = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				//取系统dll路径
                char   szAppPath[256];
				::GetModuleFileName(NULL,szAppPath,255);
				char* p = strrchr(szAppPath,'\\');
				if  ( p != NULL ) *p = 0;

				strcat(szAppPath,"\\");
				strcat(szAppPath,_2D_CORE_NAME);
				
				//加载系统dll
			    s_h2DCoreDll = ::LoadLibraryA(szAppPath);
				
			}
			break;
		case DLL_PROCESS_DETACH:
			{			
				if  ( NULL != s_h2DCoreDll )
					::FreeLibrary(s_h2DCoreDll);
				s_h2DCoreDll = NULL;
			}
			break;
	}
    return TRUE;
}



VOID  _declspec(naked) IGA2DStartup(DWORD,DWORD,DWORD )
{
	if  (  NULL == s_pFunIGA2DStartup )
	{
		s_pFunIGA2DStartup = ::GetProcAddress(s_h2DCoreDll,"IGA2DStartup");
	}

	if  ( NULL != s_pFunIGA2DStartup)
	{
		_asm jmp s_pFunIGA2DStartup;
	}
	else
	{
		_asm
		{		
		    POPAD
			POP   ESI
			POP   EBP
			RETN  12
		}
	}
}


VOID  _declspec(naked) IGA2DStartupEx( DWORD,DWORD,DWORD,DWORD )
{
	if  (  NULL == s_pFunIGA2DStartupEx )
	{
		s_pFunIGA2DStartupEx = ::GetProcAddress(s_h2DCoreDll,"IGA2DStartupEx");
	}
	
	if  ( NULL != s_pFunIGA2DStartupEx)
	{
		_asm jmp s_pFunIGA2DStartupEx;
	}
	else
	{
		_asm 
		{		
			POPAD
			MOV   EAX , 0
			POP   EDI 
			POP   ESI
			POP   EBX
			POP   EBP		
			
			RETN  16
		}	
	}
}		