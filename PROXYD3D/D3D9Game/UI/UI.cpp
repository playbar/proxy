
#include "UI.h"

CUI * g_pUI;


bool CUI::Init()
{
	//创建common纹理
	static int i = 0;
	while( i < UI_TEXTURE_ID::MAX_COMMON_TEX_NUM )
	{
		CTexture * pTexture = new CTexture;
		pTexture->CreateFromFile( GetCommonTexturePath( i ), false );
			_ASSERT( pTexture );
		m_ArrCommonTextures.AddItem( pTexture );
		++ i;
		m_pLoadingUI->UpdateLoading( 50.f / UI_TEXTURE_ID::MAX_COMMON_TEX_NUM );
		return false;
	}

	//UI
	if( !m_pLoginUI->Init() )
		return false;

	m_dwActivateId = BASEUI_LOGIN;

	m_bInitOver = true;
	m_pLoadingUI->SetActivate( false );

	return true;
}

void CUI::Release()
{
	for( int i = 0; i < MAX_BASEUI_NUM; i ++ )
	{
		SAFE_RELEASE( m_pBaseUI[i] );
		SAFE_DELETE( m_pBaseUI[i] );
	}

	SAFE_RELEASE( m_pLoadingUI );
	SAFE_DELETE( m_pLoadingUI );
	SAFE_RELEASE( m_pMouseUI );
	SAFE_DELETE( m_pMouseUI );

	for( int i = 0; i < UI_TEXTURE_ID::MAX_COMMON_TEX_NUM; i ++ )
	{
		SAFE_DELETE( m_ArrCommonTextures[i] );
	}
}


bool CUI::Process( float x, float y )
{
	m_pLoadingUI->Process( x, y );
	m_pMouseUI->Process( x, y );

	if( !m_bInitOver )
		return false;
	if( m_ArrUIMsg.Num() > 0 )
	{
		if( m_ArrUIMsg[0]->dwDialogId == UI_DIALOG_MOUSE )
		{
			if( m_pMouseUI->Event( m_ArrUIMsg[0] ) )
			{
				delete m_ArrUIMsg[0];
				m_ArrUIMsg.Remove( 0 );
			}
		}
	}
	if( m_ArrUIMsg.Num() > 0 )
	{
		//事件
		if (m_pBaseUI[(int)m_dwActivateId] == NULL)
		{
			delete m_ArrUIMsg[0];
			m_ArrUIMsg.Remove( 0 );
			return false;
		}
		else
		{
			m_pBaseUI[(int)m_dwActivateId]->Event( m_ArrUIMsg[0] );
			delete m_ArrUIMsg[0];
			m_ArrUIMsg.Remove( 0 );
		}
		
	}
	if (m_pBaseUI[(int)m_dwActivateId] == NULL)
	{
		return false;
	}

	return m_pBaseUI[(int)m_dwActivateId]->Process( x, y );
}

void CUI::Render()
{
	if( m_pLoadingUI->GetActivate() )
		m_pLoadingUI->Render();
	else
	{
		if (m_pBaseUI[(int)m_dwActivateId] != NULL)
		{
			m_pBaseUI[(int)m_dwActivateId]->Render();
		}
	}
	m_pMouseUI->Render();
}