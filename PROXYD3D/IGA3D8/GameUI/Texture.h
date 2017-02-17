
#pragma once
#include "d3d8.h"
#include "IGA3D8.h"
#include "D3Dstruct.h"

//Œ∆¿Ì
class CTexture
{
public:
	CTexture()
	{
		m_pDevice = g_pDirect3DDevice8;
		m_pTexture = NULL;
	}
	~CTexture()
	{
		SAFE_RELEASE( m_pTexture );
	}

	bool CreateFromFile( TCHAR * file, bool bZip );

	LPDIRECT3DTEXTURE8 GetBuffer(){ return m_pTexture; }

protected:

	LPDIRECT3DTEXTURE8		m_pTexture;
	LPDIRECT3DDEVICE8		m_pDevice;
};