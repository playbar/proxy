#include "stdafx.h"

#include "D2D.h"


//更新顶点信息
void CBase2D::UpdateData( DWORD dwColor, float width, float height, float x, float y, float z )
{
	VERTEX_2D * pVertices = NULL;
	m_pVertexBuffer->Lock( 0, 4 * sizeof( VERTEX_2D ), (byte **)&pVertices, 0 );

	pVertices[0].x = x;
	pVertices[0].y = y;
	
	pVertices[2].x = x;
	pVertices[2].y = y + height;

	pVertices[1].x = x + width;
	pVertices[1].y = y;

	pVertices[3].x = x + width;
	pVertices[3].y = y + height;
	
	pVertices[0].z = z;
	pVertices[1].z = z;
	pVertices[2].z = z; 
	pVertices[3].z = z;

	pVertices[0].rhw = 0.f;
	pVertices[1].rhw = 0.f;
	pVertices[2].rhw = 0.f;
	pVertices[3].rhw = 0.f;

	pVertices[0].color = dwColor;
	pVertices[1].color = dwColor;
	pVertices[2].color = dwColor;
	pVertices[3].color = dwColor;
	m_pVertexBuffer->Unlock();
}

void CBase2D::UpdateColor( DWORD dwColor )
{
	VERTEX_2D * pVertices = NULL;
	m_pVertexBuffer->Lock( 0, 4 * sizeof( VERTEX_2D ), (byte **)&pVertices, 0 );

	pVertices[0].color = dwColor;
	pVertices[1].color = dwColor;
	pVertices[2].color = dwColor;
	pVertices[3].color = dwColor;

	m_pVertexBuffer->Unlock();
}

void CBase2D::UpdateUV( float fStartU, float fStartV, float fEndU, float fEndV )
{
	VERTEX_2D * pVertices = NULL;
	m_pVertexBuffer->Lock( 0, 4 * sizeof( VERTEX_2D ), (byte **)&pVertices, 0 );

	pVertices[0].u = fStartU;
	pVertices[0].v = fStartV;
	pVertices[1].u = fEndU;
	pVertices[1].v = fStartV;
	pVertices[2].u = fStartU;
	pVertices[2].v = fEndV;
	pVertices[3].u = fEndU;
	pVertices[3].v = fEndV;

	m_pVertexBuffer->Unlock();
}

void CBase2D::Render()
{
	D3DXMATRIX matTrans;
	D3DXMatrixIdentity( &matTrans );
	m_pDevice->SetTransform( D3DTS_WORLD, &matTrans );

	m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	if( m_pTexture )
	{
		m_pDevice->SetTexture( 0, m_pTexture );

		//m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,D3DTOP_BLENDTEXTUREALPHA );	//使用透明贴图

		m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );


		m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
		m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );

		//m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		//m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );


		m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
		m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


		m_pDevice->SetStreamSource( 0, m_pVertexBuffer, sizeof( VERTEX_2D ) );
		//m_pDevice->SetFVF( D3DFVF_2D );

		//m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		//m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		//m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		//m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}
}

void CBase2D::SetRotate(float R)
{
	R -= m_fR;
	VERTEX_2D * pVertices = NULL;
	m_pVertexBuffer->Lock( 0, 4 * sizeof( VERTEX_2D ), (byte **)&pVertices, 0 );
	
	D3DXVECTOR3 vPos;
	
	vPos.x = (pVertices[0].x + pVertices[1].x + pVertices[2].x + pVertices[3].x)/4;
	vPos.y = (pVertices[0].y + pVertices[1].y + pVertices[2].y + pVertices[3].y)/4;
	vPos.z = (pVertices[0].z + pVertices[1].z + pVertices[2].z + pVertices[3].z)/4;

	D3DXMATRIX MatrixT;
	D3DXMATRIX MatrixR;
	D3DXMATRIX MatrixT2;

	D3DXMatrixTranslation(&MatrixT,-vPos.x,-vPos.y,-vPos.z);
	D3DXMatrixRotationZ(&MatrixR,R);
	D3DXMatrixTranslation(&MatrixT2,vPos.x,vPos.y,vPos.z);


	for (int i = 0; i < 4; i ++)
	{
		vPos.x = pVertices[i].x; 
		vPos.y = pVertices[i].y;
		vPos.z = pVertices[i].z;
		
		APP_Tools::appVec3Transform(vPos,MatrixT);
		APP_Tools::appVec3Transform(vPos,MatrixR);
		APP_Tools::appVec3Transform(vPos,MatrixT2);

		pVertices[i].x = vPos.x;
		pVertices[i].y = vPos.y;
		pVertices[i].z = vPos.z;
	}
	m_pVertexBuffer->Unlock();
}