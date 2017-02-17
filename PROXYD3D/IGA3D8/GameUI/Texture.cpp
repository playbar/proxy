#include "stdafx.h"

#include "Texture.h"

bool CTexture::CreateFromFile( TCHAR * file, bool bZip )
{
	if( bZip && D3DFMT_DXT5 )
	{
		if( FAILED( D3DXCreateTextureFromFileEx( m_pDevice, 	
						file, 512, 512, 1, 0, D3DFMT_DXT5 , 	
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