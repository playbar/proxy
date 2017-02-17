#include "StdAfx.h"
#include "FtpDownLog.h"
#include "ServerULog.h"
#include "IniFile.h"
#include "SystemTime.h"
#include <strsafe.h>

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
using namespace std;


extern TCHAR g_szIniFileName[256];
CFtpDownLog g_FtpDownLog;

CFtpDownLog::CFtpDownLog(void)
{
	 InitializeCriticalSection( &m_cs );
	
}

CFtpDownLog::~CFtpDownLog(void)
{
	DeleteCriticalSection( &m_cs );  
}



bool CFtpDownLog::StartDownFile(RFTPEventStr* pBeforEvent)
{
	if (EVNT_StartDown == pBeforEvent->Event)
	{
		bool bIsExist = false;
		for ( m_iterDownlist = m_DownList.begin(); m_iterDownlist != m_DownList.end();  m_iterDownlist ++)
		{
			if ( ((DownFileInfoAndID )* m_iterDownlist).m_SessionID == pBeforEvent->SessionID )
			{
				bIsExist = true;
				break;
			}
		}
		if ( ! bIsExist )
		{
			DownFileInfoAndID downFileInfo;
			memset(&downFileInfo, 0, sizeof(downFileInfo));
			GetLocalTime( &downFileInfo.m_DownFileInfo.m_st);
			strcpy(downFileInfo.m_DownFileInfo.m_szFileName, pBeforEvent->AuxOne);
			strcpy(downFileInfo.m_DownFileInfo.m_szIp, pBeforEvent->ClientIP);
			downFileInfo.m_DownFileInfo.m_fDownSpeed = 0;
			downFileInfo.m_DownFileInfo.m_enuDownFileState = DOWNFILE_FAILE;
			downFileInfo.m_SessionID = pBeforEvent->SessionID;
			//downFileInfo.m_dwBegin=GetTickCount();
			m_DownList.push_back( downFileInfo);
		}
		return true;
	}
	return false;
}

bool CFtpDownLog::EndDownFile(RFTPEventStr* pAfterStruc)
{
	//OutputDebugString(L"CFtpDownLog::EndDownFile");
	if ( EVNT_EndDown == pAfterStruc->Event )
	{

		for ( m_iterDownlist = m_DownList.begin(); m_iterDownlist != m_DownList.end();  m_iterDownlist ++ )
		{
			if ( ((DownFileInfoAndID )* m_iterDownlist).m_SessionID == pAfterStruc->SessionID )
			{
				try
				{
					memset(&m_downFileInfo, 0, sizeof(m_downFileInfo));
					m_downFileInfo.m_st = ((DownFileInfoAndID )* m_iterDownlist).m_DownFileInfo.m_st;
					strcpy(m_downFileInfo.m_szFileName, ((DownFileInfoAndID )* m_iterDownlist).m_DownFileInfo.m_szFileName);
					strcpy(m_downFileInfo.m_szIp, ((DownFileInfoAndID )* m_iterDownlist).m_DownFileInfo.m_szIp);
					m_downFileInfo.m_fDownSpeed = (float)pAfterStruc->Size /(float) pAfterStruc->Duration;
					m_downFileInfo.m_enuDownFileState = DOWNFILE_SUCCESS;
					if ( WriteToFile())
					{
						m_DownList.erase(m_iterDownlist);
					}
				}
				catch(...)
				{
					LogToFile("EndDownFile");

				}
				
				break;
			}
		}
		return false;
	}
	return false;
}

bool CFtpDownLog::CloseDownd(RFTPEventStr* pCloseStruc)
{
	if ( EVNT_Close == pCloseStruc->Event )
	{
		for ( m_iterDownlist = m_DownList.begin(); m_iterDownlist != m_DownList.end();  m_iterDownlist ++ )
		{
			try
			{
				if ( ((DownFileInfoAndID )* m_iterDownlist).m_SessionID == pCloseStruc->SessionID )
				{
					memset(&m_downFileInfo, 0, sizeof(m_downFileInfo));
					m_downFileInfo.m_st = ((DownFileInfoAndID )* m_iterDownlist).m_DownFileInfo.m_st;
					strcpy(m_downFileInfo.m_szFileName, ((DownFileInfoAndID )* m_iterDownlist).m_DownFileInfo.m_szFileName);
					strcpy(m_downFileInfo.m_szIp, ((DownFileInfoAndID )* m_iterDownlist).m_DownFileInfo.m_szIp);
					m_downFileInfo.m_fDownSpeed = (float)pCloseStruc->Size /(float) pCloseStruc->Duration;
					m_downFileInfo.m_enuDownFileState = DOWNFILE_FAILE;
					if ( WriteToFile())
					{
						m_DownList.erase(m_iterDownlist);
					}
				}
			}
			catch(...)
			{

				LogToFile("CloseDownd");
			}
		
		}
		return true;
	}
	return false;
}

