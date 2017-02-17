/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGATexture9.h
*	功能描述：IGATextur9, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：    
*	修改：重新设计接口，目的能够实现灵活播放纹理数据，通过传入文件名的方式修改
*	修改者：	hgl
*	修改日期： 2009-01-19
/************************************************************************/

#pragma  once 

//extern LPDIRECT3DDEVICE8	g_pd3dDevice;

HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice );
HRESULT D3D9StartPlay(char * pszTextureFileName);
HRESULT D3D9StopPlay();
LPDIRECT3DTEXTURE9 D3D9GetTexture( int id, bool* bIsPlayEnd, float* w = NULL, float* h = NULL );
HRESULT D3D9PausePlay();
HRESULT D3D9UnPausePlay();
HRESULT D3D9GotoFrame(int iFrame);
HRESULT D3D9GetCurrentFrame();
void	D3D9TextureDestory();

//////////////////////////////////////////////////////////////////////////

int     DrawPlayer();
void	PlayAVI( LPDIRECT3DDEVICE9 pd3dDevice, char* name);
void	PlayFLASH(char* name);
void	PausePlayer();
void	StopPlayer( DWORD dwID);