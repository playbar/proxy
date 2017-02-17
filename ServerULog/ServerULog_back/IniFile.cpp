#include "stdafx.h"
#include <stdio.h>

#include "IniFile.h"

CIniFile::CIniFile(LPCTSTR szFileTotalName)
{
    Attach(szFileTotalName);
}

CIniFile::~CIniFile()
{
}

bool CIniFile::SetIniFileName(LPCTSTR szFileTotalName)
{
	try
	{  
		wcscpy(szFileName,szFileTotalName);
	}
	catch(...)
	{
		throw("ZFileIni Attach err.");
	}
	return true;
}

VOID CIniFile::Attach(LPCTSTR szFileTotalName)
{
    try
	{  
	    wcscpy(szFileName,szFileTotalName);
	}
	catch(...)
	{
		throw("ZFileIni Attach err.");
	}
	return ;
}
BOOL CIniFile::GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal )
{
    
	if (szSection == NULL ||
		szKeyName == NULL ||
		szValue   == NULL )
		return FALSE;

    GetPrivateProfileString(szSection, szKeyName, szDefaultVal, 
		szValue, size, (LPCTSTR)szFileName);

	return TRUE;
}

BOOL CIniFile::GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

    nValue = GetPrivateProfileInt(szSection, szKeyName, DefaultVal, (LPCTSTR)szFileName);

	return TRUE;
}
BOOL CIniFile::WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue, LPCTSTR szDefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

	WritePrivateProfileString(szSection, szKeyName, szValue, (LPCTSTR)szFileName );
	return true;
}
BOOL CIniFile::WriteInt(LPCTSTR szSection,LPCTSTR szKeyName,int nValue,int DefaultVal )
{

	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

	TCHAR sznValue[32 * sizeof(TCHAR)];
	swprintf(sznValue, L"%d", nValue);

	WritePrivateProfileString(szSection, szKeyName, sznValue, (LPCTSTR)szFileName );

	//WritePrivateProfileInt()
	return FALSE;
}

LPCTSTR CIniFile::GetAppPath()
{
	static TCHAR szTmp[256];
	::GetModuleFileName(NULL,szTmp,256);

	TCHAR* p = wcsrchr(szTmp,L'\\');
    if(NULL == p)
		p	= wcsrchr(szTmp, L'/');
	++p;
	*p	= 0;

	return szTmp;
}

LPCTSTR CIniFile::GetAppPath(LPCTSTR szFileName)
{
    static TCHAR szTmp[256];
	::GetModuleFileName(NULL,szTmp,256);
	TCHAR* p = wcsrchr(szTmp,'\\');
    if(NULL == p)
		p	= wcsrchr(szTmp, '/');
	++p;
	*p	= 0;
	wcscat(szTmp,szFileName);
	return szTmp;
}



BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode 
	// string. 
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);

	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
	return TRUE;
}

//-------------------------------------------------------------------------------------
//Description:
// This function maps a wide-character string to a new character string
//
//Parameters:
// lpcwszStr: [in] Pointer to the character string to be converted 
// lpszStr: [out] Pointer to a buffer that receives the translated string. 
// dwSize: [in] Size of the buffer
//
//Return Values:
// TRUE: Succeed
// FALSE: Failed
// 
//Example:
// MByteToWChar(szW,szA,sizeof(szA)/sizeof(szA[0]));
//---------------------------------------------------------------------------------------
BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return TRUE;
}

