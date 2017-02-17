

//#include "../NetApp.h"
#include "LoginUI.h"
#include "UI.h"



bool CLoginUI::Init()
{

	//创建LoginUI所需纹理
	static int i = 0;
	while( i < UI_TEXTURE_ID::MAX_LOGINUI_TEX_NUM )
	{
		CTexture * pTexture = new CTexture;
		pTexture->CreateFromFile( GetLoginUITexturePath( i ), false );
		_ASSERT( pTexture );
		m_pTextures.AddItem( pTexture );
		++ i;
		g_pUI->m_pLoadingUI->UpdateLoading( 40.f / UI_TEXTURE_ID::MAX_COMMON_TEX_NUM );
		return false;
	}
	

	//UI初始化
	m_pLogUI = new CUILog;
	m_pLogUI->Init( UI_LOGIN_DIALOG_LOGIN, 0.f,0.f,1024.f, 768.f,true, this);
	m_pDialogs[UI_LOGIN_DIALOG_LOGIN] = m_pLogUI;

	i = 0;
	return true;
}


void CLoginUI::Release()
{
	for( int i = 0; i < MAX_LOGIN_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			m_pDialogs[i]->Release();
			delete m_pDialogs[i];
			m_pDialogs[i] = NULL;
		}
	}

	for( int i = 0; i < UI_TEXTURE_ID::MAX_LOGINUI_TEX_NUM; i ++ )
	{
		SAFE_DELETE( m_pTextures[i] );
	}
}

bool CLoginUI::Process( float x, float y )
{
	//GetCursorPos( &m_MousePos );
	bool bFlag = false;
	for( int i = 0; i < MAX_LOGIN_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			if( m_pDialogs[i]->Process( x, y ) )
				bFlag = true;
		}
	}
	return bFlag;
}

void CLoginUI::Render()
{
	//g_pD3D->GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

	for( int i = 0; i < MAX_LOGIN_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			m_pDialogs[i]->Render();
		}
	}

	g_pMyD3D->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}


bool CLoginUI::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	for( int i = 0; i < MAX_LOGIN_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			if( m_pDialogs[i]->WndProc( msg, wParam, lParam ) )
				return true;
		}
	}
	return false;
}

bool CLoginUI::Event( ST_UI_MSG * pMsg )
{
	for( int i = 0; i < MAX_LOGIN_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			if( m_pDialogs[i]->Event( pMsg ) )
				return true;
		}
	}
	return false;
}