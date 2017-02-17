
#pragma once 
#include <d3d9.h>
#include <d3dx9.h>
#include "../D3DStruct.h"


//×ÖÌå
class CVFont
{
public:

	CVFont()
	{
		m_pFont = NULL;
	}
	~CVFont()
	{
		SAFE_RELEASE( m_pFont );
	}

	bool CreateVFont( WORD width, WORD height, TCHAR * name );

	void Text( int x, int y, int width, int height, TCHAR * text, DWORD dwColor, DWORD dwTextFlag );
	
	void Text( LPD3DXSPRITE pSprite, int x, int y, int width, int height, TCHAR * text, DWORD dwColor, DWORD dwTextFlag );
	

	LPD3DXFONT		m_pFont;

private:

protected:

};
