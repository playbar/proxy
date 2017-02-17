#pragma once

/***********************************************************************
* Module:  CPlayFlashWindow.h
* Author:  Administrator
* Modified: 2008年11月5日 10:31:53
* Purpose: Declaration of the class CPlayFlashWindow
* Comment: 播放flash窗口
***********************************************************************/

#include "KFlash.h"
typedef unsigned int UINT;

class CPlayFlashWindow
{
public:
	CPlayFlashWindow();
	~CPlayFlashWindow();
public:
	bool DownFlash(void);
	bool UpFlash(void);
	int PlayFlash(void);
	int PausePlayFlash(void);
	int ContinuePlayFlash(void);
	static UINT CreateFlashThread(PVOID pParam);

protected:
private:
	int m_ixPos;
	int m_iyPos;
	int m_iWidth;
	int m_iHeight;
	int m_iPlayingTime;
	int m_iPlayCount;
	bool m_bDown;
	bool m_bUp;
	char m_szFlashName[256];

	KFlash    m_Flash;


};


class CBeforGame : public CPlayFlashWindow
{
public:
	UINT CreateFlashThread(void);

protected:
private:

};


class CAfterGame : public CPlayFlashWindow
{
public:
	UINT CreateFlashThread(void);

protected:
private:

};