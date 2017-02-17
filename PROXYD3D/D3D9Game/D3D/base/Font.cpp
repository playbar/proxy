
#include "Font.h"
#include "../D3D.h"

bool CVFont::CreateVFont( WORD width, WORD height, TCHAR * name )
{
	SAFE_RELEASE( m_pFont );
	if( FAILED( D3DXCreateFont( g_pMyD3D->GetDevice(), height, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, name, &m_pFont ) ) )
		return false;
	return true;
}

void CVFont::Text( int x, int y, int width, int height, TCHAR * text, DWORD dwColor, DWORD dwTextFlag )
{
	RECT rect = { x, y, x + width, y + height };
	m_pFont->DrawTextW( NULL, text, (INT)_tcslen( text ), &rect, dwTextFlag, dwColor );
}

void CVFont::Text( LPD3DXSPRITE pSprite, int x, int y, int width, int height, TCHAR * text, DWORD dwColor, DWORD dwTextFlag )
{
	RECT rect = { x, y, x + width, y + height };
	m_pFont->DrawTextW( pSprite, text, (INT)_tcslen( text ), &rect, dwTextFlag, dwColor );
}