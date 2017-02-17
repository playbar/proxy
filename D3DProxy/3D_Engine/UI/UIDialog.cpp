
#include "UIDialog.h"
#include "UI.h"


void CUIControl::ShowText()
{
	if( _tcscmp( L"", m_szText ) == 0 )
		return;

	m_pVFont->Text( (int)(m_fX + m_fBWidth), (int)(m_fY + m_fBHeight), (int)(m_fWidth - m_fBWidth), (int)(m_fHeight - m_fBHeight), m_szText, m_dwTextColor, m_dwTextFlag );
}

/////////////////////////////////////////////////////////////////////////////////////////
//CUIDialog

bool CUIDialog::Process( float x, float y ) //相对屏幕坐标
{
	if( !m_bActivate )
		return false;

	//if( !APP::appInRect( (int)x, (int)y, (int)m_fX, (int)m_fY, (int)m_fWidth, (int)m_fHeight ) )
	//	return false;

	bool bFlag = false;
	for(int i=0;i<m_pControls.Num();i++)
	{
		if( m_pControls[i]->Process( x, y ) )
			bFlag = true;
	}
	return bFlag;
}

void CUIDialog::Render()
{
	if( !m_bActivate )
		return;

	for(int i=0;i<m_pControls.Num();i++)
	{
		m_pControls[i]->Render();
	}
}

void CUIDialog::SendMsg( ST_UI_MSG * pMsg )
{
	g_pUI->m_ArrUIMsg.AddItem( pMsg );
}

CUIButton * CUIDialog::AddButton( DWORD dwId, float x, float y, float width, float height, bool bActivate )
{
	CUIControl * p = new CUIButton;
	m_pControls.AddItem( p );
	p->Init( dwId, x, y, width, height, bActivate, this );
	return (CUIButton *)p;
}

CUIStatic * CUIDialog::AddStatic( DWORD dwId, float x, float y, float width, float height, bool bActivate )
{
	CUIControl * p = new CUIStatic;
	m_pControls.AddItem( p );
	p->Init( dwId, x, y, width, height, bActivate, this );
	return (CUIStatic *)p;
}

CUIEditBox * CUIDialog::AddEditBox( DWORD dwId, float x, float y, float width, float height, bool bActivate )
{
	CUIControl * p = new CUIEditBox;
	m_pControls.AddItem( p );
	p->Init( dwId, x, y, width, height, bActivate, this );
	return (CUIEditBox *)p;
}

CUIListBox * CUIDialog::AddListBox( DWORD dwId, float x, float y, float width, float height, bool bActivate )
{
	CUIListBox * p = new CUIListBox;
	m_pControls.AddItem( p );
	p->Init( dwId, x, y, width, height, bActivate, this );
	return (CUIListBox *)p;
}







//////////////////////////////////////////////////////////////////////////////////////////////////
//CUIButton

bool CUIButton::Process( float x, float y )
{
	if( !m_bActivate )
		return false;

	bool bFlag = false;


	if( APP_Tools::appInRect( (int)x, (int)y, (int)m_fX, (int)m_fY, (int)m_fWidth, (int)m_fHeight ) )
	{
		CUIMouse * pUI = g_pUI->m_pMouseUI;

		//点击事件
		if( pUI->m_iLUp == 0 && m_bMouseFlag == BUTTON_MOUSE_DOWN )
		{
			ST_UI_MSG * pMsg = new ST_UI_MSG;
			pMsg->dwDialogId = m_pDialog->GetId();
			pMsg->dwControlId = this->GetId();
			pMsg->msg = BUTTON_CLICK;
			m_pDialog->SendMsg( pMsg );
		}

		if( pUI->m_iLDown == -1 )
		{
			SetOn();
		}
		else
		{
			if( m_bMouseFlag != BUTTON_MOUSE_NORMAL )
				SetDown();
			else
				SetNormal();
		}
	}
	else
	{
		SetNormal();
	}


	return bFlag;
}



 

//////////////////////////////////////////////////////////////////////////////////////////////
//CUIStatic

bool CUIStatic::Process( float x, float y )
{
	if( !m_bActivate )
		return false;

	if( !APP_Tools::appInRect( (int)x, (int)y, (int)m_fX, (int)m_fY, (int)m_fWidth, (int)m_fHeight ) )
		return false;
	return true;

	/*
	CUIMouse * pUI = g_pUI->m_pMouseUI;

	if(pUI->m_nLDown == 1)
	{
		if(g_pUI->m_pEquipUI->MouseEvent(m_wId,L_DOWN))
			return;
		
		if(g_pUI->m_pItemUI->MouseEvent(m_wId,L_DOWN))
			return;

		//
	}
	else if(pUI->m_nLUp == 1)
	{
		if(g_pUI->m_pEquipUI->MouseEvent(m_wId,L_UP))
			return;

		if(g_pUI->m_pItemUI->MouseEvent(m_wId,L_UP))
			return;

		//
	}
	*/
	
}



/////////////////////////////////////////////////////////////////////////////////
//CUIEditBox

bool CUIEditBox::Process( float x, float y )
{
	DOUBLE fTimeNow = APP_Tools::appGetTime();
	if( fTimeNow - m_fPassTime < 0.5f )
	{
		static int a = 0;
		a++;
		m_bCursor = false;
	}
	else if( fTimeNow - m_fPassTime >= 0.5f && fTimeNow - m_fPassTime < 1.f )
	{
		static int a = 0;
		a++;
		m_bCursor = true;
	}
	else
	{
		m_fPassTime = fTimeNow;
		m_bCursor = false;
	}

	return false;
}

