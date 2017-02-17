
#include "ServiceManager.h"
#include <assert.h>
#include "stdio.h"
CServiceModule g_ServiceModule;

inline void CServiceModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid)
{
	//CComModule::Init(p, h, plibid);

	m_bService = TRUE;

	LoadString(h, nServiceNameID, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));

	// set up the initial service status 
	m_hServiceStatus = NULL;
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_status.dwCurrentState = SERVICE_STOPPED;
	m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_status.dwWin32ExitCode = 0;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;
}

LONG CServiceModule::Unlock()
{
	LONG l = CComModule::Unlock();
	if (l == 0 && !m_bService)
		PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
	return l;
}

BOOL CServiceModule::IsInstalled()
{
	BOOL bResult = FALSE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

BOOL CServiceModule::Install()
{
	if (IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[_MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

	//m_szServiceName="LogFileStat";

	SC_HANDLE hService = ::CreateService(
		hSCM, m_szServiceName, m_szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS ,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T("RPCSS\0"), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	Start();
	return TRUE;
}

BOOL CServiceModule::Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	MessageBox(NULL, _T("Service could not be deleted"), m_szServiceName, MB_OK);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions
void CServiceModule::LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	if (m_bService)
	{
		/* Get a handle to use with ReportEvent(). */
		hEventSource = RegisterEventSource(NULL, m_szServiceName);
		if (hEventSource != NULL)
		{
			/* Write to event log. */
			ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
			DeregisterEventSource(hEventSource);
		}
	}
	else
	{
		// As we are not running as a service, just write the error to the console.
		_putts(chMsg);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
inline void CServiceModule::Start()
{
	SERVICE_TABLE_ENTRY st[] =
	{
		{ m_szServiceName, _ServiceMain },
		{ NULL, NULL }
	};
	if (m_bService && !::StartServiceCtrlDispatcher(st))
	{
		m_bService = FALSE;
	}
	if (m_bService == FALSE)
		Run();
}

inline void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
	// Register the control request handler
	m_status.dwCurrentState = SERVICE_START_PENDING;
	m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
	if (m_hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(SERVICE_START_PENDING);

	m_status.dwWin32ExitCode = S_OK;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;

	//AfxMessageBox("ServiceMain");

	// When the Run function returns, the service has stopped.
	Run();

	SetServiceStatus(SERVICE_STOPPED);
	LogEvent(_T("Service stopped"));
}

inline void CServiceModule::Handler(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		SetServiceStatus(SERVICE_STOP_PENDING);
		PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		LogEvent(_T("Bad service request"));
	}
}

void WINAPI CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	g_ServiceModule.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
	g_ServiceModule.Handler(dwOpcode); 
}

void CServiceModule::SetServiceStatus(DWORD dwState)
{
	m_status.dwCurrentState = dwState;
	::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CServiceModule::Run()
{
	g_ServiceModule.dwThreadID = GetCurrentThreadId();
	LogEvent(_T("Service started"));
	if (m_bService)
		SetServiceStatus(SERVICE_RUNNING);

	MSG msg;
	SetTimer(NULL, 0,1000,NULL);
	while (GetMessage(&msg, 0, 0, 0) && m_status.dwCurrentState == SERVICE_RUNNING)
	{
	
		DispatchMessage(&msg);
	}
}
//停止服务
bool CServiceModule::OnStopService(void)
{	
	SC_HANDLE hSCM =OpenSCManagerA(NULL,NULL,SC_MANAGER_ALL_ACCESS);   
	//ASSERT(hSCM);         
	//   open   the   service   
	SC_HANDLE   hService =OpenServiceA(hSCM, m_szServiceName,SERVICE_ALL_ACCESS);   
	//ASSERT(hService);   
	//   stop   the   service   
	SERVICE_STATUS   ss;   
	BOOL b =ControlService(hService,SERVICE_CONTROL_STOP,  &ss);   
	if(!b){   
		;
		//DWORD  dwErr =GetLastError();   
		//ASSERT(0);
	}   
	//   close   the   service   handle   
	CloseServiceHandle(hService);   
	//   close   the   service   control   manager   handle   
	CloseServiceHandle(hSCM);  


	return   TRUE;   
}

//启动服务
bool CServiceModule::OnStartService(void)
{
	SERVICE_STATUS   ssStatus;     
	DWORD   dwOldCheckPoint;     
	DWORD   dwStartTickCount;   
	DWORD   dwWaitTime;   
	DWORD   dwStatus;   

	SC_HANDLE schSCManager =OpenSCManagerA(NULL,NULL,SC_MANAGER_ALL_ACCESS);   
	//ASSERT(schSCManager);  
	SC_HANDLE schService   =   OpenService(     
		schSCManager,                     //   SCM   database     
		m_szServiceName,                     //   service   name   
		SERVICE_ALL_ACCESS);     

	if   (schService   ==   NULL)     
	{     
		//AfxMessageBox("OpenService");     
	}   

	if   (!StartService(   
		schService,     //   handle   to   service     
		0,                       //   number   of   arguments     
		NULL)   )             //   no   arguments     
	{   
		//AfxMessageBox("StartService");     
	}   
	else     
	{   
		printf("Service   start   pending.\n");     
	}   

	//   Check   the   status   until   the   service   is   no   longer   start   pending.     

	if   (!QueryServiceStatus(     
		schService,       //   handle   to   service     
		&ssStatus)   )     //   address   of   status   information   structure   
	{   
		//AfxMessageBox("QueryServiceStatus");     
	}   

	//   Save   the   tick   count   and   initial   checkpoint.   

	dwStartTickCount   =   GetTickCount();   
	dwOldCheckPoint   =   ssStatus.dwCheckPoint;   

	while   (ssStatus.dwCurrentState   ==   SERVICE_START_PENDING)     
	{     
		dwWaitTime   =   ssStatus.dwWaitHint   /   10;   
		if(   dwWaitTime   <   1000   )   
			dwWaitTime   =   1000;   
		else   if   (   dwWaitTime   >   10000   )   
			dwWaitTime   =   10000;   
		Sleep(   dwWaitTime   );     

		if   (!QueryServiceStatus(     
			schService,       //   handle   to   service     
			&ssStatus)   )     //   address   of   structure   
			break;     
		if   (   ssStatus.dwCheckPoint   >   dwOldCheckPoint   )   
		{   
			dwStartTickCount   =   GetTickCount();   
			dwOldCheckPoint   =   ssStatus.dwCheckPoint;   
		}   
		else   
		{   
			if(GetTickCount()-dwStartTickCount   >   ssStatus.dwWaitHint)   
			{   
				break;   
			}   
		}   
	}     

	if   (ssStatus.dwCurrentState   ==   SERVICE_RUNNING)     
	{   
		printf("StartService   SUCCESS.\n");     
		dwStatus   =   NO_ERROR;   
	}   
	else     
	{     
		printf("\nService   not   started.   \n");   
		printf("     Current   State:   %d\n",   ssStatus.dwCurrentState);     
		printf("     Exit   Code:   %d\n",   ssStatus.dwWin32ExitCode);     
		printf("     Service   Specific   Exit   Code:   %d\n",     
			ssStatus.dwServiceSpecificExitCode);     
		printf("     Check   Point:   %d\n",   ssStatus.dwCheckPoint);     
		printf("     Wait   Hint:   %d\n",   ssStatus.dwWaitHint);     
		dwStatus   =   GetLastError();   
	}     
	CloseServiceHandle(schService);     
	return   dwStatus;   

}
