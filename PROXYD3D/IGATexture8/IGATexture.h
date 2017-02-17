#pragma  once

#include "d3d8.h"
#include "d3dx8.h"
#include <list>
#include <map>
#include <set>
using namespace std;

class FlashSprite;
class CIGATextureBase;

bool GetModulePath( char * szPathName );

enum TEXTURE_FILE_TYPE
{
	TEXTURE_FILE_SWF = 1,
	TEXTURE_FILE_AVI,
	TEXTURE_FILE_MPG,
	TEXTURE_FILE_WMV,
	TEXTURE_FILE_DDS,
};

class CIGATexture
{
public:
	CIGATexture();
	~CIGATexture();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	LostDevice();	
	void	ResumeDevice();		
	void	ReleaseResource();		
	void	AddResource( DWORD id );	//根据ID添加资源
	void	RemoveResource( DWORD id );			//根据ID移除资源
	LPDIRECT3DTEXTURE8 GetTexture( DWORD id, bool* bIsPlayEnd, float* w = NULL, float* h = NULL );
	void StopPlay( DWORD dwID);
	void Destroy();
	void StartPlay();
	void Pause();

private:
	LPDIRECT3DDEVICE8	m_pD3DDevice;
	LPDIRECT3DTEXTURE8	m_pD3DTexture;

private:
	typedef pair<DWORD, CIGATextureBase*>	MAP_TEXTURE_NODE;
	multimap<DWORD, CIGATextureBase*>			m_mapTexLibrary;	//保存网格的容器
	multimap<DWORD, CIGATextureBase*>::iterator	m_mapIterator;

	//设备丢失、恢复时使用
	list<DWORD>				m_listTexIDLibrary;		//在设备丢失时保存当前使用的网格信息的ID,在设备恢复的时候来重新加载他们
	list<DWORD>::iterator	m_listIterator;

	set<DWORD>				m_setErrorID;		//下面的对象用来记录加载时失败的资源ID,当加载失败的ID时候，就跳过不执行加载操作
	set<DWORD>::iterator	m_setIterator;	
};

extern CIGATexture g_CIGATexture;

class CIGATextureBase
{
public:
	CIGATextureBase() : m_pd3dDevice(NULL), m_pTexture(NULL), m_bIsPlayEnd(false)
	{
	}
	virtual ~CIGATextureBase()
	{
		m_pd3dDevice	= NULL;
		m_pTexture		= NULL;
		m_bIsPlayEnd	= false;
	}

public:

	virtual HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, char* name ){return S_OK;};
	virtual LPDIRECT3DTEXTURE8 GetTexture(){ return NULL;};
	virtual void Destroy(){};
	virtual void StartPlay(){ return;}
	virtual void StopPlay(){ return;}
	virtual void Pause(){ return;}
	virtual	void GetSize( float* w = NULL, float* h = NULL ){}

protected:
	
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DTEXTURE8		m_pTexture;
	int						m_iTexWidth;
	int						m_iTexHeight;
	char					m_szName[MAX_PATH];

public:
	bool					m_bIsPlayEnd;
};
//flash纹理 
class CIGAFlashTexture : public CIGATextureBase
{
public:
	CIGAFlashTexture();
	~CIGAFlashTexture();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, char* name );
	LPDIRECT3DTEXTURE8 GetTexture();
	void Destroy();
	virtual void StartPlay();
	virtual void StopPlay();
	virtual void Pause(){ return;}
	virtual	void GetSize( float* w = NULL, float* h = NULL );

private:
	FlashSprite		m_FlashSprite;
};

//-------------------------------------------------------------
//视屏纹理
class CIGAVideoTexture : public CIGATextureBase
{
public:
	CIGAVideoTexture();
	~CIGAVideoTexture();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, char* name );
	LPDIRECT3DTEXTURE8 GetTexture();
	void Destroy();
	virtual void StartPlay();
	virtual void StopPlay();
	virtual void Pause();
	virtual	void GetSize( float* w = NULL, float* h = NULL );

private:
	CDirectXVideoTexture	m_VideoTexture;
};

//--------------------------------------------------------------
//静态纹理
class CIGAStaticTexture : public CIGATextureBase
{
public:
	CIGAStaticTexture();
	~CIGAStaticTexture();

private:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, char* name );
	LPDIRECT3DTEXTURE8 GetTexture();
	void Destroy();
	virtual void StartPlay(){}
	virtual void StopPlay(){}
	virtual void Pause(){}
	virtual	void GetSize( float* w = NULL, float* h = NULL ){}

private:
	//纹理对象,直接使用基类的对象
};