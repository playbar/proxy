
#include "Sprite.h"


void CBillBoard::Render()
{	
	D3DXMATRIX matWorld,matScaling,matRotation,matTranslation,matView;

	D3DXMatrixScaling( &matScaling, 1.f/25, 1.f/25, 1.f/25 );
	D3DXMatrixRotationY( &matRotation, 0.f );
	D3DXMatrixTranslation( &matTranslation, m_vPos.x, m_vPos.y, m_vPos.z );
	D3DXMatrixMultiply( &matWorld, &matScaling, &matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTranslation );
	//m_pSprite->SetTransform( &matWorld );

	//m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	m_pDevice->GetTransform( D3DTS_VIEW, &matView );
	m_pSprite->SetWorldViewLH( &matWorld, &matView );
	m_pSprite->GetTransform( &matWorld );
	
	if( FAILED(m_pSprite->Begin( m_dwRenderFlag )) )
		return;
	if( m_pTexture )
		m_pSprite->Draw( m_pTexture->GetBuffer(), NULL, &m_vCenter, &m_vPos, m_dwColor );
	else
		m_pSprite->Draw( NULL, NULL, &m_vCenter, &m_vPos, m_dwColor );

	m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	m_pVFont->Text( m_pSprite, 0, 0, 200, 20, L"asldfjasd", 0xffff0000, DT_LEFT );
	m_pSprite->End();
}

void CBillBoard::RenderText( TCHAR * szText, DWORD dwColor, DWORD dwTextFlag )
{
	if( m_pVFont )
		m_pVFont->Text( m_pSprite, m_Rect.left, m_Rect.top, m_Rect.right - m_Rect.left, m_Rect.bottom - m_Rect.top, szText, dwColor, dwTextFlag );
}