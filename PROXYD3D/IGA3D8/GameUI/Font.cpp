
#include "stdafx.h"

#include "IGA3D8.h"
#include "Font.h"
#include "D3D.h"

bool CVFont::CreateVFont( WORD width, WORD height, TCHAR * name )
{
	SAFE_RELEASE( m_pFont );
	HFONT TextFont=CreateFont( 12, 6,
		0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_SCRIPT, "ËÎÌו" );

	if( FAILED( D3DXCreateFont( g_pDirect3DDevice8, TextFont, &m_pFont ) ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CVFont::Text( int x, int y, int width, int height, TCHAR * text, DWORD dwColor, DWORD dwTextFlag )
{
	RECT rect = { x, y, x + width, y + height };
	m_pFont->DrawText( text, (INT)_tcslen( text ), &rect, dwTextFlag, dwColor );
}

void CVFont::Text( LPD3DXSPRITE pSprite, int x, int y, int width, int height, TCHAR * text, DWORD dwColor, DWORD dwTextFlag )
{
	RECT rect = { x, y, x + width, y + height };
	m_pFont->DrawText( text, (INT)_tcslen( text ), &rect, dwTextFlag, dwColor );
}