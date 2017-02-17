
#pragma once

#include "GameUI.h"
#include "LoginUI.h"
#include "LoadingUI.h"
#include "MouseUI.h"



//×ÜUIÀà
class CUI
{
public:
	CUI()
	{
		m_pDevice = g_pMyD3D->GetDevice();
		m_dwActivateId = BASEUI_LOGIN;
		for( int i = 0; i < MAX_BASEUI_NUM; i ++ )
		{
			m_pBaseUI[i] = NULL;
		}
		m_pGameUI = NULL;
		m_pLoginUI = NULL;
		m_bInitOver = false;
		m_pLoadingUI = new CUILoading;
		m_pLoadingUI->Init( UI_DIALOG_LOADING ,0.f,0.f,800.f,600.f, true, NULL);
		m_pMouseUI = new CUIMouse;
		m_pMouseUI->Init( UI_DIALOG_MOUSE,0.f,0.f,0.f,0.f, true, NULL );
		m_pLoginUI = new CLoginUI;
		m_pBaseUI[BASEUI_LOGIN] = m_pLoginUI;
	}
	~CUI()
	{
		//delete m_pLoadingUI;
		//m_pLoadingUI = NULL;
		
		//delete m_pMouseUI;
		//m_pMouseUI = NULL;

		//delete m_pLoginUI;
		//m_pLoginUI = NULL;
	}

	CBaseUI *		m_pBaseUI[MAX_BASEUI_NUM];
	DWORD			m_dwActivateId;

	CGameUI *		m_pGameUI;
	CLoginUI *		m_pLoginUI;


	CUILoading *	m_pLoadingUI;
	CUIMouse *		m_pMouseUI;


	Array<ST_UI_MSG *>		m_ArrUIMsg;

	Array<CTexture *>		m_ArrCommonTextures;


	bool					m_bInitOver;

protected:

	LPDIRECT3DDEVICE9					m_pDevice;

public:

	bool Init();

	void Release();

	void Render();

	bool Process( float x, float y );

	bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
	{
		switch( msg )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			{
				ST_UI_MSG * pMsg = new ST_UI_MSG;
				pMsg->dwDialogId = UI_DIALOG_MOUSE;
				pMsg->msg = msg;
				pMsg->wParam = wParam;
				pMsg->lParam = lParam;
				m_pMouseUI->SendMsg( pMsg );
			}
		}
		if( m_pBaseUI[(int)m_dwActivateId] )
			return m_pBaseUI[(int)m_dwActivateId]->WndProc( msg, wParam, lParam );
		else
			return false;
	}

	bool SetActivateId( DWORD dwId )
	{
		static int i = 0;
		++ i;
		if( i == 1 )
		{
			m_pBaseUI[(int)m_dwActivateId]->Release();
			SAFE_DELETE( m_pBaseUI[(int)m_dwActivateId] );
			m_dwActivateId = dwId;
		}
		if( dwId == BASEUI_GAME )
		{
			if( i == 1 )
				m_pGameUI = new CGameUI;
			if( m_pGameUI->Init() )
			{
				m_pBaseUI[(int)m_dwActivateId] = m_pGameUI;
				m_pLoginUI = NULL;
				i = 0;
				return true;
			}
			else
				return false;
		}
		else if( dwId == BASEUI_LOGIN )
		{
			if( i == 1 )
				m_pLoginUI = new CLoginUI;
			if( m_pLoginUI->Init() )
			{
				m_pBaseUI[(int)m_dwActivateId] = m_pLoginUI;
				m_pGameUI = NULL;
				i = 0;
				return true;
			}
			else
				return false;
		}
		else
			return true;
	}

};

extern CUI * g_pUI;