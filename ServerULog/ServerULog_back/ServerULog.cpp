// ServerULog.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "FtpDownLog.h"
#include "ServerULog.h"

WORD CALLBACK HandleEventHook(RFTPEventStr* pEventStruc)
{
	/*
	if (true )
	{
	TCHAR szTemp[256];
	swprintf(szTemp, TEXT("Event = %d , SessionID = %d,Size = %d,Duration = %d\n"), pEventStruc->Event, pEventStruc->SessionID,pEventStruc->Size, pEventStruc->Duration );
	OutputDebugString(szTemp);
	}
	//*/
	try
	{
		if (EVNT_StartDown == pEventStruc->Event )
		{
			g_FtpDownLog.FtpEnterCriticalSection();
			if (g_FtpDownLog.FilterFile(pEventStruc))
			{
				return REVNT_None;
			}

			g_FtpDownLog.StartDownFile(pEventStruc);
			g_FtpDownLog.FtpLeaveCriticalSection();
		}
		else if ( EVNT_EndDown == pEventStruc->Event )
		{
			g_FtpDownLog.FtpEnterCriticalSection();
			if (g_FtpDownLog.FilterFile(pEventStruc))
			{
				return REVNT_None;
			}

			g_FtpDownLog.EndDownFile(pEventStruc);
			g_FtpDownLog.FtpLeaveCriticalSection();
		}
		else if ( EVNT_Close == pEventStruc->Event )
		{
			g_FtpDownLog.FtpEnterCriticalSection();
			if (g_FtpDownLog.FilterFile(pEventStruc))
			{
				return REVNT_None;
			}
			g_FtpDownLog.CloseDownd(pEventStruc);
			g_FtpDownLog.FtpLeaveCriticalSection();
		}
	}
	catch (...)
	{
		LogToFile("HandleEventHook");
	}
	
	return REVNT_None;
}


int __cdecl HandleClientEvent(RClientEventStr* pClientEvent)
{
	//OutputDebugString(L"server u HandleClientEvent");
	return 0;
}
