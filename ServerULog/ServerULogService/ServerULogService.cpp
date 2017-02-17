// IGALogFileService.cpp : 定义控制台应用程序的入口点。
//


#include "ServerULogService.h"
#include "ServiceManager.h"
#include "SqlOperate.h"
#include <map>
#include <string>
#include <Windows.h>
using namespace std;

TCHAR				g_szServiceName[_MAX_PATH];
TCHAR				g_szServiveLogFile[_MAX_PATH];
CString				g_strLogFilePath;
CString				g_strServiceINIPath;
CString             g_strTimer;
CString				g_strLastTime;
CString				g_strHomeDir;
int					g_iLogFilePosition;


CSqlOperate			g_sqlOperate;
CString				g_strServerName;
CString				g_strDatabaseName;
CString				g_strUserName;
CString				g_strPassword;
int					g_iTotalSum;

#define SHOW_DEBUG 1

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	//::CoInitialize(NULL);
	try
	{
		OutputDebugString("_tmain() begin");
		InitData();
		AutoToDB();
		SERVICE_TABLE_ENTRY service_table[] =
		{
			{ g_szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};
		if (argc >= 2) 
		{
			if (stricmp(argv[1], "/install") == 0)
			{
				 
				if(InstallService())
					OutputDebugString("安装Log服务成功!");
				//g_ServiceModule.Install();
			}
			else if (stricmp(argv[1], "/uninstall") == 0)
			{
				UninstallService();
				//g_ServiceModule.Uninstall();
			}
		}

		if (!::StartServiceCtrlDispatcher(service_table))
		{
			OutputDebugString("Register Service Main Function Error!");
			//LogEvent(_T("Register Service Main Function Error!"));
		}
		OutputDebugString("_tmain end");
		//::CoUninitialize();
	}
	catch(...)
	{
		OutputDebugString("_tmain Error");
		return -1;
	}
	return 0;
}

