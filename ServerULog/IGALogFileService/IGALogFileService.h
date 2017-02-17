#pragma once

#include "resource.h"
#include <WINSVC.H>


TCHAR					g_szServiceName[] = _T("IGALogFileService");
BOOL					g_bInstall;
SERVICE_STATUS_HANDLE	g_hServiceStatus;
SERVICE_STATUS			g_ServiceStatus;
DWORD					g_dwThreadID;
HANDLE					g_hThread;

//定义全局函数

void InitData();
BOOL IsInstalled();
BOOL InstallService();
BOOL UninstallService();
void LogEvent(LPCTSTR pszFormat, ...);

void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv);
void WINAPI ServiceStrl(DWORD dwOpcode);

// 日志导入相关函数定义
BOOL Log(LPCTSTR lpLogFilePath,LPCTSTR lpszFormat, ...);
BOOL LoadAllGameInfo();
BOOL FindFile();
BOOL AutoToDB();

BOOL GetSdkInfoFormLog(CString strFilePath);
BOOL GetHardwareInfoFromLog(CString strFilePath);
BOOL GetPlayInfoFormLog(CString strFilePath);
BOOL GetFocusInfoFormLog(CString strFilePath,int  LastUpdateInfoNum,int* lpAddCount);
void OnSdkInfo(CString strUpdatePath,int LastUpdateInfoNum,BOOL bWrite= FALSE,BOOL bCheck=FALSE) ; 
void OnHarewareInfo(CString strHardwareInfoPath,int LastHardwareInfoNum ,BOOL bWrite = FALSE,BOOL bCheck=FALSE);
void OnPlayInfor(CString strPlayInfoPath,int LastPlayInfoNum,BOOL bWrite= FALSE);
void OnFocusInfo(CString strFocusInfoPath,int LastFocusInfoNum,CTime time,BOOL bWrite = FALSE);


CString GetGameVersion(DWORD dwGameID);
void OnSetTimer();
void CALLBACK TimerProc( HWND hwnd,	UINT message,UINT idTimer,DWORD dwTime);
DWORD WINAPI ThreadProc(void *p);

