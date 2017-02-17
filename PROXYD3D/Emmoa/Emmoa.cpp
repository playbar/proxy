// ADShell.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#define USING_D8


static  BYTE Code[]={
	0x8B,0xFF,
		0x55,
		0x8B,0xEC,
		0x81,0xEC,0x14,0x01,0x00,0x00,
		0x90,
		0xE9,0x00,0x00,0x00,0x00,
};


#if defined(USING_D8)
	#define  _EMMOA_CORE_NAME   "emmoaz.iaf"
#elif defined(USING_D9)
	#define  _EMMOA_CORE_NAME   "emmoaz.iaf"
#endif

static HINSTANCE  s_hEmmoaCoreDll         = NULL;
static void*      s_pFunIGAStartUP        = NULL;
static void*      s_pFunInitSheel         = NULL;



//#include <stdio.h>
#include <io.h>
#include "Compression.h"

#pragma comment (lib,"version.lib")

void DeleteDir(CString strDelDir)
{
    CFileFind ff;
    CString strDir, strFile;
	
    strDir = strDelDir;
    if ( strDir.Right(1) != "\\" )
        strDir += "\\";
	
    strDir += "*.*";
	
    BOOL bFind = ff.FindFile(strDir);
    while ( bFind )
    {
        bFind = ff.FindNextFile();
        if ( ff.IsDots() )
            continue;
        CString strFileName = ff.GetFileName();
		
        strFile = strDelDir;
        if ( strFile.Right(1) != "\\" )
            strFile += "\\";
		strFile += strFileName;
        if ( ff.IsDirectory() )
            DeleteDir(strFile);
        else
        {
            if ( ff.IsReadOnly() )
                SetFileAttributes(strFile, GetFileAttributes(strFile) &
				
				(~FILE_ATTRIBUTE_READONLY) );
            DeleteFile(strFile);
            
        }
    }
    ff.Close();
    RemoveDirectory(strDelDir);
    
}

BOOL GetVersionFromText(DWORD& dw1, DWORD& dw2, DWORD& dw3, DWORD& dw4, LPCTSTR lpszVersion)
{
//	ASSERT(!::IsBadStringPtr(lpszVersion, -1));
	
	TCHAR  szTmpBuf [ 256 ] ;
	::_tcscpy(szTmpBuf,lpszVersion);
	
	TCHAR	*ch		= ::_tcsrchr(szTmpBuf, _T('.'));
	if( ch == NULL ) return	FALSE;
	
	dw1	= _ttol(ch + 1);
	*ch	= 0;
	ch	= _tcsrchr(szTmpBuf, _T('.'));
	if( ch != NULL )
	{
		dw2		= _ttol(ch + 1);
		*ch	= 0;
		ch	= _tcsrchr(szTmpBuf, _T('.'));
		if(ch != NULL)
		{
			dw3		= _ttol(ch + 1);
			*ch	= 0;
			dw4		= _ttol(szTmpBuf);
		}
	}
	
	return	TRUE;
}

BOOL  VersionCmp(LPCTSTR szServerVersion,LPCTSTR szClientVersion)
{
	BOOL bRet;
	
	if  ( szClientVersion == NULL ||szClientVersion[0] == NULL  )  
		return TRUE;
	
	
	DWORD   dwServerVer1  = 0;
	DWORD   dwServerVer2  = 0;
	DWORD   dwServerVer3  = 0;
	DWORD   dwServerVer4  = 0;
	
    bRet = GetVersionFromText(dwServerVer1,dwServerVer2,dwServerVer3,dwServerVer4,szServerVersion);
	if  ( !bRet)  return FALSE;
	
	
	DWORD   dwClientVer1  = 0;
	DWORD   dwClientVer2  = 0;
	DWORD   dwClientVer3  = 0;
	DWORD   dwClientVer4  = 0;
	
    bRet = GetVersionFromText(dwClientVer1,dwClientVer2,dwClientVer3,dwClientVer4,szClientVersion);
    if  ( !bRet)  return TRUE;
	
	if  ( dwServerVer4 > dwClientVer4 )  return TRUE;
	if  ( dwServerVer4 < dwClientVer4 )  return FALSE;
	
	if  ( dwServerVer3 > dwClientVer3 )  return TRUE;
	if  ( dwServerVer3 < dwClientVer3 )  return FALSE;
	
	if  ( dwServerVer2 > dwClientVer2 )  return TRUE;
	if  ( dwServerVer2 < dwClientVer2 )  return FALSE;
	
	if  ( dwServerVer1 > dwClientVer1 )  return TRUE;
	if  ( dwServerVer1 < dwClientVer1 )  return FALSE;
	
	return FALSE;
	
}

