// SecurityCheck.cpp: implementation of the SecurityCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecurityCheck.h"
#include "z_FileIni.h"

#pragma  warning(disable: 4786)
#include  <list>
#include  <string>


typedef std::list < std::string >              CHECK_LST  ;   //等待查询的感兴趣窗体
typedef std::list < std::string >::iterator    CHECK_LST_IT  ;  
static CHECK_LST    s_win_list;


SecurityCheck::SecurityCheck()
{
    m_dwTimeOut   = 1000 * 60 * 1 ;   //1分钟扫描一次
}

SecurityCheck::~SecurityCheck()
{

}


DWORD WINAPI SecurityCheck::ThreadProc(void *p)
{		
	::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_LOWEST );	
	SecurityCheck	*_thisApp	= (SecurityCheck *)p;			
	
	HANDLE  h[] = { _thisApp->m_hEventStop};
	
    BOOL  bRun = TRUE;
	while( bRun )
	{
        DWORD  dwResult = ::WaitForMultipleObjects(sizeof(h)/sizeof(HANDLE),h,FALSE,_thisApp->m_dwTimeOut);						
        switch(dwResult)
		{
		case WAIT_OBJECT_0:
			{//退出				
				bRun = FALSE;                   
				break;
			}	
		case WAIT_TIMEOUT:
			{//枚举检测窗体
				if  ( !_thisApp->IsSecurity() )
				{
                    //HANDLE  hProcess = ::GetCurrentProcess()
					//if  ( NULL == hProcess)
				    LOG << "ExitProcess(0)";
					::ExitProcess(0);
				}
                break;
			}
		default:
			break;
		}
		
	}	
	return	0L;
}

/************************************************************************/
/* 启动主应用线程                                                       */
/************************************************************************/
BOOL SecurityCheck::Start()
{
	if  ( m_hWorkingThread != NULL )  return TRUE;	
	
	START_LOG;
	LOG << "启动安全检测线程";
    m_hEventStop = ::CreateEvent(NULL,TRUE,FALSE,NULL) ;
	ASSERT(NULL != m_hEventStop );
    
	m_hWorkingThread	= ::CreateThread( NULL, 0, ThreadProc, this, 0, &m_dwWorkingThreadID);

	END_LOG;
	return	(m_hWorkingThread != NULL);
}

void SecurityCheck::Stop()
{	

	START_LOG;
	LOG << "结束安全检测线程";
	if( m_hWorkingThread != NULL )
	{
		if( m_hEventStop )
		{
			::SetEvent( m_hEventStop );            			
			if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hWorkingThread, 5000))			
			{
				LOG << "强制结束安全检测线程";
				::TerminateThread( m_hWorkingThread , 10 );				
			}
			
			::CloseHandle(m_hWorkingThread);
			m_hWorkingThread	= NULL;
			m_dwWorkingThreadID	= 0L;
			
			::CloseHandle( m_hEventStop );
			m_hEventStop	= NULL;			
		}
	}	
	END_LOG;

	return	;		
}


SecurityCheck* SecurityCheck::CreateInstance()
{
	static SecurityCheck     s_Check;

	
	TCHAR      szFileName [ _MAX_PATH ];


	TCHAR	szBuf[_MAX_PATH ]	= { 0 };
	::GetModuleFileName(NULL, szBuf, _MAX_PATH);
	TCHAR	*p	= ::_tcsrchr(szBuf, '\\');	
	*p	= 0;

	wsprintf(szFileName,_T("%s\\InterAdtive\\EmmoaSecurity.ini"),szBuf);
	ZFileIni   ini(szFileName);

	int  nCount  = 0;
	std::string     str1;
	TCHAR  szValue [ 256+1 ];
	ini.GetInt(_T("title"),_T("count"),nCount,0);
	nCount = 10 ;

	for ( int  i = 0; i< nCount ; i++)
	{
        wsprintf(szBuf,_T("%d"),i+1);
		ini.GetString(_T("title"),szBuf,szValue ,256,"");
		str1  = szValue ;
		if  ( str1 != "")
           s_win_list.push_back(str1);
	}


	return &s_Check;
}


BOOL CALLBACK MyEnumChildProc( HWND hwnd,  LPARAM lParam  )
{
    enum { MAX_WIN_TOTAL = 1024 };
	TCHAR   szWinTotal [ MAX_WIN_TOTAL +1  ];
	::GetWindowText(hwnd,szWinTotal, MAX_WIN_TOTAL);
    
	if  ( szWinTotal[0] != 0 )
	{	
		for (CHECK_LST_IT it =  s_win_list.begin() ; it != s_win_list.end(); it ++ )
		{
			if  ( ::_tcsstr( szWinTotal, it->c_str() ) != NULL )
			{
				LOG << "找到感兴趣窗体：   "  << szWinTotal << "/r/n";
				*((DWORD*)lParam) = 1;
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL  SecurityCheck::IsSecurity() 
{
	DWORD  dwRet = 0 ;
	//枚举桌面子窗体
	HWND    hDesktopWnd = ::GetDesktopWindow();    
	BOOL bRet = ::EnumChildWindows(hDesktopWnd,MyEnumChildProc,(LPARAM) (&dwRet));
	
	if  ( dwRet != 0 )  return FALSE;   

    return TRUE;
}