
#include "Texture.h"

bool CTexture::CreateFromFile( TCHAR * file, bool bZip )
{
	if( bZip && g_pMyD3D->m_D3DFMT )
	{
		if( FAILED( D3DXCreateTextureFromFileEx( m_pDevice, 	
						file, 512, 512, 1, 0, g_pMyD3D->m_D3DFMT, 	
						D3DPOOL_DEFAULT ,D3DX_DEFAULT, 	
						D3DX_DEFAULT,0xFF003000,0,0,&m_pTexture ) ) )
			return false;

	}
	else
	{
		if( FAILED( D3DXCreateTextureFromFile( m_pDevice, file, &m_pTexture ) ) )
			return false;
	}
	return true;
}