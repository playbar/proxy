
#pragma once

#include "D3D.h"
#include "Base/Texture.h"

#define D3DFVF_BILLBOARD (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)


enum BILLBOARD_TYPE
{
	BILLBOARD_PARALLEL,
	BILLBOARD_VERTICAL,
	BILLBOARD_STAND,
};



class CBillBoard : public CBase
{
public:
	CBillBoard( int iType, float fWidth, float fHeight, float fFrameTime, D3DXVECTOR3 vCenterPos, D3DXVECTOR3 vPos, DWORD dwColor = 0xff000000 );	
	~CBillBoard();

	void SetMatrix();
	void Render();
	void AddTexture( CTexture * pTex );

	void SetFrameTime( float fTime )
	{
		m_dFrameTime = fTime;
	}

	WORD	m_wFrameNum;
	WORD	m_wCurFrame;

private:

	struct VERTEX_BILLBOARD
	{
		FLOAT x, y, z; 
		DWORD color;
		FLOAT u, v;
	};

protected:

	int									m_iType;
	float								m_fWidth;
	float								m_fHeight;
	D3DXVECTOR3							m_vCenterPos;
	
	LPDIRECT3DVERTEXBUFFER9				m_pVertexBuffer;

	Array<CTexture *>					m_pTextures;

	double								m_dFrameTime;
	double								m_dLastTime;

};