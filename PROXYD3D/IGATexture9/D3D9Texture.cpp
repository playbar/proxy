/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGATexture.cpp
*	功能描述：IGATextur9, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#include "stdafx.h"
#include "D3D9Texture.h"

//////////////////////////////////////////////////////////////////////////
//flash 纹理
CD3D9FlashTexture::CD3D9FlashTexture(){}

CD3D9FlashTexture::~CD3D9FlashTexture(){}

HRESULT CD3D9FlashTexture::D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight)
{
	SetDevie(pd3dDevice);
	SetFileName( szFileName);
	SetWidth( iWidth );
	SetHeith( iHeight );
	return S_OK;
}

HRESULT CD3D9FlashTexture::D3D9ResetDevice( LPDIRECT3DDEVICE9 pd3dDevice )
{
	SetDevie(pd3dDevice );
	return S_OK;
}

void CD3D9FlashTexture::D3D9StartPlay()
{
	m_FlashSprite.SetLoopPlay( true );
	if( m_FlashSprite.StartFlash( m_pd3dDevice,m_szFileName, m_iTexWidth, m_iTexHeight ))
	{
		m_bIsPlay = true;
	}
	if (m_FlashSprite.IsPlaying() )
	{
		m_bIsPlay = true;
	}
	else
	{
		m_bIsPlay = false;
	}
}

void  CD3D9FlashTexture::D3D9StopPlay()
{ 
	if (m_FlashSprite.IsPlaying() )
	{
		m_FlashSprite.SetLoopPlay(true );
		m_FlashSprite.Pause();
	}
}


LPDIRECT3DTEXTURE9 CD3D9FlashTexture::D3D9GetTexture()
{
	if (m_FlashSprite.IsPlaying() )
	{
		m_FlashSprite.Update();
		m_pTexture = m_FlashSprite.GetRenderTarget();
	}
	else
	{
		m_FlashSprite.GotoFrame( 0 );
	}
	if( m_pTexture == NULL )
	{
		return NULL;
	}
	return m_pTexture;
}

void CD3D9FlashTexture::D3D9PausePlay()
{
	if (m_FlashSprite.IsPlaying() )
	{
		m_iCurrentFrame = m_FlashSprite.GetCurrentFrame();
		m_FlashSprite.Pause();
	}
}

void CD3D9FlashTexture::D3D9UnPausePlay()
{
	if ( ! m_FlashSprite.IsPlaying() )
	{
		m_FlashSprite.GotoFrame( m_iCurrentFrame );
		m_FlashSprite.Unpause();
	}
}
void CD3D9FlashTexture::D3D9GotoFrame(int iFrame)
{
	m_FlashSprite.GotoFrame( iFrame );
}

void CD3D9FlashTexture::D3D9GetCurrentFrame()
{
	m_iCurrentFrame = m_FlashSprite.GetCurrentFrame();
}


void CD3D9FlashTexture::D3D9TextureDestory()
{
	m_FlashSprite.Destory();
	m_bIsPlay = false;

	CD3D9TextureBase::D3D9TextureDestory();


}

//////////////////////////////////////////////////////////////////////////
//视频纹理
CD3D9VideoTexture::CD3D9VideoTexture()
{

}

CD3D9VideoTexture::~CD3D9VideoTexture()
{

}

HRESULT CD3D9VideoTexture::D3D9TextureInit(LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight )
{
	SetDevie(pd3dDevice);
	SetFileName( szFileName);
	SetWidth( iWidth );
	SetHeith( iHeight );

	HRESULT hr = m_VideoTexture.CreateFromAVIFile( pd3dDevice, szFileName );
	if( FAILED(hr) )
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CD3D9VideoTexture::D3D9ResetDevice( LPDIRECT3DDEVICE9 pd3dDevice )
{
	return S_OK;
}

void CD3D9VideoTexture::D3D9StartPlay()
{
	m_VideoTexture.StartPlay();
}

void CD3D9VideoTexture::D3D9StopPlay()
{
	//m_VideoTexture.Destroy();
}


LPDIRECT3DTEXTURE9  CD3D9VideoTexture::D3D9GetTexture()
{
	m_pTexture = m_VideoTexture.GetTexture();
	m_VideoTexture.CheckForLoop();
	//m_bIsPlay = m_VideoTexture.m_bStartPlay;

	return m_pTexture;
}


void CD3D9VideoTexture::D3D9PausePlay()
{
	//m_VideoTexture.Pause();
}

void CD3D9VideoTexture::D3D9UnPausePlay()
{
	//m_VideoTexture.StartPlay();
}

void CD3D9VideoTexture::D3D9GotoFrame(int iFrame)
{

}

void CD3D9VideoTexture::D3D9GetCurrentFrame()
{

}

void CD3D9VideoTexture::D3D9TextureDestory()
{
	m_VideoTexture.Destroy();
	m_pTexture = NULL;
	m_pd3dDevice = NULL;
}

//------------------------------------------------------
//静态纹理
CIGAStaticTexture::CIGAStaticTexture()
{

}

CIGAStaticTexture::~CIGAStaticTexture()
{

}

HRESULT CIGAStaticTexture::D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* name )
{
	HRESULT hr = D3DXCreateTextureFromFile( pd3dDevice, name, &m_pTexture );
	if( FAILED(hr) )
	{
		m_pTexture = NULL;
		return E_FAIL;
	}
	return S_OK;
}

LPDIRECT3DTEXTURE9 CIGAStaticTexture::D3D9GetTexture()
{
	return NULL;
}

void CIGAStaticTexture::D3D9TextureDestory()
{
	//if( m_pTexture != NULL )
	//{
	//	m_pTexture->Release();
	//	m_pTexture = NULL;
	//}
}

