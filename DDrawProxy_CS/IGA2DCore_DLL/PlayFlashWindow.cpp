//#include "stdafx.h"
#include "PlayFlashWindow.h"

/***********************************************************************
* Module:  CPlayFlashWindow.cpp
* Author:  Administrator
* Modified: 2008年11月5日 10:31:53
* Purpose: Implementation of the class CPlayFlashWindow
* Comment: 播放flash窗口
***********************************************************************/
extern HANDLE g_hevtPlayFlashEvent;

CPlayFlashWindow::CPlayFlashWindow()
{
	::CoInitialize(NULL);				
	AtlAxWinInit();
	IGAWinInit();

	m_iHeight = 300;
	m_iWidth = 400;
	strcpy(m_szFlashName,"c:\\001.swf");
}

CPlayFlashWindow::~CPlayFlashWindow()
{
	::CoUninitialize();
}

bool CPlayFlashWindow::DownFlash(void)
{
	return false;
	// TODO : implement
}

bool CPlayFlashWindow::UpFlash(void)
{
	return false;
	// TODO : implement
}


int CPlayFlashWindow::PlayFlash(void)
{
	m_Flash.Create(NULL);
	m_Flash.CenterWindow(m_iWidth,m_iHeight);
	
	if  (m_Flash.Play(m_szFlashName, m_iPlayingTime ))
	{
		MSG  msg;
		while(  ::GetMessage(&msg,m_Flash.GetHwnd(),0,0) )
		{
			::DispatchMessage(&msg);
			if  ( m_Flash.m_bStop  )
			{
				m_Flash.Close();
				m_Flash.DestroyWindow();
				SetEvent(g_hevtPlayFlashEvent);
				::DispatchMessage(&msg);
				break;
			}
		}
	}
	return 0;
	
}

int CPlayFlashWindow::PausePlayFlash(void)
{
	// TODO : implement
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Name:       CPlayFlashWindow::ContinuePlayFlash()
// Purpose:    Implementation of CPlayFlashWindow::ContinuePlayFlash()
// Return:     int
////////////////////////////////////////////////////////////////////////

int CPlayFlashWindow::ContinuePlayFlash(void)
{
	// TODO : implement
	return 0;
}

UINT CPlayFlashWindow::CreateFlashThread(PVOID pParam)
{
	// TODO : implement
	CPlayFlashWindow * playFlash = (CPlayFlashWindow *)pParam;
	playFlash->PlayFlash();
	return 0;
}


UINT CBeforGame::CreateFlashThread(void)
{
	// TODO : implement
	return 0;
}


UINT CAfterGame::CreateFlashThread(void)
{
	// TODO : implement
	return 0;
}