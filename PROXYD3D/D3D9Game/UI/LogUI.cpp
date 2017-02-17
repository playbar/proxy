
//#include "../NetApp.h"
#include "LogUI.h"
#include "UI.h"
#include "../Game.h"




void CUILog::Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	//m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pStatic = AddStatic( UI_LOGIN_LOG_CONTROL_STATIC_BACK , 0.f, 0.f, width, height, true );
	pStatic->SetTexture( pBaseUI->m_pTextures[UI_TEXTURE_ID::LOGINUI_TEX_LOGIN_BACK] );
	pStatic->SetUV( 0.f, 0.f, 800.f / 1024.f, 600.f / 1024.f );
	pStatic->SetTextColor( 0xffff0000 );
	pStatic->SetColor( 0xff000000 );
	pStatic->m_dwTextFlag = DT_CENTER;

	pStatic = AddStatic( UI_LOGIN_LOG_CONTROL_STATIC_LOG, 325.f, 120.f, 150.f, 200.f, true );
	pStatic->SetTexture( pBaseUI->m_pTextures[UI_TEXTURE_ID::LOGINUI_TEX_LOGIN_LOG] );
	pStatic->SetUV( 0.f, 0.f, 150.f / 256.f, 200.f / 256.f );
	pStatic->SetColor( 0xff000000 );

	CUIEditBox * pEditBox = AddEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID, 368.f, 238.f, 64.f, 12.f, true );
	//pEditBox->SetTexture( g_pGame->m_pUI->m_pTextures[UI_TEX_LOGIN_LOG] );
	pEditBox->SetTextColor( 0xff000000 );
	pEditBox->m_bFocus = true;
	
	pEditBox = AddEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD, 368.f, 286.f, 64.f, 12.f, true );
	//pEditBox->SetTexture( g_pGame->m_pUI->m_pTextures[UI_TEX_LOGIN_LOG] );
	pEditBox->SetTextColor( 0xff000000 );
	pEditBox->m_bFocus = false;
	pEditBox->m_bPassword = true;


	CUIButton * pButton = AddButton( UI_LOGIN_LOG_CONTROL_BUTTON_LOG, 320.f, 330.f, 75.f, 25.f, true );
	pButton->InitTexture( g_pUI->m_ArrCommonTextures[UI_TEXTURE_ID::COMMON_TEX_BUTTON_OK_NORMAL], g_pUI->m_ArrCommonTextures[UI_TEXTURE_ID::COMMON_TEX_BUTTON_OK_ON], g_pUI->m_ArrCommonTextures[UI_TEXTURE_ID::COMMON_TEX_BUTTON_OK_DOWN] );
	pButton->SetColor( 0xff000000 );

	pButton = AddButton( UI_LOGIN_LOG_CONTROL_BUTTON_EXIT, 405.f, 330.f, 75.f, 25.f, true );
	pButton->InitTexture( g_pUI->m_ArrCommonTextures[UI_TEXTURE_ID::COMMON_TEX_BUTTON_CANCEL_NORMAL], g_pUI->m_ArrCommonTextures[UI_TEXTURE_ID::COMMON_TEX_BUTTON_CANCEL_ON], g_pUI->m_ArrCommonTextures[UI_TEXTURE_ID::COMMON_TEX_BUTTON_CANCEL_DOWN] );
	pButton->SetColor( 0xff000000 );
}

void CUILog::Release()
{
	CUIDialog::Release();
}


bool CUILog::Process( float x, float y )
{
	return CUIDialog::Process( x, y );
}

void CUILog::Render()
{
	CUIDialog::Render();
}

bool CUILog::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !m_bActivate )
		return false;
	switch( msg )
	{
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		{
			ST_UI_MSG * pMsg = new ST_UI_MSG;
			pMsg->dwDialogId = m_dwId;
			pMsg->msg = msg;
			pMsg->wParam = wParam;
			pMsg->lParam = lParam;
			SendMsg( pMsg );
			return true;
		}
	}
	return false;
}

