/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : z_FileIni.h
*	INI文件读取和写入封装类, add by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once 


class CFileIni  
{
	//enum{ MAX_PATH_ = 256	};
public:
	CFileIni();
	~CFileIni();

	VOID Attach(LPCTSTR szFileTotalName);

	BOOL GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = _T("") );
	BOOL GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

	BOOL WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = _T("") );
	BOOL WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

private:
	TCHAR   m_szFileName[MAX_PATH];

public:
    static LPCTSTR GetAppPath();
	static LPCTSTR GetAppPath(LPCTSTR szFileName);
};

extern CFileIni g_CFileIni;
