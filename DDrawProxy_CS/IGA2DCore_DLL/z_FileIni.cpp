#include "stdafx.h"
#include <stdio.h>

#include "z_FileIni.h"

ZFileIni::ZFileIni(LPCTSTR szFileTotalName)
{
    Attach(szFileTotalName);
}

ZFileIni::~ZFileIni()
{

}

VOID ZFileIni::Attach(LPCTSTR szFileTotalName)
{
    try
	{  
	    _tcscpy(szFileName,szFileTotalName);
	}
	catch(...)
	{
		throw("ZFileIni Attach err.");
	}
	return ;
}
BOOL ZFileIni::GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal )
{
    
	if (szSection == NULL ||
		szKeyName == NULL ||
		szValue   == NULL )
		return FALSE;

    GetPrivateProfileString(szSection, szKeyName, szDefaultVal, 
		szValue, size, (LPCTSTR)szFileName);

	return TRUE;
}

BOOL ZFileIni::GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

    nValue = GetPrivateProfileInt(szSection, szKeyName, DefaultVal, (LPCTSTR)szFileName);

	return TRUE;
}
BOOL ZFileIni::WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue, LPCTSTR szDefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

	WritePrivateProfileString(szSection, szKeyName, szValue, (LPCTSTR)szFileName );
	return true;
}
BOOL ZFileIni::WriteInt(LPCTSTR szSection,LPCTSTR szKeyName,int nValue,int DefaultVal )
{

	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

	char sznValue[32];
	sprintf(sznValue, "%d", nValue);

	WritePrivateProfileString(szSection, szKeyName, sznValue, (LPCTSTR)szFileName );

	//WritePrivateProfileInt()
	return FALSE;
}

LPCTSTR ZFileIni::GetAppPath()
{
	static TCHAR szTmp[256];
	::GetModuleFileName(NULL,szTmp,256);

	TCHAR* p = _tcsrchr(szTmp,'\\');
    if(NULL == p)
		p	= _tcsrchr(szTmp, '/');
	++p;
	*p	= 0;

	return szTmp;
}

LPCTSTR ZFileIni::GetAppPath(LPCTSTR szFileName)
{
    static TCHAR szTmp[256];
	::GetModuleFileName(NULL,szTmp,256);

	TCHAR* p = _tcsrchr(szTmp,'\\');
    if(NULL == p)
		p	= _tcsrchr(szTmp, '/');
	++p;
	*p	= 0;

	_tcscat(szTmp,szFileName);
	return szTmp;
}