bool CUILog::Event( ST_UI_MSG * pMsg )
{
	if( !m_bActivate )
		return false;

	switch( pMsg->msg )
	{
	case WM_CHAR:
		{
			CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
			_ASSERT( pControl );

			if( pControl->m_bFocus )
			{
				OnCharId( pMsg->wParam );
				return true;
			}

			pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
			_ASSERT( pControl );
			
			if( pControl->m_bFocus )
			{
				OnCharPassword( pMsg->wParam );
				return true;
			}
		}
		return false;
	case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
			case VK_TAB:
				{
					OnKeyDownTab();
					return true;
				}
			case VK_RETURN:
				{
					CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
					_ASSERT( pControl );

					if( pControl->m_bFocus )
						OnLButtonDownPassword();
					else
						OnLogin();
					return true;
				}
			}
		}
		return false;
	case WM_LBUTTONDOWN:
		{
			int x = LOWORD( pMsg->lParam );
			int y = HIWORD( pMsg->lParam );

			CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
			_ASSERT( pControl );

			RECT rect;
			pControl->GetRect( &rect );
			if( APP_Tools::appInRect( x, y, rect ) )
			{
				OnLButtonDownId();
				return true;
			}

			pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
			_ASSERT( pControl );

			pControl->GetRect( &rect );
			if( APP_Tools::appInRect( x, y, rect ) )
			{
				OnLButtonDownPassword();
				return true;
			}
			

			/*CUIButton * pButton = GetButton( UI_LOGIN_LOG_CONTROL_BUTTON_LOG );
			_ASSERT( pButton );

			pButton->GetRect( &rect );
			if( APP::appInRect( x, y, rect ) )
			{
				pButton->SetDown();
				return true;
			}

			pButton = GetButton( UI_LOGIN_LOG_CONTROL_BUTTON_EXIT );
			_ASSERT( pButton );
			
			pButton->GetRect( &rect );
			if( APP::appInRect( x, y, rect ) )
			{
				pButton->SetDown();
				return true;
			}*/
		}
		return false;
	case WM_LBUTTONUP:
		{
			int x = LOWORD( pMsg->lParam );
			int y = HIWORD( pMsg->lParam );

			//ÅÐ¶ÏµÇÂ½
			//CUIButton * pButton = GetButton( UI_LOGIN_LOG_CONTROL_BUTTON_LOG );
			//_ASSERT( pButton );

			//RECT rect;
			//pButton->GetRect( &rect );
			//if( APP::appInRect( x, y, rect ) )
			//{
			//	if( pButton->m_bMouseFlag == BUTTON_MOUSE_DOWN )
			//	{
			//		OnLogin();
			//		pButton->SetOn();
			//		//g_pGame->m_pUI->SetActivateId( BASEUI_GAME );
			//		//g_pGame->SetGameControl( GAME_LOAD );
			//		//m_bActivate = false;
			//		//CUIDialog * pUI = ((CGameUI *)m_pBaseUI)->GetDialog( UI_LOGIN_DIALOG_SELSERVER );
			//		//_ASSERT( pUI );
			//		//pUI->SetActivate( true );
			//		return true;
			//	}
			//}
			//pButton->SetOn();


			////ÅÐ¶ÏÍË³ö
			//pButton = GetButton( UI_LOGIN_LOG_CONTROL_BUTTON_EXIT );
			//_ASSERT( pButton );

			//pButton->GetRect( &rect );
			//if( APP::appInRect( x, y, rect ) )
			//{
			//	if( pButton->m_bMouseFlag == BUTTON_MOUSE_DOWN )
			//	{
			//		PostQuitMessage( 0 );
			//		return true;
			//	}
			//}
			//pButton->SetOn();
		}
		return false;
	case BUTTON_CLICK:
		{
			switch( pMsg->dwControlId )
			{
			case UI_LOGIN_LOG_CONTROL_BUTTON_LOG:
				{
					OnLogin();
					return true;
				}
			case UI_LOGIN_LOG_CONTROL_BUTTON_EXIT:
				{
					PostQuitMessage( 0 );
					return true;
				}
			}
		}
		return false;
	}
	return false;
}


void CUILog::OnCharId( WPARAM wParam )
{
	CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
	_ASSERT( pControl );
	pControl->OnChar( wParam );

}

void CUILog::OnCharPassword( WPARAM wParam )
{
	CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
	_ASSERT( pControl );
	pControl->OnChar( wParam );
}

void CUILog::OnKeyDownTab()
{
	CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
	_ASSERT( pControl );
	pControl->m_bFocus = !pControl->m_bFocus;

	pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
	_ASSERT( pControl );
	pControl->m_bFocus = !pControl->m_bFocus;
}

void CUILog::OnLButtonDownId()
{
	CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
	_ASSERT( pControl );
	pControl->m_bFocus = true;

	pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
	_ASSERT( pControl );
	pControl->m_bFocus = false;
}

void CUILog::OnLButtonDownPassword()
{
	CUIEditBox * pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
	_ASSERT( pControl );
	pControl->m_bFocus = true;

	pControl = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
	_ASSERT( pControl );
	pControl->m_bFocus = false;
}

void CUILog::OnLogin()
{
	CUIEditBox * pEditBoxId = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_ID );
	CUIEditBox * pEditBoxPsw = GetEditBox( UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD );
	_ASSERT( pEditBoxId );
	_ASSERT( pEditBoxPsw );
	TCHAR * pId = pEditBoxId->GetText();

	CUIStatic * pStatic = GetStatic( UI_LOGIN_LOG_CONTROL_STATIC_BACK );
	_ASSERT( pStatic );
	if( _tcscmp( pId, L"" ) == 0 )
	{
		pStatic->SetText( L"ÇëÊäÈëÓÃ»§Ãû£¡" );
		return;
	}
	TCHAR * pPsw = pEditBoxPsw->GetText();
	if( _tcscmp( pPsw, L"" ) == 0 )
	{
		pStatic->SetText( L"ÇëÊäÈëÃÜÂë£¡" );
		return;
	}

	g_pGame->SetGameControl(GAME_ENTER_LOADING);
	g_pGame->m_pUI->m_pLoadingUI->SetLoading(0.f);
	g_pGame->m_pUI->m_pLoadingUI->SetActivate(true);
}