DWORD GetFileVersion(LPCTSTR lpszFileName, LPTSTR pszVersion, DWORD &dwSize)
{
	DWORD	dwHandle	= 0;
#if _MSC_VER >= 1300
	dwSize	= ::GetFileVersionInfoSize(lpszFileName, &dwHandle);
#else
	dwSize	= ::GetFileVersionInfoSize((TCHAR*)lpszFileName, &dwHandle);
#endif
	TCHAR	*pData	= new TCHAR[dwSize];	
#if _MSC_VER >= 1300
	if(::GetFileVersionInfo(lpszFileName, 0, dwSize, pData) )
#else
		if(::GetFileVersionInfo((TCHAR*)lpszFileName, 0, dwSize, pData) )
#endif
		{
			void* info_buf = NULL;
			UINT info_len = 0;
			if(::VerQueryValue(pData, _T("\\"), &info_buf, &info_len))
			{
				VS_FIXEDFILEINFO* vs_info = (VS_FIXEDFILEINFO*)info_buf;
				WORD ver0	= LOWORD(vs_info->dwFileVersionLS);// & 0xFF;
				WORD ver1	= HIWORD(vs_info->dwFileVersionLS);//>>16 & 0xFF);
				WORD ver2	= LOWORD(vs_info->dwFileVersionMS);// & 0xFF;
				WORD ver3	= HIWORD(vs_info->dwFileVersionMS);//>>16 & 0xFF);
				dwHandle	= vs_info->dwProductVersionLS;
				dwSize	= wsprintf(pszVersion, _T("%d.%d.%d.%d"), ver3, ver2, ver1, ver0);
			}
		}
		
		delete []pData;
		return	dwHandle;
}
/* 解压升级core*/
BOOL UpdateCore()
{
	char   szAppPath[256];
	::GetModuleFileNameA(NULL,szAppPath,255);
	char* p = strrchr(szAppPath,'\\');
	if  ( p != NULL ) *p = 0;

	char   szSdkName [ 256 ];
	sprintf(szSdkName,"%s\\InterAdtive\\EMMOA\\SDK\\Emmoa.sdk",szAppPath);

	/*
	不需要升级
	*/
    if  ( ::access(szSdkName,0) == -1 )
	{
		#ifdef NEED_OUTPUT_MSG
		OutputDebugString("Emmoa.sdk不存在 ，不需要升级");
		#endif

		return FALSE;
	}

	/*
	解压缩到临时目录
	*/

	char  szTmpZipPath[MAX_PATH];
	sprintf(szTmpZipPath,"%s\\InterAdtive\\EMMOA\\SDK\\TmpZip",szAppPath);

	if(_taccess(szTmpZipPath, 0)==-1)
	{
		if(!::CreateDirectory(szTmpZipPath,NULL))
		{
			#ifdef NEED_OUTPUT_MSG
			char szTmp[200];
			sprintf(szTmp,"创建路径失败 08X",GetLastError());
			OutputDebugString(szTmp);
			#endif

			return FALSE;
		}
	}

    KUnzip   unZip;
	if(!unZip.Unzip(szSdkName,szTmpZipPath))
	{
		#ifdef NEED_OUTPUT_MSG
		char szTmp[300];
		sprintf(szTmp,"解压到临时目录失败 %s",szTmpZipPath);
		OutputDebugString(szTmp);
		#endif
		
		return FALSE;
	}

	/*
	解压成功，取版本号
	*/
	TCHAR    szSDKVersionLocal   [ MAX_PATH ];
	TCHAR    szSDKVersionZip   [ MAX_PATH ];

	DWORD   dwSize = 0;
	TCHAR   szCoreName [ _MAX_PATH +1 ];
	sprintf(szCoreName,"%s\\EMMOAZ.iaf",szAppPath);
	GetFileVersion(szCoreName,szSDKVersionLocal,dwSize); 


	TCHAR   szCoreNameZip [ _MAX_PATH +1 ];
	sprintf(szCoreNameZip,"%s\\emmoaz.iaf",szTmpZipPath);
	dwSize=0;
	GetFileVersion(szCoreNameZip,szSDKVersionZip,dwSize);
	
	#ifdef NEED_OUTPUT_MSG
	char szTmp[300];
	sprintf(szTmp,"版本 %s - %s",szSDKVersionLocal,szSDKVersionZip);
	OutputDebugString(szTmp);
	#endif

	/*
	解压成功，要删除所有临时文件
	*/
	CString strDir = szTmpZipPath;
	DeleteDir(strDir);

	/*
	目前只比较主文件版本,
	*/
	if( VersionCmp(szSDKVersionZip,szSDKVersionLocal) )
	{
		/*
		需要更新
		*/

		if(!unZip.Unzip(szSdkName,szAppPath))
		{
			#ifdef NEED_OUTPUT_MSG
			char szTmp[300];
			sprintf(szTmp,"解压到目标目录失败 %s",szAppPath);
			OutputDebugString(szTmp);
			#endif
			
			/*
			如果更新失败，很危险，
			目前定于以后更新,暂时不删除.
			应该先保存以前的主程序文件，此时重新恢复!!!!!!!!!!!!!!!!!!!!!!
			*/
			return FALSE;
		}
		else
		{
			/*
			更新成功
			*/

			char szTmp[300];
			sprintf(szTmp,"更新成功 %s - %s",szSDKVersionLocal,szSDKVersionZip);
			OutputDebugString(szTmp);
		}
	}
	else
	{
		char szTmp[300];
		sprintf(szTmp,"版本相同或者低，不需要更新 %s - %s",szSDKVersionLocal,szSDKVersionZip);
		OutputDebugString(szTmp);
	}

	/*
	更新成功 或者不需要更新 都要删除sdk包
	*/
	if(!DeleteFile(szSdkName))
	{
		char szTmp[300];
		sprintf(szTmp,"删除失败 %s ",szSdkName);
		OutputDebugString(szTmp);
	}

	return  TRUE;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	switch(ul_reason_for_call)
	{
 		 case DLL_PROCESS_ATTACH:
			 {
				 UpdateCore();
			 }
			 break;			 
         case DLL_PROCESS_DETACH:
			 {		
			 }
			 break;       
	}
    return TRUE;
}


