#pragma once
#include <d3d9.h>
#include <tchar.h>
#include <strsafe.h>
#include <atlbase.h>

#include "ToolFunction.h"

#define MAX_CLIP_RECT_NUM 256

struct CUSTOMVERTEX
{
	FLOAT x, y, z;    //顶点位置  
	FLOAT u,v ;		  //顶点纹理坐标
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ | D3DFVF_TEX1 )

class CShowPic
{
public:
	CShowPic(void);
	~CShowPic(void);

public:
	void	SetDevice9(LPDIRECT3DDEVICE9 pD3DDevice){m_spD3DDevice = pD3DDevice;}
	LPDIRECT3DDEVICE9 GetDevice9() const	{ return m_spD3DDevice; }
	void	SetGameWnd(HWND hGameWnd)		{ m_hGameWnd = hGameWnd; }
	HWND	GetGameWnd() const				{ return m_hGameWnd; }
	bool	GetbShowSearch()const			{ return m_bShowSearch; }
	void	SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_spTextureSearch = pTexture; }
	LPDIRECT3DTEXTURE9	GetTexture( )	{ return m_spTextureSearch; }

	void DrawRect( );

	HRESULT InitPic( IDirect3DDevice9 * pD3DDevice );
	HRESULT InitTranslationMatrix();
	HRESULT RenderPic();
	HRESULT DestroyPic();
	bool CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ScreenPosToWorldPos(const HWND hWnd,const D3DXMATRIXA16 &matProj,const D3DXMATRIXA16& matView,int x, int y, D3DXVECTOR3 &vWorldPos);

	void GetRay(HWND hWnd, D3DXMATRIXA16 &matProj, D3DXMATRIXA16& matView, D3DXVECTOR3 vPos, int x, int y, LPRAY ray );

	bool  RayCheck(D3DXVECTOR3 vMin,D3DXVECTOR3 vMax,float fScaling, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir);

	void AppVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans );

	LPSTR GetPicFileName()  { return m_szPicFileName; }

	int GetixPos(){ return m_ixPos; }
	int GetiyPos(){ return m_iyPos;	}
	int GetiWidth()	{ return m_iWidth; }
	int GetiHeight(){ return m_iHeight; }

public:
	//RECT m_Rect1;
	//RECT m_Rect2;
	//RECT m_Rect3;

	//CUSTOMVERTEX customerVertices[4];

	float m_fU1, m_fV1;
	float m_fU2, m_fV2;
	float m_fU3, m_fV3;
	float m_fU4, m_fV4;

	int		m_iCurrentFrame;
	
	float m_fPicWidth;
	float m_fPicHeight;
	bool m_bIsMove;

public:
	volatile bool			m_bProxyMsg; //消息是否被接管 

private:
	D3DXMATRIXA16			m_matWorld;		
	D3DXMATRIXA16			m_matView;
	D3DXMATRIXA16			m_matProj;
	IDirect3DDevice9*		m_spD3DDevice;
	IDirect3DTexture9*      m_spTextureSearch;      //纹理
	IDirect3DVertexBuffer9* m_spVBSearchPic;      //顶点缓冲区
	volatile bool			m_bMouseOver;
	volatile bool			m_bShowSearch;
	bool					m_bMouseBtnDown;

	HWND					m_hGameWnd;

	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 m_vEyePt;
	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;

	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;

	int m_ixPos; //图片显示的位置
	int m_iyPos; 
	int m_iWidth;	//显示图片的高宽
	int m_iHeight;

	char m_szPicFileName[MAX_PATH];//文件全路径
	char m_szVideoFileName[MAX_PATH];

private:
	RECT m_rectTexture[MAX_CLIP_RECT_NUM];
	UINT m_nMouseLeave[MAX_CLIP_RECT_NUM];
	UINT m_nMouseL;
	UINT m_nMouseOver[MAX_CLIP_RECT_NUM];
	UINT m_nMouseO;
	UINT m_nMouseDown[MAX_CLIP_RECT_NUM];
	UINT m_nMouseD;
};

extern CShowPic	 *g_pShowPic;


