#pragma  once 

#include "d3d8.h"
#include "d3dx8.h"
#include <map>
#include <list>
#include <set>
#include "pack.h"
using namespace std;
//注意：资源要创建在内存中
//-----------------------------------------------
class CFace
{
public:
	CFace();
	~CFace();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	HRESULT SetPosition( int x = 0, int y = 0, int size_x = 100, int size_y = 100 );
	void	SetTexture( LPDIRECT3DTEXTURE8 pTexture );

	void Render();

	void Destory();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	struct _vf
	{
		float x,y,z,w;
		float u,v;
	};
	#define D3D_FVF ( D3DFVF_XYZRHW|D3DFVF_TEX1 ) 

	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	LPDIRECT3DTEXTURE8			m_pTex;
	D3DMATERIAL8				m_Material;



};

CFace* GetCFace();

//--------------------------------------------------------
//UI的纹理库
class CUITexture
{
public:
	CUITexture();
	~CUITexture();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	LPDIRECT3DTEXTURE8 GetTexture( DWORD id );

	

	void	Lost();	//设备丢失
	void	Resume();	//设备恢复

//---------------------------------------------------
//添加或移除
private:
	void AddResource( DWORD id );
	void ReMove( DWORD id );	//根据ID移除资源
	void Destory();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	LPDIRECT3DTEXTURE8	m_pTex;

//--------------------------------------------------
//纹理库 
private:
	typedef map<DWORD, LPDIRECT3DTEXTURE8>	UI_TEXTURE;
	typedef pair<DWORD, LPDIRECT3DTEXTURE8> UI_TEX_NODE;

	UI_TEXTURE		m_UITexLibrary;	//纹理库
	map<DWORD, LPDIRECT3DTEXTURE8>::iterator	m_pNode;

//--------------------------------------------------
//设备丢失、恢复时使用
private:
	typedef list<DWORD>		TEX_ID;
	TEX_ID					m_TexIDLibrary;

	list<DWORD>::iterator	m_pTexID;

//--------------------------------------------------
//下面的对象用来记录加载时失败的资源ID
//当加载失败的ID时候，就跳过不执行加载操作
private:
	typedef set<DWORD>		TEX_ERROR_ID;
	TEX_ERROR_ID			m_ErrorID;

	set<DWORD>::iterator	m_pErrorNode;

private:
	CPack					m_Pack;
};

CUITexture* GetUITexture();

//-------------------------------------------------------------------
//UI
class CUI
{
public:
	CUI();
	~CUI();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	void Render();
	
	void	Lost();	//设备丢失
	void	Resume();	//设备恢复

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

};

CUI* GetUI();

//--------------------------------------------------------------------
//UI对话框
class CUIDialog
{
public:
	CUIDialog();
	~CUIDialog();

public:
	DWORD Render( POINT* pPoint, DWORD id );	//绘制对话框

private:
	POINT	m_Point;	//当前鼠标的位置，用来判断对话框事件

};

DWORD ShowDlg( POINT* pPoint, DWORD id );