////////////////////
//Game.h
/////////////////////
#pragma once

#include "main.h"
#include "UI\UI.h"
#include "PlayerManager.h"
#include "SkillManager.h"

class CWindow
{
public:
	CWindow()
	{
		m_bInitOver = false;
	}
	~CWindow(){}

public:

	HINSTANCE		m_hHinstance;
	HWND			m_hWND;

	WORD			m_wScreenWidth;
	WORD			m_wScreenHeight;
	WORD			m_wBPP;
	bool			m_bFullScreen;

	bool			m_bWindowClosed;

	bool			m_bInitOver;

private:

	POINT			m_MousePos;			//相对窗口坐标



public:

	void SetMousePos()
	{
		GetCursorPos(&m_MousePos);
		ScreenToClient(m_hWND,&m_MousePos);
	}
	void GetMousePos(LPPOINT pPoint)
	{
		pPoint->x = m_MousePos.x;
		pPoint->y = m_MousePos.y;
	}


};

class CGame : public CWindow
{
public:

	CGame(){}
	~CGame(){}

	HFONT			m_hFont[3];

	CCamera *		m_pCamera;
	CTerrain *		m_pTerrain;
	CSkyBox *		m_pSkyBox;

	CSkillManager * m_pSkillManager;
	CPlayerManager * m_pPlayerManager;
	CItemManager * m_pItemManager;
	

	CUI *			m_pUI;

	DWORD			m_dwStartTime;

private:

	DWORD			m_dwFrame;
	DWORD			m_dwFrameTime;
	WORD			m_wGameControl;
	WORD			m_wLoadingFlag;

public:

	bool Init( HINSTANCE hHinstance,HWND hWND,WORD wScreenWidth,WORD wScreenHeight,WORD wBPP,bool bFullScreen );
	void Release();
	bool MainLoop();
	void UpdateFrame();
	void Process();

	void SetGameControl( WORD wControl )
	{
		m_wGameControl = wControl;
	}
	WORD GetGameControl()
	{
		return m_wGameControl;
	}
	DWORD GetFrame()
	{
		return m_dwFrame;
	}

	CGameUI * GetGameUI()
	{
		return m_pUI->m_pGameUI;
	}


	bool GameLogin();
	bool GameNew();
	bool GameLoad();
	bool GameRun();
	bool GameOver();
	bool GameStory();
	bool GameInitLoading();
	bool GameEnterLoading();
	bool GameReturnLogin();

};

extern CGame * g_pGame;