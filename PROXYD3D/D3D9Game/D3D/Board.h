
#pragma once

#include "D3D.h"
#include "Base/Texture.h"


#define D3DFVF_BOARD	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

struct VERTEX_BOARD
{
	float	x,y,z;
	DWORD	color;
	float	tu,tv;
};

class CStaticBoardPool : public CBase
{
public:
	CStaticBoardPool( DWORD dwMaxBoard )
	{
		m_dwBaseType = OBJECT_TYPE_BOARDPOOL;
		m_pTexture = NULL;
		m_dwMaxBoard = dwMaxBoard;
		m_dwBoardNum = 0;
		m_pDevice = g_pMyD3D->GetDevice();
		m_pDevice->CreateVertexBuffer( sizeof(VERTEX_BOARD) * dwMaxBoard * 4, 0, D3DFVF_BOARD, 
			D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );
		m_pDevice->CreateIndexBuffer( sizeof(DWORD)*6*dwMaxBoard, 0,
			D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL );
		SetCustomRenderState( COLOR_SELECT_TEXTURE, ALPHA_SELECT_TEXTURE, ALPHABLEND_NONE, FILTER_POINT, CULL_NONE, TRUE, TRUE, 0 );
	}
	~CStaticBoardPool()
	{
		SAFE_RELEASE( m_pVertexBuffer );
		SAFE_RELEASE( m_pIndexBuffer );
	}


protected:

	DWORD						m_dwMaxBoard;
	DWORD						m_dwBoardNum;

	CTexture *					m_pTexture;

	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuffer;


public:

	void SetTexture( CTexture * pTex )
	{
		m_pTexture = pTex;
	}

	bool AddBoard( float fWidth, float fHeight, float fAngleX, float fAngleY, float fStartU, float fStartV, float fEndU, float fEndV, D3DXVECTOR3 vCenterPos, DWORD dwColor = 0xff000000 )
	{
		if( m_dwBoardNum == m_dwMaxBoard )
			return false;

		VERTEX_BOARD VB[] =
		{
			{-0.5f, 0.5f, 0.f, dwColor, fStartU, fStartV },
			{-0.5f,-0.5f, 0.f, dwColor, fStartU, fEndV },
			{ 0.5f, 0.5f, 0.f, dwColor, fEndU, fStartV },
			{ 0.5f,-0.5f, 0.f, dwColor, fEndU, fEndV }
		};

		D3DXMATRIX mat;
		D3DXVECTOR3 vPos,vDir;
		for( int i = 0; i < 4; ++ i )
		{
			VB[i].x *= fWidth;
			VB[i].y *= fHeight;
			vPos.x = VB[i].x;
			vPos.y = VB[i].y;
			vPos.z = VB[i].z;
			D3DXMatrixRotationX( &mat, fAngleX );
			APP_Tools::appVec3Transform( vPos, mat );
			D3DXMatrixRotationY( &mat, fAngleY );
			APP_Tools::appVec3Transform( vPos, mat );
			vPos.x += vCenterPos.x;
			vPos.y += vCenterPos.y;
			vPos.z += vCenterPos.z;
			VB[i].x = vPos.x;
			VB[i].y = vPos.y;
			VB[i].z = vPos.z;
		}

		void * pVertex;
		void * pIndex;
		m_pVertexBuffer->Lock( 0, sizeof(VERTEX_BOARD) * m_dwMaxBoard * 4, &pVertex, 0 );
		VERTEX_BOARD * p =(VERTEX_BOARD *)pVertex;
		memcpy( &((VERTEX_BOARD *)pVertex)[m_dwBoardNum*4], VB, sizeof(VB) );
		m_pVertexBuffer->Unlock();

		m_pIndexBuffer->Lock( 0, sizeof(DWORD)*6*m_dwMaxBoard, &pIndex, 0 );
		((DWORD *)pIndex)[m_dwBoardNum*6 + 0] = m_dwBoardNum*4 + 0;
		((DWORD *)pIndex)[m_dwBoardNum*6 + 1] = m_dwBoardNum*4 + 1;
		((DWORD *)pIndex)[m_dwBoardNum*6 + 2] = m_dwBoardNum*4 + 2;
		((DWORD *)pIndex)[m_dwBoardNum*6 + 3] = m_dwBoardNum*4 + 2;
		((DWORD *)pIndex)[m_dwBoardNum*6 + 4] = m_dwBoardNum*4 + 1;
		((DWORD *)pIndex)[m_dwBoardNum*6 + 5] = m_dwBoardNum*4 + 3;
		m_pIndexBuffer->Unlock();

		++ m_dwBoardNum;

		return true;
	}

	void Render();

	void Clear(){ m_dwBoardNum = 0; }

};


class CBoard : public CBase
{
public:
	CBoard( float fWidth, float fHeight, float fFrameTime, D3DXVECTOR3 vPos, DWORD dwColor = 0xff000000 )
	{
		m_dwBaseType = OBJECT_TYPE_BOARD;
		m_pDevice = g_pMyD3D->GetDevice();
		m_fWidth = fWidth;
		m_fHeight = fHeight;
		m_vPos = vPos;

		m_wFrameNum = 0;
		m_wCurFrame = 0;
		m_dFrameTime = fFrameTime;
		m_dLastTime = APP_Tools::appGetTime();

		VERTEX_BOARD VB[] = 
		{
			{ -0.5f, 0.5f, 0.0f, dwColor, 0.0f, 0.0f },
			{ -0.5f,-0.5f, 0.0f, dwColor, 0.0f, 1.0f },
			{  0.5f, 0.5f, 0.0f, dwColor, 1.0f, 0.0f },
			{  0.5f,-0.5f, 0.0f, dwColor, 1.0f, 1.0f }
		};

		for( int i = 0; i < 4; ++ i )
		{
			VB[i].x *= fWidth;
			VB[i].y *= fHeight;
		}

		void * pVertex;
		m_pDevice->CreateVertexBuffer( sizeof(VB), D3DUSAGE_WRITEONLY, D3DFVF_BOARD,                 
			D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );

		m_pVertexBuffer->Lock( 0, sizeof(VB), (void**)&pVertex, 0 );
		memcpy( pVertex, VB, sizeof(VB) );
		m_pVertexBuffer->Unlock();

		SetCustomRenderState( COLOR_SELECT_TEXTURE, ALPHA_SELECT_TEXTURE, ALPHABLEND_NONE, FILTER_POINT, CULL_NONE, TRUE, TRUE, 0 );
	}
	~CBoard();

protected:

	float								m_fWidth;
	float								m_fHeight;
	
	LPDIRECT3DVERTEXBUFFER9				m_pVertexBuffer;

	Array<CTexture *>					m_pTextures;
	WORD								m_wFrameNum;
	WORD								m_wCurFrame;
	double								m_dFrameTime;
	double								m_dLastTime;


public:

	void Render();

	void AddTexture( CTexture * );


};