void InitData()
{
	OutputDebugString("InitData()");

	g_hServiceStatus = NULL;
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;

	TCHAR szFileName[_MAX_PATH];
	::GetModuleFileName(NULL, szFileName, _MAX_PATH);
	TCHAR	*p	= ::_tcsrchr(szFileName, _T('\\'));
	if(!p)
		p = ::_tcsrchr(szFileName, _T('/'));
	*++p = 0;
	_snprintf(g_szServiveLogFile,_MAX_PATH,"%s\\ServerULogService.log",szFileName);
	g_strServiceINIPath.Format("%s\\ServerULogService.INI",szFileName);

	GetPrivateProfileString("LogFileInfo","LogFileDirectory","",g_strLogFilePath.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strLogFilePath.ReleaseBuffer();

	GetPrivateProfileString("LogFileInfo","LastDate",		"",	g_strLastTime.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strLastTime.ReleaseBuffer();

	g_iLogFilePosition = GetPrivateProfileInt("LogFileInfo","LogFilePositoin", 0, g_strServiceINIPath);
	GetPrivateProfileString("LogFileInfo","TimeBlank","10",		g_strTimer.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strTimer.ReleaseBuffer();

	GetPrivateProfileString("LogFileInfo","HomeDir","10",		g_strHomeDir.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strHomeDir.ReleaseBuffer();

	GetPrivateProfileString("SqlInfo","ServerName","127.0.0.1",	g_strServerName.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strServerName.ReleaseBuffer();

	GetPrivateProfileString("SqlInfo","DatabaseName","test",	g_strDatabaseName.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strDatabaseName.ReleaseBuffer();

	GetPrivateProfileString("SqlInfo","UserName","test",		g_strUserName.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strUserName.ReleaseBuffer();

	GetPrivateProfileString("SqlInfo","Password","test",		g_strPassword.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strPassword.ReleaseBuffer();

	GetPrivateProfileString("LogFileInfo","NAME","Emmoa-SULogService",		g_szServiceName,_MAX_PATH,g_strServiceINIPath);
	//OutputDebugString( g_strServiceINIPath );
	g_dwThreadID=0;
	g_hThread=NULL;

}

//*********************************************************

//*********************************************************
void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv)
{
	//::CoInitialize(NULL);

	OutputDebugString("ServiceMain() begin");

	// Register the control request handler
	g_hServiceStatus = RegisterServiceCtrlHandler(g_szServiceName, ServiceStrl);
	if (g_hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);

	//启动线程执行日志导入任务
	//InitData();
	g_hThread	= ::CreateThread(NULL, 0, ThreadProc, NULL, 0, &g_dwThreadID);

	g_ServiceStatus.dwWin32ExitCode = S_OK;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);

	/*
	创建失败
	*/
	if(g_hThread == NULL)
	{
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);
		//LogEvent(_T("Service stopped"));
	}
	OutputDebugString("ServiceMain end");
}

//*********************************************************
//Functiopn:			ServiceStrl
//Description:			服务控制主函数，这里实现对服务的控制，
//						当在服务管理器上停止或其它操作时，将会运行此处代码
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	OutputDebugString("ServiceStrl() begin");
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		/*if(m_dwThreadID!=0)
            PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);*/
		if(g_hThread)
		{
			TerminateThread(g_hThread,0);
			g_hThread=NULL;
			g_dwThreadID=0;
		}

		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);
		OutputDebugString("SERVICE_CONTROL_STOP");
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		/*if(g_hThread)
		{
			TerminateThread(g_hThread,0);
			g_hThread=NULL;
			g_dwThreadID=0;
		}
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);
		OutputDebugString("SERVICE_CONTROL_SHUTDOWN");*/
		break;
	default:
		OutputDebugString(_T("Bad service request"));
		break;
	}
	OutputDebugString("ServiceStrl() end");
}
//*********************************************************
//Functiopn:			IsInstalled
//Description:			判断服务是否已经被安装
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL IsInstalled()
{
	BOOL bResult = FALSE;
	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, g_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

//*********************************************************
//Functiopn:			InstallService
//Description:			安装服务函数
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL InstallService()
{
	OutputDebugString("InstallService() begin");
	if (IsInstalled())
		return TRUE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		//MessageBox(NULL, _T("Couldn't open service manager"), g_szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务
	SC_HANDLE hService = ::CreateService(
		hSCM, g_szServiceName, g_szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		//MessageBox(NULL, _T("Couldn't create service"), g_szServiceName, MB_OK);
		return FALSE;
	}

	if (!StartService(
		hService,	// handle to service 
		0,          // number of arguments 
		NULL) )     // no arguments 
    {
  //      CTime	time = CTime::GetCurrentTime();
		//Log(m_szLogFilePath,"%s\t%s Error: %d",time.Format("%Y-%m-%d %H:%M:%S"),"StartService失败",GetLastError());
		
    }
    else 
    {
        OutputDebugString("Service start pending"); 
    }
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	OutputDebugString("InstallService() end");
	return TRUE;
}

//*********************************************************
//Functiopn:			UninstallService
//Description:			删除服务函数
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL UninstallService()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), g_szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, g_szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), g_szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent(_T("Service could not be deleted"));
	return FALSE;
}

//*********************************************************
//Functiopn:			LogEvent
//Description:			记录服务事件
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void LogEvent(LPCTSTR pFormat, ...)
{
	//OutputDebugString(pFormat);

	return;

	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, g_szServiceName);
	if (hEventSource != NULL) 
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

DWORD WINAPI ThreadProc(void *p)
{
	//::CoInitialize(NULL);
	try
	{
		OnSetTimer();
		OutputDebugString("设置完成定时器");
		MSG   msg;  
		BOOL bRet;
		while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
		{ 
			if (bRet == -1)
			{
				// handle the error and possibly exit
				CTime	time = CTime::GetCurrentTime();
				Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"GetMessage失败");
				break;
			}
			else
			{
				OutputDebugString("TranslateMessage");
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
	}
	catch(...)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"ThreadProc未知异常");
	}
	//::CoUninitialize();
	return 0;
}

