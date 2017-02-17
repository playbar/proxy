#pragma once

#include <atlstr.H>
#include <strsafe.h>
#include <assert.h>
#include <atltime.h>


SERVICE_STATUS_HANDLE	g_hServiceStatus;
SERVICE_STATUS			g_ServiceStatus;

BOOL					g_bInstall = false;
DWORD					g_dwThreadID = 0;
HANDLE					g_hThread = NULL;

#define	ID_TIMER1 1
#define ID_TIMER2 2

#define DAT_EXT			_T(".dat")


enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};

typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					// 系统当前时间
	char			m_szFileName[MAX_PATH]; //下载文件名
	char			m_szIp[16];				// 客户端IP
	float			m_fDownSpeed;			//下载速度
	DOWNFILE_STATE	m_enuDownFileState;		// 是否成功下载
} CDownFileInfo;

typedef struct DOWNFILECOUNT_TYP
{
	//char m_szFileName[MAX_PATH];
	SYSTEMTIME	m_st;
	int			m_iFrequency;
	float		m_fDownSpeed;
	int			m_type;
}DownFileCount;

//定义全局函数

void InitData();
BOOL IsInstalled();
BOOL InstallService();
BOOL UninstallService();
void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv);
void WINAPI ServiceStrl(DWORD dwOpcode);

void LogEvent(LPCTSTR pszFormat, ...);


// 日志导入相关函数定义
BOOL Log(LPCTSTR lpLogFilePath,LPCTSTR lpszFormat, ...);
void OnSetTimer();
void CALLBACK TimerProc( HWND hwnd,	UINT message,UINT idTimer,DWORD dwTime);
DWORD WINAPI ThreadProc(void *p);

bool AutoToDB();
bool LogFileIntoSqlDatabase(LPCSTR szLogFile);
bool GetTimeFromStr(int* nYear ,int* nMonth,int* nDay,LPCSTR szTime);
bool FindGameIDAndFileName(CString & strSrc, CString &strFileName, int & iGameId);
bool LogFileIntoSqlDatabase(LPCSTR szLogFile,CString strTableName);


