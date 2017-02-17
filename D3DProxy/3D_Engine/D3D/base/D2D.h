

#pragma once

#include "..\D3D.h"
#include "Texture.h"


#define D3DFVF_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


//2D»ùÀà
class CBase2D
{
public:

	CBase2D()
	{
		m_pDevice = g_pMyD3D->GetDevice();
		m_pTexture = NULL;
		m_fR = 0;
		m_pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * 4, 0, D3DFVF_2D, D3DPOOL_MANAGED, &m_pVertexBuffer, 0 );
	}
	~CBase2D() { m_pVertexBuffer->Release(); }
	void SetRotate(float R);

	float m_fR;
private:

	struct VERTEX_2D
	{
		FLOAT x, y, z, rhw;
		DWORD color;
		FLOAT u, v;
	};

	LPDIRECT3DDEVICE9				m_pDevice;

	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;
	LPDIRECT3DTEXTURE9				m_pTexture;


protected:

	void UpdateData( DWORD dwColor, float width, float height, float x, float y, float z = 0.f );
	
	void UpdateColor( DWORD dwColor );

	void UpdateUV( float fStartU, float fStartV, float fEndU, float fEndV );

	void UpdateTexture( LPDIRECT3DTEXTURE9 tex )
	{
		m_pTexture = tex;
	}

	void UpdateTexture( CTexture * tex )
	{
		if( tex )
			m_pTexture = tex->GetBuffer();
		else
			m_pTexture = NULL;
	}

	virtual void Render();

};