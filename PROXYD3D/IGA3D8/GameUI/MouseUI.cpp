#include "stdafx.h"
#include "MouseUI.h"
//#include "UI.h"

void CUIMouse::Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;
	CUIStatic * pUIStaticItem = AddStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM,x,y,51.f,39.f,false);
	pUIStaticItem->SetUV(0.f,0.f,51.f/64.f,39.f/64.f);
}

void CUIMouse::Render()
{
	CUIDialog::Render();
}

bool CUIMouse::Process( float x, float y )
{
	m_MousePos.x = (long)x;
	m_MousePos.y = (long)y;

	if( m_iLDown != -1 )
		m_iLDown ++;
	if( m_iLUp != -1 )
		m_iLUp ++;
	if( m_iRDown != -1 )
		m_iRDown ++;
	if( m_iRUp != -1 )
		m_iRUp ++;

	if( m_iLDown >= 500 )
		m_iLDown = 2;
	if( m_iLUp >= 500 )
		m_iLUp = 2;
	if( m_iRDown >= 500 )
		m_iRDown = 2;
	if( m_iRUp >= 500 )
		m_iRUp = 2;
	
	CUIStatic * pUIStaticMouse = GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
	pUIStaticMouse->SetXY(x,y);

	return CUIDialog::Process( x, y );
}

bool CUIMouse::Event( UI_MSG * pMsg )
{
	switch( pMsg->msg )
	{
	case WM_LBUTTONDOWN:
		{
			m_iLDown = 0;
			m_iLUp = -1;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_iLDown = -1;
			m_iLUp = 0;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			m_iRDown = 0;
			m_iRUp = -1;

			m_LastPos.x = LOWORD( pMsg->lParam );
			m_LastPos.y = HIWORD( pMsg->lParam );
		}
		break;
	case WM_RBUTTONUP:
		{
			m_iRDown = -1;
			m_iRUp = 0;
		}
		break;
	}
	return false;
}