// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "IniFile.h"
#include <shlobj.h>
#include <shlwapi.h>

TCHAR g_szIniFileName[256];

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	int iTemplen = 0;
	CIniFile iniFile;

	::GetModuleFileName(hModule,g_szIniFileName,256);
	TCHAR* p = wcsrchr(g_szIniFileName,L'\\');
	if(NULL == p)
		p	= wcsrchr(g_szIniFileName, L'/');
	++p;
	*p	= 0;
	wcscat(g_szIniFileName,TEXT("LogFile.ini"));
	//OutputDebugString(szIniFileName);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		iniFile.SetIniFileName( g_szIniFileName );
		TCHAR szLogPathW[MAX_PATH * sizeof(TCHAR)];
		iniFile.GetString(TEXT("LogFile"),TEXT("FilePath"),szLogPathW, sizeof(szLogPathW)/sizeof(szLogPathW[0]));
		iTemplen = wcslen(szLogPathW);
		if (szLogPathW[iTemplen -1] == '\\')
		{
			szLogPathW[iTemplen - 1] = '\0';
		}
	
		if( ! PathFileExists(szLogPathW))
		{
			SHCreateDirectoryEx(NULL, szLogPathW, NULL);
		}
		//OutputDebugString(L"server u DLLMain DLL_PROCESS_ATTACH");
		break;
	case DLL_THREAD_ATTACH:
		//OutputDebugString(L"server u DLLMain DLL_THREAD_ATTACH");
		break;

	case DLL_THREAD_DETACH:
		//OutputDebugString(L"server u DLLMain DLL_THREAD_DETACH");
		break;

	case DLL_PROCESS_DETACH:
		//OutputDebugString(L"server u DLLMain DLL_PROCESS_DETACH");
		break;
	}
	return TRUE;
}

