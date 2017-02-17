/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGATexture.h
*	功能描述：IGATextur9, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once

#include "d3d9.h"
#include "IGATexture9.h"


//flash纹理 
class CD3D9FlashTexture : public CD3D9TextureBase
{
public:
	CD3D9FlashTexture();
	virtual ~CD3D9FlashTexture();

public:
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight );
	HRESULT D3D9ResetDevice( LPDIRECT3DDEVICE9 pd3dDevice );
	void D3D9StartPlay();
	void D3D9StopPlay();
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	void D3D9PausePlay();
	void D3D9UnPausePlay();
	void D3D9GotoFrame(int iFrame);
	void D3D9GetCurrentFrame();
	void D3D9TextureDestory();

private:
	FlashSprite		m_FlashSprite;
};


//视频纹理
class CD3D9VideoTexture : public CD3D9TextureBase
{
public:
	CD3D9VideoTexture();
	~CD3D9VideoTexture();

public:
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight );
	HRESULT D3D9ResetDevice( LPDIRECT3DDEVICE9 pd3dDevice );
	void D3D9StartPlay();
	void D3D9StopPlay();
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	void D3D9PausePlay();
	void D3D9UnPausePlay();
	void D3D9GotoFrame(int iFrame);
	void D3D9GetCurrentFrame();
	void D3D9TextureDestory();

private:
	CDirectXVideoTexture	m_VideoTexture;
};

//--------------------------------------------------------------
//静态纹理
class CIGAStaticTexture : public CD3D9TextureBase
{
public:
	CIGAStaticTexture();
	~CIGAStaticTexture();

private:
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* name );
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	void D3D9TextureDestory();
	virtual void D3D9StartPlay(){}
	virtual void D3D9StopPlay(){}
	virtual void D3D9PausePlay(){}
	virtual	void GetSize( float* w = NULL, float* h = NULL ){}

private:
	//纹理对象,直接使用基类的对象
};