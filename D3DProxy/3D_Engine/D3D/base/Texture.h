
#pragma once


#include "..\D3D.h"


//ÎÆÀí
class CTexture
{
public:
	CTexture()
	{
		m_pDevice = g_pMyD3D->GetDevice();
		m_pTexture = NULL;
	}
	~CTexture()
	{
		SAFE_RELEASE( m_pTexture );
	}

	bool CreateFromFile( TCHAR * file, bool bZip );

	LPDIRECT3DTEXTURE9 GetBuffer(){ return m_pTexture; }

protected:

	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DDEVICE9		m_pDevice;
};