void OnSetTimer() 
{
	OutputDebugString("OnSetTimer()");
	int timer = atoi(g_strTimer);
	SetTimer(NULL,ID_TIMER2, 1000*60*timer, (TIMERPROC) TimerProc);
	CTime	time = CTime::GetCurrentTime();
	Log(g_szServiveLogFile,"%s\t%s %d分钟",time.Format("%Y-%m-%d %H:%M:%S"),"开始启动计时器",timer);
}

void CALLBACK TimerProc( 
						HWND hwnd,       
						UINT message,     
						UINT idTimer,     
						DWORD dwTime)   
{
	OutputDebugString("TimerProc()");
	CTime timeStart = CTime::GetCurrentTime();
	Log(g_szServiveLogFile,"开始时间：%s",timeStart.Format("%Y-%m-%d %H:%M:%S") );
	g_iTotalSum = 0;
	AutoToDB();
	CTime timeEnd = CTime::GetCurrentTime();
	CTimeSpan need = timeEnd - timeStart;

	Log(g_szServiveLogFile,"%s\t耗时: %s,本次解析总数 %d",
		timeEnd.Format("%Y-%m-%d %H:%M:%S"),
		need.Format("%H:%M:%S\r\n"), g_iTotalSum );
	g_iTotalSum = 0;
}

//--------------------------------------------------------------------------------------
// 从路径中解析游戏ID 
// CString &strFileName OUT
// int & iGameId OUT
//--------------------------------------------------------------------------------------
bool FindGameIDAndFileName(CString & strSrc, CString &strFileName, int & iGameId)
{
	/*
	如果路径比g_strHomeDir短，放弃处理
	*/
	if(strSrc.GetLength()<g_strHomeDir.GetLength())
		return false;

	//	确保这两个路径都是大小写相同的。
	int iBStrPosition = strSrc.Find(g_strHomeDir);

	/*
	如果不是以g_strHomeDir开头，放弃处理
	*/
	if (iBStrPosition != 0)
		return false;

	/*
	认为g_strHomeDir目录的子目录是游戏id相关的 !!!!!!
	*/
	int nPos = g_strHomeDir.GetLength();
	if(strSrc[nPos]=='\\')
		nPos++;

	int nPosEnd = strSrc.Find("\\", nPos );
	if(nPosEnd>0)
	{
		CString strTemp = strSrc.Mid(nPos, nPosEnd - nPos );
		iGameId = atoi((char*)(LPCSTR)strTemp);
	}

	//
	//提取短文件名称
	//
	nPos = strSrc.ReverseFind('\\');
	nPosEnd = strSrc.ReverseFind('.');
	if(nPosEnd> nPos)
	{
		//有效的扩展名
		strFileName = strSrc.Mid(nPos + 1, nPosEnd - nPos -1 );
	}
	else
	{
		//没有扩展名
		strFileName = strSrc.Mid(nPos + 1);
	}
	
	return true;
}

