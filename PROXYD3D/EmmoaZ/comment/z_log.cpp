#include "stdafx.h"
#include "z_log.h"

//是否输出日志的标记
extern int g_nDebug;

ZLog::ZLog() 
{

}
ZLog::~ZLog()
{

}


void  ZLog::Printf(LPCTSTR   lpFormat , ...)
{
     Log(_T("no implement ZLog::Printf(LPCTSTR   lpFormat , ...).\r\n")) ;
}

ZLog& ZLog::operator<<(LPCTSTR  lpStr)         
{
	  Log(lpStr);
      return *this;
}

ZLog& ZLog::operator<<(char           n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%d",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(unsigned char  n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%u",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(short          n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%d",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(unsigned short n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%u",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(int            n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%d",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(unsigned int   n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%u",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(long           n    )         
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%d",n);
	Log(szBuf);
	return *this;
}     
ZLog& ZLog::operator<<(unsigned long  n    )          
{
	TCHAR   szBuf[32];
	wsprintf(szBuf,"%u",n);
	Log(szBuf);
	return *this;
}    
ZLog& ZLog::operator<<(bool           n    )       
{
	TCHAR   szBuf[32];	
	wsprintf(szBuf,"%u", n? "TRUE" : "FALSE" );
	Log(szBuf);
	return *this;

}
//二进制字符串
ZLog& ZLog::LogHex(const unsigned char* d ,size_t len )          
{
	static TCHAR hexTab[   ] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};		

	if ( d!= NULL && len != 0)  
	{		
		size_t  nlen     = min(128,len);
		TCHAR   szBuf [3*128 +sizeof(TCHAR)] ;
        

		size_t i = 0;
		for( i = 0 ; i<  nlen; i++)
		{									
            szBuf[(i<<1) +i   ] = hexTab [ (d[i]>>4) & 0x0F ];
			szBuf[(i<<1) +i +1] = hexTab [ d[i]      & 0x0F ];
			szBuf[(i<<1) +i +2] = ' ';
		}
		szBuf [(i<<1) +i ] = '\0';

		Log(szBuf);

		if (len > 128 )
		{
		   len -=128;
		   LogHex(d+128,len);
		}

	}
	return *this;
}    


BOOL ZLog::Log(LPCTSTR  lpStr)        
{
	::OutputDebugString(lpStr);
     return TRUE;
}
//行开始  结束
VOID ZLog::Start()
{

}
VOID ZLog::End()
{

}


/*写log到文件*/
ZLogFile::ZLogFile  ():m_hFile(NULL)
{


}
ZLogFile::ZLogFile (LPCTSTR lpszName):m_hFile(NULL)
{
	TCHAR  szFilePath [ _MAX_PATH +1 ]; 	
	::GetModuleFileName(NULL, szFilePath, _MAX_PATH);
	TCHAR	*p	= ::_tcsrchr(szFilePath, '\\');	
	*p = 0;
	
	TCHAR  szFileName [ _MAX_PATH +1 ];
    wsprintf(szFileName,_T("%s\\%s.LOG"),szFilePath,lpszName);	
	_createFile(szFileName);
}
ZLogFile::ZLogFile (HINSTANCE  hDll,LPCTSTR lpszName) :m_hFile(NULL)
{
	TCHAR  szFilePath [ _MAX_PATH +1 ]; 	
	::GetModuleFileName(hDll, szFilePath, _MAX_PATH);
	TCHAR	*p	= ::_tcsrchr(szFilePath, '\\');	
	*p = 0;
	
	TCHAR  szFileName [ _MAX_PATH +1 ];
    wsprintf(szFileName,_T("%s\\%s.LOG"),szFilePath,lpszName);	
	::OutputDebugString("ZLogFile::ZLogFile (HINSTANCE  hDll,LPCTSTR lpszName) :m_hFile(NULL)");
	_createFile(szFileName);
}

ZLogFile::~ZLogFile ()
{
	_closeFile();
}
BOOL      ZLogFile::Attach(LPCTSTR lpszName)
{
	if  ( INVALID_HANDLE_VALUE != m_hFile )
		_closeFile();

	::OutputDebugString("Attach");
	::OutputDebugString(lpszName);
	return  _createFile(lpszName);
}

BOOL      ZLogFile::_createFile(LPCTSTR lpszFileName)
{   
	m_hFile = ::CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
	::_tcscpy(m_szFileName,lpszFileName);

	//if  ( m_hFile == INVALID_HANDLE_VALUE	)
	    ::OutputDebugString(lpszFileName);

	m_dwSize   = 0;
	memset(&ol,0,sizeof(ol));
	return m_hFile != INVALID_HANDLE_VALUE;	
}
VOID      ZLogFile::_closeFile()
{
    if (m_hFile == INVALID_HANDLE_VALUE)  return ;
	::CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}

BOOL ZLogFile::Log(LPCTSTR  lpStr)        
{
	if(g_nDebug>0)
	{
		if (m_hFile == INVALID_HANDLE_VALUE)  return FALSE;
		
		DWORD   dwWrite;	
		::WriteFile(m_hFile,lpStr,_tcslen(lpStr),&dwWrite,&ol);
		ol.Offset  +=dwWrite;
		m_dwSize   += dwWrite;
		if (m_dwSize > 1024*1024*20)
		{
			_closeFile();
			
			TCHAR  szFilePath [ _MAX_PATH +1 ]; 	
			TCHAR	*p	= ::_tcsrchr(m_szFileName, '.');	
			*p = 0;	
			
			SYSTEMTIME  st;
			::GetLocalTime(&st);

			wsprintf(szFilePath,_T("%s_%02d%02d%02d.LOG"),m_szFileName,st.wYear,st.wMonth,st.wDay);	
			_createFile(szFilePath);
		}	
	}
	return TRUE;
}

//行开始  结束
VOID ZLogFile::Start()
{
	if(g_nDebug>0)
	{
		DWORD   dwWrite;	
		TCHAR   szBuf[128];
		
		SYSTEMTIME  st;
		::GetLocalTime(&st);
		
		wsprintf(szBuf,_T("%02d:%02d:%02d:%06d  <TID=%8X>"),
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds,		
			::GetCurrentThreadId());
		::WriteFile(m_hFile,szBuf,_tcslen(szBuf),&dwWrite,&ol);
		ol.Offset  +=dwWrite;
		m_dwSize   += dwWrite;	
	}
}
VOID ZLogFile::End()
{
	if(g_nDebug>0)
	{
		DWORD   dwWrite;	
		TCHAR   szBuf[128];	
		
		wsprintf(szBuf,_T("\r\n"));
		::WriteFile(m_hFile,szBuf,_tcslen(szBuf),&dwWrite,&ol);
		ol.Offset  +=dwWrite;
		m_dwSize   += dwWrite;	
	}
}

#if   defined(ENABLE_FILE_LOG)
   ZLogFile   g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log;
#elif defined(ENABLE_AUTO_FILE_LOG)
   ZLogFile   g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log("emmoaz");
#else
   ZLog       g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log;
#endif



