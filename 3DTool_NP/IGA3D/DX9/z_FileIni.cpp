#include "stdafx.h"
#include "z_FileIni.h"

ZFileIni* GetIni()
{
	static ZFileIni iniFile;
	return &iniFile;
}

//--------------------------------------------------
ZFileIni::ZFileIni()
{

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

    ::GetPrivateProfileString(szSection, szKeyName, szDefaultVal, 
		szValue, size, (LPCTSTR)szFileName);

	return TRUE;
}

BOOL ZFileIni::GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

    nValue = ::GetPrivateProfileInt(szSection, szKeyName, DefaultVal, (LPCTSTR)szFileName);

	return TRUE;
}
BOOL ZFileIni::WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal  )
{

	return FALSE;
}
BOOL ZFileIni::WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal )
{

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