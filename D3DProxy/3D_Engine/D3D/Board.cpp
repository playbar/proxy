
#include "Board.h"


void CStaticBoardPool::Render()
{
	UseRenderState();
	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );
	m_pDevice->SetTransform( D3DTS_WORLD, &mat );
	m_pDevice->SetFVF( D3DFVF_BOARD );
	m_pDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof( VERTEX_BOARD ) );
	m_pDevice->SetIndices( m_pIndexBuffer );
	m_pDevice->SetTexture( 0, m_pTexture->GetBuffer() );
	m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_dwBoardNum*4, 0, m_dwBoardNum*2 );
}

//////////////////////////////////////////////////////////////


void CBoard::Render()
{
	UseRenderState();
	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );
	m_pDevice->SetTransform( D3DTS_WORLD, &mat );
	m_pDevice->SetFVF( D3DFVF_BOARD );
	m_pDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof( VERTEX_BOARD ) );
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

void CBoard::AddTexture( CTexture * pTex )
{
	m_pTextures.AddItem( pTex );
	++ m_wFrameNum;
}