bool LogFileIntoSqlDatabase(LPCSTR szLogFile,CString strTableName)
{

	CDownFileInfo	downInfo;
	int len = sizeof(CDownFileInfo);
	memset(&downInfo, 0, len);

	HANDLE hFile = CreateFile(szLogFile, GENERIC_WRITE|GENERIC_READ, 
		FILE_SHARE_WRITE | FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		Log(g_szServiveLogFile,"日志文件打开错误, %s", szLogFile );
		return false;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile, NULL,PAGE_READWRITE, 0, dwFileSize, NULL);   
	if (hFileMap == NULL) 
	{ 
		Log(g_szServiveLogFile,"文件映射错误");
		return false;
	}

	LPCTSTR pBuf = (LPTSTR) MapViewOfFile(hFileMap,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		dwFileSize);  
	if (pBuf == NULL)
	{
		Log(g_szServiveLogFile,"日志文件内容错误");
		return false;
	}
	while ( g_iLogFilePosition < dwFileSize )
	{
		CopyMemory( &downInfo, pBuf + g_iLogFilePosition , len);
		CString strSql;
		CString strPathFileName;
		CString strFileName;
		int iGameId = 0;
		strPathFileName = downInfo.m_szFileName;
		strPathFileName.MakeLower();
		FindGameIDAndFileName(strPathFileName, strFileName, iGameId );
		char szTime[50];
		memset(szTime, 0, sizeof(szTime));
		sprintf(szTime, "%d-%d-%d %d:%d:%d",\
			downInfo.m_st.wYear, downInfo.m_st.wMonth, downInfo.m_st.wDay, \
			downInfo.m_st.wHour,downInfo.m_st.wMinute,downInfo.m_st.wSecond);


		strSql.Format("insert into %s (r_datetime, r_PathFile, r_GameID, r_FileName, r_downrate, r_ip, r_type) \
					  values('%s', '%s', %d,'%s', %f, '%s', %d)",\
					  strTableName, szTime, strPathFileName,iGameId, strFileName, downInfo.m_fDownSpeed, downInfo.m_szIp, downInfo.m_enuDownFileState );
		g_sqlOperate.InsertInto(strSql);
		g_iTotalSum ++;

		CString strTemp;
		g_iLogFilePosition += len;
		strTemp.Format("%d",g_iLogFilePosition);
		WritePrivateProfileString("LogFileInfo","LogFilePositoin", strTemp, g_strServiceINIPath);
		
	}
	UnmapViewOfFile(pBuf);
	CloseHandle(hFileMap);
	SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
	SetEndOfFile(hFile);
	CloseHandle(hFile);

	return true;

}

