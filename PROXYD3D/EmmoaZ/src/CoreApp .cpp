
#include "stdafx.h"
#include "CoreApp.h"

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")


KCoreApp::KCoreApp():
          m_hWorkingThread(NULL),
          m_dwWorkingThreadID(0),
          m_hEventStop(NULL), 
		  m_hEventDo(NULL),
		  m_dwWait(2000),
		  m_dwTimeOut(INFINITE)
{	

}

KCoreApp::~KCoreApp()
{	
}

/************************************************************************/
/* 启动主应用线程                                                       */
/************************************************************************/
BOOL KCoreApp::Start()
{
	if  ( m_hWorkingThread != NULL )  return TRUE;
#ifdef NEED_OUTPUT_MSG
	OutputDebugString("KCoreApp::Start");
#endif

	m_dwWait     = 1000;

    m_hEventStop = ::CreateEvent(NULL,TRUE,FALSE,NULL) ;
	ASSERT(NULL != m_hEventStop );

    m_hEventDo   = ::CreateEvent(NULL,TRUE,FALSE,NULL) ;
    ASSERT(NULL != m_hEventDo );

	m_hWorkingThread	= ::CreateThread( NULL, 0, ThreadProc, this, 0, &m_dwWorkingThreadID);

	
	return	(m_hWorkingThread != NULL);
}

/************************************************************************/
/*  主应用线程回调函数                                                  */
/************************************************************************/
DWORD WINAPI KCoreApp::ThreadProc(void *p)
{	
	try
	{
#ifdef NEED_OUTPUT_MSG
		OutputDebugString("KCoreApp::ThreadProc");
#endif	
		::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_LOWEST );

		KCoreApp	*_thisApp	= (KCoreApp *)p;		
 		::Sleep(_thisApp->m_dwWait);
 
		
		WSADATA	data;
		::WSAStartup(MAKEWORD(1,1), &data);
		
		_thisApp->OnInitThread();

		HANDLE  h[] = { _thisApp->m_hEventStop,  _thisApp->m_hEventDo};

#ifdef NEED_OUTPUT_MSG
		::OutputDebugString("--bRun.\r\n");
#endif
		
		BOOL  bRun = TRUE;
		while( bRun )
		{
			DWORD  dwResult = ::WaitForMultipleObjects(sizeof(h)/sizeof(HANDLE),h,FALSE,_thisApp->m_dwTimeOut);						
			switch(dwResult)
			{
				case WAIT_OBJECT_0:
					{//退出
#ifdef NEED_OUTPUT_MSG
						::OutputDebugString("--ThreadProc end.\r\n");
#endif
					   _thisApp->OnExit();
						bRun = FALSE;                   
					}
					break;
				case WAIT_OBJECT_0 +1 :
					{//具体工作
					   _thisApp->OnUpdateByXML();
#ifdef NEED_OUTPUT_MSG
					   ::OutputDebugString("--ThreadProc onDo.\r\n");
#endif					   
					}
					break;
				case WAIT_TIMEOUT:
					{//超时工作
					   _thisApp->OnTimeOut();
#ifdef NEED_OUTPUT_MSG
					   ::OutputDebugString("--ThreadProc onTimeOut.\r\n");
#endif
					}
				default:
		    		break;
			}

		}

		::WSACleanup();	
#ifdef NEED_OUTPUT_MSG
		::OutputDebugString("ThreadProc bye bye");
#endif
	}catch(...)
	{
#ifdef NEED_OUTPUT_MSG
		::OutputDebugString("发生异常");
#endif		
		START_LOG ;
		LOG << "KCoreApp::ThreadProc异常";
		END_LOG;
		return -1;
	}
	return	0L;
}

/************************************************************************/
/* 结束主应用线程                                                       */
/************************************************************************/
void KCoreApp::Stop()
{	
#ifdef NEED_OUTPUT_MSG
	OutputDebugString("KCoreApp::Stop()");
#endif	
	if( m_hWorkingThread != NULL )
	{
		if( m_hEventStop )
		{
			::SetEvent( m_hEventStop );            			
			if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hWorkingThread, 5000))			
			{
				LOG << "强制结束通讯线程";
				::TerminateThread( m_hWorkingThread , 10 );				
			}
			else
			{
			    LOG << "正常结束通讯线程";
			}

			::CloseHandle(m_hWorkingThread);
			m_hWorkingThread	= NULL;
			m_dwWorkingThreadID	= 0L;

			::CloseHandle( m_hEventStop );
			m_hEventStop	= NULL;

			::CloseHandle( m_hEventDo );
			m_hEventDo	    = NULL;
		}
	}

	return	;		
}


INT KCoreApp::DoUpdate()
{
	::SetEvent(m_hEventDo);
    return  0;
}

INT KCoreApp::UnDo      ()
{
	::ResetEvent(m_hEventDo);
	return 0;
}


INT KCoreApp::OnUpdateByXML()
{		
	return	0L;
}
INT KCoreApp::OnTimeOut()
{
    return 0l;
}

INT KCoreApp::OnExit()
{
    return 0l;
}
INT KCoreApp::OnInitThread()
{
	return 0l;
}
