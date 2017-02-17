
#include "LoadingUI.h"

void CUILoading::Init(  DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pStatic = AddStatic( UI_LOADING_CONTROL_STATIC_BACK, 0.f, 0.f, 800.f, 600.f, true );
	pStatic->SetTexture( m_pTextures[0] );
	pStatic->SetUV( 0.f, 0.f, 800.f / 1024.f, 600.f / 1024.f );
	pStatic->SetColor( 0xff000000 );

	pStatic = AddStatic( UI_LOADING_CONTROL_STATIC_LOADING, 150.f, 500.f, 0.f, 25.f, true );
	pStatic->SetTexture( m_pTextures[1] );
	pStatic->SetUV( 0.f, 0.f, 0.f, 1.f );
}

void CUILoading::Release()
{
	CUIDialog::Release();
}

void CUILoading::Render()
{
	CUIDialog::Render();
}

bool CUILoading::Process( float x, float y )
{
	return CUIDialog::Process( x, y );
}

bool CUILoading::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

bool CUILoading::Event( ST_UI_MSG * pMsg )
{
	return false;
}

void CUILoading::UpdateLoading( float fLoading )
{
	if( m_fLoading < 100.f )
		m_fLoading += fLoading;
	if( m_fLoading > 100.f )
		m_fLoading = 100.f;

	CUIStatic * pStatic = GetStatic( UI_LOADING_CONTROL_STATIC_LOADING );
	_ASSERT( pStatic );

	RECT rect;
	pStatic->GetRect( &rect );
	pStatic->SetWH( m_fLoading / 100.f * MAX_LOADING_LENGTH, (float)(rect.bottom - rect.top) );
	pStatic->SetUV( 0.f, 0.f, m_fLoading / 100.f, 1.f );
}

void CUILoading::SetLoading( float fLoading )
{
	m_fLoading = fLoading;
	UpdateLoading( 0.f );
}