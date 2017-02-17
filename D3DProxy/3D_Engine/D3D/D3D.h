
#pragma once


#include "base\base.h"
#include "base\BaseDefine.h"
#include "D3DStruct.h"
#include "base\Font.h"


class CMyD3D
{
public:
	CMyD3D()
	{
		for( int i = 0; i < 10; i ++ )
		{
			m_pVFont[i] = NULL;
		}
		m_pSky = NULL;
		m_pTerrain = NULL;
	}

	~CMyD3D() {}

	bool	Init( HWND hWnd, WORD wWidth, WORD wHeight, bool bFullScreen );
	void	Release();
	void	Render();

	inline void Clear( DWORD dwColor )
	{
		m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dwColor, 1.0f, 0 );
	}
	inline void BeginScene()
	{
		m_pDevice->BeginScene();
	}
	inline void EndScene()
	{
		m_pDevice->EndScene();
	}
	inline void Flip()
	{
		m_pDevice->Present( NULL, NULL, NULL, NULL );
	}


	inline LPDIRECT3DDEVICE9 GetDevice()
	{
		return m_pDevice;
	}


	void AddToWorld( CBase * p )
	{
		if( p->GetType() == OBJECT_TYPE_SKY )
			m_pSky = p;
		else if( p->GetType() == OBJECT_TYPE_TERRAIN )
			m_pTerrain = p;
		else
			m_pBaseList.AddItem( p );
	}
	void RemoveFromWorld( CBase * p )
	{
		m_pBaseList.RemoveItem(p);
	}

	void RemoveAllFromWorld()
	{
		m_pBaseList.Empty();
		m_pSky = NULL;
		m_pTerrain = NULL;
	}

	HWND GetHWND()
	{
		return m_hWnd;
	}


	void SetFog( float start, float end, float density, DWORD color, WORD fogkey );


public:

	CVFont *						m_pVFont[10];

	WORD							m_wScreenWidth;
	WORD							m_wScreenHeight;

	_D3DFORMAT						m_D3DFMT;


	float							m_fFogStart;
	float							m_fFogEnd;
	float							m_fFogDensity;
	WORD							m_wFogKey;
	DWORD							m_dwFogColor;


	Array<LPDIRECT3DTEXTURE9>		m_pColorTextures;

private:

protected:

	HWND							m_hWnd;

	LPDIRECT3D9						m_pD3D9;
	LPDIRECT3DDEVICE9				m_pDevice;

	Array<CBase *>					m_pBaseList;

	CBase *							m_pSky;
	CBase *							m_pTerrain;


};

extern CMyD3D * g_pMyD3D;
