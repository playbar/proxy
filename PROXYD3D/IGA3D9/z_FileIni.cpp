/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : z_FileIni.cpp
*	INI文件读取和写入封装类, add by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/


#include "stdafx.h"
#include "z_FileIni.h"


CFileIni g_CFileIni;


CFileIni::CFileIni()
{
	memset( m_szFileName, 0, MAX_PATH );
}

CFileIni::~CFileIni()
{

}

VOID CFileIni::Attach(LPCTSTR szFileTotalName)
{
    try
	{  
	    _tcscpy(m_szFileName,szFileTotalName);
	}
	catch(...)
	{
		throw("ZFileIni Attach err.");
	}
	return ;
}
BOOL CFileIni::GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal )
{
    
	if (szSection == NULL ||
		szKeyName == NULL ||
		szValue   == NULL )
		return FALSE;

    ::GetPrivateProfileString(szSection, szKeyName, szDefaultVal, 
		szValue, size, (LPCTSTR)m_szFileName);

	return TRUE;
}

BOOL CFileIni::GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal  )
{
	if (szSection == NULL ||
		szKeyName == NULL )
		return FALSE;

    nValue = ::GetPrivateProfileInt(szSection, szKeyName, DefaultVal, (LPCTSTR)m_szFileName);

	return TRUE;
}
BOOL CFileIni::WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal  )
{

	return FALSE;
}
BOOL CFileIni::WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal )
{

	return FALSE;
}

LPCTSTR CFileIni::GetAppPath()
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

LPCTSTR CFileIni::GetAppPath(LPCTSTR szFileName)
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