bool CFtpDownLog::WriteToFile()
{
	int iTemplen = 0;
	CIniFile iniFile;
	iniFile.SetIniFileName( g_szIniFileName);
	TCHAR szLogPathW[MAX_PATH * sizeof(TCHAR)];
	iniFile.GetString(TEXT("LogFile"),TEXT("FilePath"),szLogPathW, sizeof(szLogPathW)/sizeof(szLogPathW[0]));
	iTemplen = wcslen(szLogPathW);
	if (szLogPathW[iTemplen -1] == '\\')
	{
		szLogPathW[iTemplen - 1] = '\0';
	}

	TCHAR szLogFile[MAX_PATH];
	CSystemTime st;
	st.GetTodayDate(szLogFile, sizeof(szLogFile)/ sizeof(szLogFile[0]));
	StringCchPrintf(szLogPathW, (sizeof(szLogPathW)/sizeof(szLogPathW[0])) ,TEXT("%s\\serverU%s.dat"),szLogPathW, szLogFile);

	char szLogFileA[MAX_PATH];
	WCharToMByte(szLogPathW, szLogFileA, MAX_PATH);

	iTemplen = sizeof(m_downFileInfo);
	FILE *pLogFile = fopen(szLogFileA,"a+b");
	if (pLogFile == NULL)
	{
		return false;
	}
	size_t slen = 0;
	slen = fwrite(&m_downFileInfo,iTemplen, 1, pLogFile);
	fclose(pLogFile);
	return true;
}


/*
*如果有过滤文件，返回true；没有返回false
*/
bool CFtpDownLog::FilterFile(RFTPEventStr* pStruc)
{
	CIniFile iniFile;
	iniFile.SetIniFileName( g_szIniFileName );
	TCHAR szLogPathW[MAX_PATH * sizeof(TCHAR)];
	iniFile.GetString(TEXT("LogFile"),TEXT("FilePath"),szLogPathW, sizeof(szLogPathW)/sizeof(szLogPathW[0]));
	int iFile=0;
	TCHAR szFilterFile[MAX_PATH];

	list <string> lsFilterFile;
	list<string>::iterator ite_ls;

	while ( true )
	{
		iFile++;
		swprintf(szFilterFile, TEXT("FilterFile%d"),iFile);
		iniFile.GetString(TEXT("LogFile"),szFilterFile,szLogPathW, sizeof(szLogPathW)/sizeof(szLogPathW[0]));
		char szLogFileA[MAX_PATH];
		WCharToMByte(szLogPathW, szLogFileA, MAX_PATH);
		CharLowerA(szLogFileA);
		if (  strlen(szLogFileA) == 0)
		{
			break;
		}
		else
		{
			lsFilterFile.push_back(szLogFileA);
		}
	}

	for ( ite_ls = lsFilterFile.begin(); ite_ls != lsFilterFile.end(); ite_ls++ )
	{
		//char szTemp[MAX_PATH];
		//memset(szTemp, 0, MAX_PATH );
		//memcpy(szTemp, ite_ls->c_str(), MAX_PATH);
		//CharLowerA(szTemp);
		CharLowerA(pStruc->AuxOne);
		if ( strcmp(ite_ls->c_str(), pStruc->AuxOne) == 0 )
		{
			return true;
		}
	}
	return false;
}


void LogToFile(CString szLog)
{
	FILE *pLogFile = fopen("c:\\serverUlog.txt","a+");
	if (pLogFile == NULL)
	{
		return;
	}
	size_t slen = 0;
	slen = fwrite(szLog,szLog.GetLength(), 1, pLogFile);
	slen = fwrite("\r\n", 2, 1, pLogFile );
	fclose(pLogFile);
}