bool LogFileIntoSqlDatabase(LPCSTR szLogFile)
{
	static bool bFirst = true;
	CDownFileInfo	downInfo;
	int len = sizeof(CDownFileInfo);
	memset(&downInfo, 0, len);
	FILE *pFile = NULL;

	if ((pFile = fopen((LPCTSTR)szLogFile,  "rb")) != NULL)
	{
		if (bFirst)
		{
			fseek(pFile, g_iLogFilePosition, SEEK_SET );
			bFirst = false;
		}
		typedef multimap<string, DownFileCount>DOWNFILEMAP;
		typedef DOWNFILEMAP::iterator DOWNFILEMAP_ITER;
		typedef pair<DOWNFILEMAP_ITER, DOWNFILEMAP_ITER> DownFile_Pair_Rang;

		DOWNFILEMAP			downFileMap;
		DOWNFILEMAP_ITER	iter_df1,iter_df2;
		DownFile_Pair_Rang	pairRang;

		//bool bInsert = false;
		int iKey = 0;
		DownFileCount downFileCount;
		while (fread(&downInfo, 1, len, pFile) == len)
		{
			//bInsert = false;
			iKey = 0;
			memset( &downFileCount, 0, sizeof(DownFileCount));
			//strcpy(downFiletemp.m_szFileName, downInfo.m_szFileName);
			downFileCount.m_st		   = downInfo.m_st;
			downFileCount.m_fDownSpeed = downInfo.m_fDownSpeed;
			downFileCount.m_iFrequency = 1;
			downFileCount.m_type		= downInfo.m_enuDownFileState;

			iter_df1 = downFileMap.find(downInfo.m_szFileName);
			if ( iter_df1 != downFileMap.end() )
			{
				float fspeed	=	iter_df1->second.m_fDownSpeed;
				int ifrequency	=	iter_df1->second.m_iFrequency;
				double dtotal   = fspeed * ifrequency;
				dtotal		+= downInfo.m_fDownSpeed;
				ifrequency	+= 1;
				fspeed	= dtotal/ifrequency;
				iter_df1->second.m_fDownSpeed = fspeed;
				iter_df1->second.m_iFrequency = ifrequency;		
			}
			else
			{
				string strTemp = downInfo.m_szFileName;	
				downFileMap.insert(make_pair(downInfo.m_szFileName, downFileCount ));
			}
			//*/
			g_iLogFilePosition += len;
			CString strTemp;
			strTemp.Format("%d",g_iLogFilePosition);
			WritePrivateProfileString("LogFileInfo","LogFilePositoin", strTemp, g_strServiceINIPath);
			//*/

		}
		fclose( pFile );
		for ( iter_df1 = downFileMap.begin(); iter_df1 != downFileMap.end(); iter_df1 ++ )
		{
			CString strSql;
			CString strPathFileName;
			CString strFileName;
			int iGameId = 0;
			int iFrequency = 0;
			float favespeed = 0;
			double dtolspeed = 0;
			strPathFileName = iter_df1->first.c_str();
			FindGameIDAndFileName(strPathFileName, strFileName, iGameId );
		
			char szbTime[50];
			char szETime[50];
			memset(szbTime, 0, sizeof(szbTime));
			sprintf(szbTime, "%d-%d-%d 0:00:00",iter_df1->second.m_st.wYear, iter_df1->second.m_st.wMonth, iter_df1->second.m_st.wDay);
			sprintf(szETime, "%d-%d-%d 23:59:59",iter_df1->second.m_st.wYear, iter_df1->second.m_st.wMonth, iter_df1->second.m_st.wDay);
			int itype = iter_df1->second.m_type;



			strSql.Format("select * from T_downFileInfo where r_PathFile = '%s' and r_Type =%d and r_datetime >='%s' and r_datetime<='%s'", \
				strPathFileName, itype, szbTime,szETime );
			_RecordsetPtr pRecordset = g_sqlOperate.SelectData(strSql);
			_variant_t var; 
			if( pRecordset !=NULL && !pRecordset->adoEOF )
			{
				int iKey = 0;
				var = pRecordset->GetCollect("r_id");
				if (var.vt != VT_NULL)
				{
					iKey = atoi((LPCSTR)_bstr_t(var) );
				}

				var = pRecordset->GetCollect("r_frequency");
				if(var.vt != VT_NULL)
				{
					iFrequency = atoi( (LPCSTR)_bstr_t(var) );
				}
				var = pRecordset->GetCollect("r_avespeed");
				if ( var.vt != VT_NULL )
				{
					favespeed = atof((LPCSTR)_bstr_t(var) );
				}
				dtolspeed = iFrequency * favespeed;
				dtolspeed += (iter_df1->second.m_iFrequency * iter_df1->second.m_fDownSpeed);
				iFrequency += iter_df1->second.m_iFrequency;
				favespeed = dtolspeed /iFrequency;

				strSql.Format("update T_downFileInfo set r_frequency = %d, r_avespeed = %f where r_id = %d",iFrequency, favespeed, iKey );
				g_sqlOperate.Update(strSql);
				//Log(g_szServiveLogFile,"LogFileIntoSqlDatabase, %s",strSql);
			}
			else
			{
				char szTime[50];
				memset(szTime, 0, sizeof(szTime));
				sprintf(szTime, "%d-%d-%d %d:%d:%d",\
					iter_df1->second.m_st.wYear, iter_df1->second.m_st.wMonth, iter_df1->second.m_st.wDay, \
					iter_df1->second.m_st.wHour,iter_df1->second.m_st.wMinute,iter_df1->second.m_st.wSecond);

				iFrequency = iter_df1->second.m_iFrequency;
				favespeed	= iter_df1->second.m_fDownSpeed;
				strSql.Format("insert into T_downFileInfo (r_datetime, r_PathFile, r_GameID, r_FileName, r_frequency, r_avespeed, r_type) \
							  values('%s', '%s', %d,'%s', %d, %f, %d)",\
							  szTime, strPathFileName,iGameId, strFileName, iFrequency, favespeed, itype );
				g_sqlOperate.InsertInto(strSql);
				//Log(g_szServiveLogFile,"LogFileIntoSqlDatabase, %s",strSql);
			}
		}
		return true;
	}
	return false;
}


