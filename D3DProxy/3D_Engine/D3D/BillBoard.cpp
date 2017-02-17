

#include "BillBoard.h"
#include "Camera.h"

CBillBoard::CBillBoard( int iType, float fWidth, float fHeight, float fFrameTime, D3DXVECTOR3 vCenterPos, D3DXVECTOR3 vPos, DWORD dwColor )
{
	m_dwBaseType = OBJECT_TYPE_BILLBOARD;
	m_pDevice = g_pMyD3D->GetDevice();
	m_iType = iType;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_vCenterPos = vCenterPos;
	m_vPos = vPos;

	m_wFrameNum = 0;
	m_wCurFrame = 0;
	m_dFrameTime = fFrameTime;
	m_dLastTime = APP_Tools::appGetTime();

	VERTEX_BILLBOARD VB[] = 
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
		VB[i].x -= vCenterPos.x;
		VB[i].y -= vCenterPos.y;
		VB[i].z -= vCenterPos.z;
	}

	void * pVertex;
	m_pDevice->CreateVertexBuffer( sizeof(VB), D3DUSAGE_WRITEONLY, D3DFVF_BILLBOARD,                 
		D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );

	m_pVertexBuffer->Lock( 0, sizeof(VB), (void**)&pVertex, 0 );
	memcpy( pVertex, VB, sizeof(VB) );
	m_pVertexBuffer->Unlock();

	SetCustomRenderState( COLOR_SELECT_TEXTURE, ALPHA_SELECT_DIFFUSE, ALPHABLEND_NONE, FILTER_POINT, CULL_NONE, TRUE, TRUE,  0 );
}

CBillBoard::~CBillBoard()
{
	SAFE_RELEASE( m_pVertexBuffer );
}


void CBillBoard::AddTexture( CTexture * pTex )
{
	m_pTextures.AddItem( pTex );
	++ m_wFrameNum;
}


void CBillBoard::SetMatrix()
{
	D3DXMATRIX matView, matWorld;
	switch( m_iType )
	{
	case BILLBOARD_PARALLEL://平行
		{
			m_pDevice->GetTransform( D3DTS_VIEW, &matView );
			D3DXMatrixInverse( &matWorld, NULL, &matView );
			matWorld._41 = m_vPos.x;
			matWorld._42 = m_vPos.y;
			matWorld._43 = m_vPos.z;
			m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
		}
		break;
	case BILLBOARD_VERTICAL://垂直
		{
			D3DXMatrixLookAtLH( &matView, &g_pCamera->GetPos(), &m_vPos, &D3DXVECTOR3( 0.f, 1.f, 0.f ) );
			D3DXMatrixInverse( &matWorld, NULL, &matView );
			matWorld._41 = m_vPos.x;
			matWorld._42 = m_vPos.y;
			matWorld._43 = m_vPos.z;
			m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
		}
		break;
	case BILLBOARD_STAND:  //站立
		{
			D3DXVECTOR3 vPos = g_pCamera->GetPos();
			D3DXVECTOR3 vLookAt = g_pCamera->GetLookAt();
			vPos.y = vLookAt.y;
			D3DXMatrixLookAtLH( &matView, &vPos, &vLookAt, &D3DXVECTOR3( 0.f, 1.f, 0.f ) );
			D3DXMatrixInverse( &matWorld, NULL, &matView );
			matWorld._41 = m_vPos.x;
			matWorld._42 = m_vPos.y;
			matWorld._43 = m_vPos.z;
			m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

		}
		break;
	}
}

void CBillBoard::Render()
{
	UseRenderState();
	SetMatrix();

	m_pDevice->SetFVF( D3DFVF_BILLBOARD );
	m_pDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(VERTEX_BILLBOARD) );
	double dTimeNow = APP_Tools::appGetTime();
	if( dTimeNow - m_dLastTime >= m_dFrameTime )
	{
		m_dLastTime = dTimeNow;
		++ m_wCurFrame;
		if( m_wCurFrame >= m_wFrameNum )
			m_wCurFrame = 0;
	}
	m_pDevice->SetTexture( 0, m_pTextures[m_wCurFrame]->GetBuffer() );
	m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}