
#include "d3d8.h"
#include "d3dx8.h"
#include "pack.h"
//----------------------------------------------------------------------------------------
//纹理类
class CTexture
{
public:
	CTexture();
	~CTexture();

public:
	HRESULT Init( IDirect3DDevice8 *pd3dDevice, char* fileName, CPack* pPack  );

	LPDIRECT3DTEXTURE8 GetTexture();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	LPDIRECT3DTEXTURE8	m_pTexture;

};

//--------------------------------------------------------------------
//纹理库
class CTexLibrary
{
public:
	CTexLibrary();
	~CTexLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Lost();	//设备丢失
	void	Resume();	//设备恢复

	LPDIRECT3DTEXTURE8 GetTexture( DWORD id );

	void	Destory();	//释放资源

private:
	void	AddResource( DWORD id );	//根据ID添加资源
	void	ReMove( DWORD id );			//根据ID移除资源

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	typedef map<DWORD, CTexture*>		TEX_LIBRARY;
	typedef pair<DWORD, CTexture*>		TEX_NODE;

	TEX_LIBRARY						m_TexLibrary;	//保存网格的容器
	map<DWORD, CTexture*>::iterator	m_pNode;

	LPDIRECT3DTEXTURE8				m_pTex;

//设备丢失、恢复时使用
private:
	typedef list<DWORD>		TEX_ID;
	TEX_ID					m_TexIDLibrary;	//在设备丢失时保存当前使用的网格信息的ID
												//在设备恢复的时候来重新加载他们

	list<DWORD>::iterator	m_pNodeID;

//下面的对象用来记录加载时失败的资源ID
//当加载失败的ID时候，就跳过不执行加载操作
private:
	typedef set<DWORD>			TEX_ERROR_ID;
	TEX_ERROR_ID				m_ErrorID;

	set<DWORD>::iterator		m_pErrorNode;	

private:
	CPack						m_Pack;

};

CTexLibrary* GetTexLibrary();