//所有的日志
bool AutoToDB()
{
	TCHAR szFileName[_MAX_PATH];
	::GetModuleFileName(NULL, szFileName, _MAX_PATH);
	TCHAR	*p	= ::_tcsrchr(szFileName, _T('\\'));
	if(!p)
		p = ::_tcsrchr(szFileName, _T('/'));
	*++p = 0;

	GetPrivateProfileString("LogFileInfo","LogFileDirectory","",g_strLogFilePath.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strLogFilePath.ReleaseBuffer();

	GetPrivateProfileString("LogFileInfo","LastDate", "", g_strLastTime.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strLastTime.ReleaseBuffer();

	g_iLogFilePosition = GetPrivateProfileInt("LogFileInfo","LogFilePositoin", 0, g_strServiceINIPath);
	
	GetPrivateProfileString("LogFileInfo","TimeBlank","10",		g_strTimer.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strTimer.ReleaseBuffer( );

	GetPrivateProfileString("LogFileInfo","HomeDir","10",		g_strHomeDir.GetBuffer(_MAX_PATH),_MAX_PATH,g_strServiceINIPath);
	g_strHomeDir.ReleaseBuffer();
	g_strHomeDir.MakeLower();

	//////////////////////////////////////////////////////////////////////////

	CString strPath = _T("");
	int nYear = 0, nMonth = 0, nDay = 0;
	//int i = strlen(g_strLastTime);
	GetTimeFromStr(&nYear,&nMonth,&nDay,g_strLastTime);
	CTime timeStart(nYear,nMonth,nDay,0,0,0);
	CTime timeEnd = CTime::GetCurrentTime();
	CTimeSpan ts(1, 0, 0, 0);
	if ( g_sqlOperate.ConnectSQLDatabase((LPSTR)(LPCSTR)g_strServerName,(LPSTR)(LPCSTR)g_strDatabaseName, (LPSTR)(LPCSTR)g_strUserName, (LPSTR)(LPCSTR)g_strPassword ))
	{
		do 
		{
			CString strFilePath = _T("");
			CString strFileName = timeStart.Format(_T("%Y-%m-%d"));
			//int i = strlen(g_strLogFilePath);
			strFilePath.Format(_T("%s\\serverU%s%s"), g_strLogFilePath, strFileName, DAT_EXT);
			if (LogFileIntoSqlDatabase((LPCTSTR)strFilePath,"T_DownLog"))
			{
				g_strLastTime = timeStart.Format(_T("%Y%m%d"));
				WritePrivateProfileString("LogFileInfo","LastDate",	g_strLastTime, g_strServiceINIPath);
			}
			else
			{
				Log(g_szServiveLogFile,"日志文件错误");
			}
			g_iLogFilePosition = 0;
			timeStart += ts;
		}while (timeStart < timeEnd);
		g_sqlOperate.CloseSqlDatabase();
	}
	else
	{
		Log(g_szServiveLogFile,"数据库链接失败");
	}
	return false;
}

bool GetTimeFromStr(int* nYear ,int* nMonth,int* nDay,LPCSTR szTime)
{
	CString strTime = szTime;
	strTime.TrimLeft();
	strTime.TrimRight();

	//20081126
	if(strTime.GetLength()!=8)
		return FALSE;

	CString szTmp;

	szTmp = strTime.Mid(0,4);
	*nYear = atoi((char*)(LPCSTR)szTmp);

	szTmp = strTime.Mid(4,2);
	*nMonth = atoi((char*)(LPCSTR)szTmp);

	szTmp = strTime.Mid(6,2);
	*nDay = atoi((char*)(LPCSTR)szTmp);

	return true;
}

//  日志导入相关全局函数实现
BOOL Log(LPCTSTR lpLogFilePath,LPCTSTR lpszFormat, ...)
{
	try
	{
		TCHAR	szText[1024*2]		= { 0 };
		va_list	ap;
		va_start(ap, lpszFormat);
		::_vsntprintf(szText, 1024*2, lpszFormat, ap);
		va_end(ap);	

		FILE	*pFile	= NULL;	
		pFile	= ::_tfopen(lpLogFilePath,  _T("a+b"));
		if ( pFile != NULL )
		{
			::_ftprintf(pFile, _T("%s\r\n"), szText);
			fclose(pFile);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}