void CUIEditBox::Render()
{
	CUIControl::Render();

	RECT rect;

	rect.left = (long)(m_fX + m_fBWidth);
	rect.top = (long)(m_fY + m_fBHeight);
	rect.right = (long)(rect.left + m_fWidth - m_fBWidth);
	rect.bottom = (long)(rect.top + m_fHeight);

	TCHAR tmp[51];
	_tcscpy( tmp, m_szText );
	_tcscat( tmp, L"a" );
	m_pVFont->m_pFont->DrawText( NULL, tmp, (INT)_tcslen( tmp ), &rect, DT_CALCRECT, m_dwTextColor );
	RECT rect2;
	rect2.left = 0;
	m_pVFont->m_pFont->DrawText( NULL, L"a", (INT)_tcslen( L"a" ), &rect2, DT_CALCRECT, m_dwTextColor );
	rect.right -= rect2.right;

	if( rect.right - rect.left > m_fWidth - 2 * m_fBWidth )
	{
		rect.right = (long)(m_fX + m_fWidth - m_fBWidth);
		if( m_bPassword )
			m_pVFont->m_pFont->DrawText( NULL, m_szPassword, (INT)_tcslen( m_szPassword ), &rect, DT_RIGHT, m_dwTextColor );
		else
			m_pVFont->m_pFont->DrawText( NULL, m_szText, (INT)_tcslen( m_szText ), &rect, DT_RIGHT, m_dwTextColor );
	}
	else
	{
		if( m_bPassword )
			m_pVFont->m_pFont->DrawText( NULL, m_szPassword, (INT)_tcslen( m_szPassword ), &rect, DT_LEFT, m_dwTextColor );
		else
			m_pVFont->m_pFont->DrawText( NULL, m_szText, (INT)_tcslen( m_szText ), &rect, DT_LEFT, m_dwTextColor );
	}

	if( m_bCursor && m_bFocus )
	{
		rect.left = rect.right - 3;
		rect.right = rect.left + 5;
		rect.bottom = rect.top + 20;
		m_pVFont->m_pFont->DrawText( NULL, L"|", 1, &rect, DT_LEFT, m_dwTextColor );
	}
}

void CUIListBox::SetXY(float x,float y)
{
	for( int i = 0; i < m_pLineStatic.Num(); i ++ )
	{
		m_pLineStatic[i]->SetXY(x,y + i*m_fHeight);
	}
	CUIControl::SetXY(x,y);
}
void CUIEditBox::OnChar( WPARAM wParam )
{
	if( wParam >= 0x20 )//可打印字符
	{
		if( m_iCharNum == 49 )
			return;
		m_szText[m_iCharNum] = (TCHAR)wParam;
		m_szText[m_iCharNum+1] = 0;
		m_szPassword[m_iCharNum] = '*';
		m_szPassword[m_iCharNum+1] = 0;
		m_iCharNum += 1;
	}
	else if( wParam == 0x08 )	//删除符号
	{
		if( m_iCharNum == 0 )
			return;
		m_szText[m_iCharNum-1] = 0;
		m_szPassword[m_iCharNum-1] = 0;
		m_iCharNum -= 1;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//CUIScrollBar

bool CUIScrollBar::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	//switch( msg )
	//{
	//case WM_LBUTTONDOWN:
	//	{
	//		int x = LOWORD( pMsg->lParam );
	//		int y = HIWORD( pMsg->lParam );

	//		RECT rect;
	//		GetRect( &rect );
	//		if( APP::appInRect( x, y, rect ) )
	//		{
	//			UI_MSG * pMsg = new UI_MSG;
	//			pMsg->dwDialogId = m_pDialog;
	//			pMsg->dwControlId = m_dwId;
	//			pMsg->msg = msg;
	//			pMsg->wParam = wParam;
	//			pMsg->lParam = lParam;
	//			m_pDialog->SendMsg( pMsg );
	//			return true;
	//		}
	//	}
	//}
	return false;
}

bool CUIScrollBar::Process( float x, float y )
{
	if( m_bMidDown )
	{
		//拖动
	}
	else
	{
		_ASSERT( g_pUI->m_pGameUI );
		CUIMouse * pMouseUI = g_pUI->m_pMouseUI;
		_ASSERT( pMouseUI );

		if( APP_Tools::appInRect( (int)x, (int)y, (int)m_fX, (int)m_fY, (int)m_fWidth, (int)m_fHeight ) )
		{
			if( pMouseUI->m_iLDown == 0 )
			{
				//判断点哪了
			}
		}
		else
		{
			if( pMouseUI->m_iLUp == 0 )
			{
				//按钮复位
			}
		}
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//CUIListBox

void CUIListBox::Render()
{
	CUIControl::Render();

	for( int i = 0; i < m_pLineStatic.Num(); i ++ )
	{
		m_pLineStatic[i]->Render();
	}
}


bool CUIListBox::Process( float x, float y )
{
	for( int i = 0; i < m_wCurLineNum; i ++ )
	{
		m_pLineStatic[i]->SetText( m_pLineText[i] );
	}
	return false;
}

bool CUIListBox::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	return false;
}