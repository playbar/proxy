#include "stdafx.h"
#include "tchar.h"
#include "z_FileIni.h"


CINIFile g_ZFileIni;

CINIFile::CINIFile()
{

}

CINIFile::~CINIFile()
{

}

VOID CINIFile::Attach(LPCTSTR szFileTotalName)
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
BOOL CINIFile::GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal )
{
    
	if (szSection == NULL ||
		szKeyName == NULL ||
		szValue   == NULL )
		return FALSE;

    ::GetPrivateProfileString(szSection, szKeyName, szDefaultVal, 
		szValue, size, (LPCTSTR)szFileName);

	return TRUE;
}

BOOL CINIFile::GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

    nValue = ::GetPrivateProfileInt(szSection, szKeyName, DefaultVal, (LPCTSTR)szFileName);

	return TRUE;
}
BOOL CINIFile::WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal  )
{

	return FALSE;
}
BOOL CINIFile::WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal )
{

	return FALSE;
}

LPCTSTR CINIFile::GetAppPath()
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

LPCTSTR CINIFile::GetAppPath(LPCTSTR szFileName)
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