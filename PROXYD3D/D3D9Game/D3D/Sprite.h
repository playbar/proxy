
#pragma once


#include "D3D.h"
#include "Base/Texture.h"


class CBillBoard : public CBase
{
public:
	CBillBoard()
	{
		m_pDevice = g_pD3D->GetDevice();
		D3DXCreateSprite( m_pDevice, &m_pSprite );

		m_pTexture = NULL;
		m_dwColor = 0xffffffff;
		m_pVFont = g_pD3D->m_pVFont[3];
		m_dwRenderFlag = D3DXSPRITE_BILLBOARD | D3DXSPRITE_SORT_TEXTURE ;
	}
	~CBillBoard()
	{
		m_pSprite->Release();
	}


protected:

	LPD3DXSPRITE			m_pSprite;

	D3DXVECTOR3				m_vCenter;

	CTexture *				m_pTexture;

	DWORD					m_dwColor;

	CVFont *				m_pVFont;

	RECT					m_Rect;

	DWORD					m_dwRenderFlag;

public:


	void Render();

	void RenderText( TCHAR * szText, DWORD dwColor, DWORD dwTextFlag );

	void SetTexture( CTexture * pTex )
	{
		m_pTexture = pTex;
	}

	void SetCenter( D3DXVECTOR3 vPos )
	{
		m_vCenter = vPos;
	}
	D3DXVECTOR3 & GetCenter()
	{
		return m_vCenter;
	}

	void SetColor( DWORD dwColor )
	{
		m_dwColor = dwColor;
	}
	DWORD GetColor()
	{
		return m_dwColor;
	}

	void SetFont( CVFont * pVFont )
	{
		m_pVFont = pVFont;
	}

	void SetRect( int x, int y, int width, int height )
	{
		m_Rect.left = x;
		m_Rect.top = y;
		m_Rect.right = x + width;
		m_Rect.bottom = y + height;
	}
	void SetRect( RECT * rect )
	{
		m_Rect.left = rect->left;
		m_Rect.top = rect->top;
		m_Rect.right = rect->right;
		m_Rect.bottom = rect->bottom;
	}

	void SetRenderFlag( DWORD dwFlag )
	{
		m_dwRenderFlag = dwFlag;
	}



};