/*加载core  dll*/
BOOL LoadCore()
{
	char   szAppPath[256];
	::GetModuleFileNameA(NULL,szAppPath,255);
	char* p = strrchr(szAppPath,'\\');
	if  ( p != NULL ) *p = 0;
	
	strcat(szAppPath,"\\");
	strcat(szAppPath,_EMMOA_CORE_NAME);
	
	//加载系统dll
	s_hEmmoaCoreDll = ::LoadLibraryA(szAppPath);

	return ( s_hEmmoaCoreDll!= NULL);
	
}

//------------------------------------------------------------------------------
//为XP下面的NP准备的接口
//------------------------------------------------------------------------------
VOID  _declspec(naked)  IGAStartup()
{	
	if  (  NULL == s_pFunIGAStartUP )
	{
		if  (LoadCore())
		    s_pFunIGAStartUP = ::GetProcAddress(s_hEmmoaCoreDll,"IGAStartup");
	}
	
	if  ( NULL != s_pFunIGAStartUP)
	{
		_asm JMP s_pFunIGAStartUP;
	}
	else
	{
#ifdef NEED_OUTPUT_MSG
		::OutputDebugString("未找到Emmoaz.iaf");
#endif

		_asm
		{	   	
			    MOV   EAX  , 0
			    MOV   [ESP + 0x20],EAX
				
				POPAD
				POP   EAX			
				RETN  4
		}
	}
}

//------------------------------------------------------------------------------
//为我们写的d3d8.dll/ded9.dll加载接口
//------------------------------------------------------------------------------

VOID  _declspec(naked)  IGAStartup2()
{
	if  (  NULL == s_pFunIGAStartUP )
	{
		if  (LoadCore())
			s_pFunIGAStartUP = ::GetProcAddress(s_hEmmoaCoreDll,"IGAStartup2");
	}
	
	if  ( NULL != s_pFunIGAStartUP)
	{
		_asm JMP s_pFunIGAStartUP;
	}
}

//------------------------------------------------------------------------------
//为Vista下准备的d3d8.dll/ded9.dll加载接口
//------------------------------------------------------------------------------

DWORD  WINAPI   IGAStartup3(LPVOID  lpResult,LPVOID  lpSP,DWORD dwFunType)
{
	typedef DWORD (WINAPI *LPFUN_IGASTARTUP3    )  (LPVOID  ,LPVOID  ,DWORD );

	DWORD dwResult=0;
	if  (  NULL == s_pFunIGAStartUP )
	{
		if  (LoadCore())
			s_pFunIGAStartUP = ::GetProcAddress(s_hEmmoaCoreDll,"IGAStartup3");
	}
	if  ( NULL != s_pFunIGAStartUP)
	{
		LPFUN_IGASTARTUP3 lpIGAStartup3 = (LPFUN_IGASTARTUP3)s_pFunIGAStartUP;
		dwResult =  lpIGAStartup3(lpResult,lpSP,dwFunType);
	}

	return dwResult ;	
}