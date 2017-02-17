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
*	引用头文件
/************************************************************************/

#pragma  once 
//#include "IGATexture.h"
#include <atlbase.h>

#ifdef D3D9TEXTURE_EXPORTS
#define D3D9TEXTURE_API __declspec(dllexport)
#else
#define D3D9TEXTURE_API __declspec(dllimport)
#endif

#ifndef SAFE_DELETE
inline const void SAFE_DELETE(void * p)       { if(p != NULL ) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
inline const void  SAFE_RELEASE(IUnknown * p)      { if(p != NULL ) { (p)->Release(); (p)=NULL; } }
#endif


class CD3D9TextureBase;

class D3D9TEXTURE_API CD3D9ManageTexture
{
public:
	CD3D9ManageTexture():m_pCD3D9TextureBase(NULL) { }
	~CD3D9ManageTexture(){  SAFE_DELETE(m_pCD3D9TextureBase);}
public:
	//m_pCD3D9TextureBase对象在D3D9TextureInit函数中创建
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice,char * pszTextureFileName,int iWidth, int iHeight );
	HRESULT D3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice );
	HRESULT D3D9StartPlay();
	HRESULT D3D9StopPlay();
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	HRESULT D3D9PausePlay();
	HRESULT D3D9UnPausePlay();
	HRESULT D3D9GotoFrame(int iFrame);
	HRESULT D3D9GetCurrentFrame();
	void	D3D9TextureDestory();

private:
	CD3D9ManageTexture& operator =(const CD3D9ManageTexture &texture ){ return (CD3D9ManageTexture &)texture; }; //屏蔽=操作符，防止直接用=赋值
	CD3D9TextureBase *m_pCD3D9TextureBase;
};


//此类是基类，纹理管理类继承此类
class CD3D9TextureBase
{
	friend class CD3D9ManageTexture;
public:
	CD3D9TextureBase() : m_pd3dDevice(NULL), m_pTexture(NULL), m_bIsPlay(false){}
	virtual ~CD3D9TextureBase()
	{  
		m_bIsPlay	= false; 
		m_pd3dDevice = NULL;
		m_pTexture = NULL;
	}

public:
	virtual HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight ){return S_OK;};
	virtual HRESULT D3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice) { return S_OK; };
	virtual void D3D9StartPlay(){ return;}
	virtual void D3D9StopPlay(){ return;}
	virtual LPDIRECT3DTEXTURE9 D3D9GetTexture(){ return NULL;};
	virtual void D3D9PausePlay(){ }
	virtual void D3D9UnPausePlay(){}
	virtual void D3D9GotoFrame(int iFrame){ }
	virtual void D3D9GetCurrentFrame(){}
	virtual void D3D9TextureDestory()
	{
		m_bIsPlay	= false; 
		m_pd3dDevice = NULL;
		m_pTexture = NULL;
	};


public:
	void	SetDevie(LPDIRECT3DDEVICE9 pd3dDevice)	
	{
		if (m_pd3dDevice != NULL )
		{
			m_pd3dDevice->Release();
			m_pd3dDevice = NULL;
		}
		m_pd3dDevice = pd3dDevice ;
	}
	LPDIRECT3DDEVICE9	GetDevice() const						{return m_pd3dDevice;}

	void				SetTexTure(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture;}
	LPDIRECT3DTEXTURE9	GetTexture() const						{ return m_pTexture; }

	void				SetWidth( int iWidth )					{ m_iTexWidth = iWidth;}
	int					GetWidth() const						{ return m_iTexWidth; }

	void				SetHeith( int iHeight)					{ m_iTexHeight = iHeight;}
	int					GetHeight() const						{ return m_iTexHeight ; }

	void				SetbPlay( bool bEnd )					{ m_bIsPlay = bEnd ;}
	bool				GetbPlay( )const						{ return m_bIsPlay; }

	void				SetFileName( char * pszFileName)		{ strcpy( m_szFileName,pszFileName ); }
	LPCSTR				GetFileName()							{ return m_szFileName; }

protected:
	IDirect3DDevice9*		m_pd3dDevice;
	IDirect3DTexture9*		m_pTexture;
	int						m_iTexWidth;
	int						m_iTexHeight;
	bool					m_bIsPlay;                 //是否正在播放,true 是正在播放，flash是停止播放
	char					m_szFileName[MAX_PATH];   //文件全路径
	int						m_iCurrentFrame;
};


//extern "C" D3D9TEXTURE_API void IGATextureDestory();
//extern "C" D3D9TEXTURE_API HRESULT IGATextureInit( LPDIRECT3DDEVICE9 pd3dDevice );
//extern "C" D3D9TEXTURE_API LPDIRECT3DTEXTURE9 IGAGetTexture( int id, bool* bIsPlayEnd, float* w = NULL, float* h = NULL );
//extern "C" D3D9TEXTURE_API int DrawPlayer();
//extern "C" D3D9TEXTURE_API void PlayAVI( LPDIRECT3DDEVICE9 pd3dDevice, char* name);
//extern "C" D3D9TEXTURE_API void PlayFLASH(char* name);
//extern "C" D3D9TEXTURE_API void PausePlayer();
//extern "C" D3D9TEXTURE_API void StopPlayer( DWORD dwID);

