#pragma once
#include "d3d8.h"

struct CUSTOMVERTEX
{
	FLOAT x, y, z;    //顶点位置  
	FLOAT u,v ;		  //顶点纹理坐标
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)

class CShowPic
{
public:
	CShowPic(void);
	~CShowPic(void);

public:
	void	SetDevice8(LPDIRECT3DDEVICE8 pD3DDevice){m_pD3DDevice = pD3DDevice;}
	LPDIRECT3DDEVICE8 GetDevice8() const	{ return m_pD3DDevice; }
	void	SetGameWnd(HWND hGameWnd)		{ m_hGameWnd = hGameWnd; }
	HWND	GetGameWnd() const				{ return m_hGameWnd; }
	bool	GetbShowSearch()const			{ return m_bShowSearch; }
	void	SetTexture(LPDIRECT3DTEXTURE8 pTexture) { m_pTextureSearchOver = pTexture; }

	HRESULT InitPic();
	HRESULT InitTranslationMatrix();
	HRESULT RenderPic();
	HRESULT DestroyPic();
	LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	D3DXMATRIXA16			m_matWorld;		
	D3DXMATRIXA16			m_matView;
	D3DXMATRIXA16			m_matProj;
	LPDIRECT3DDEVICE8		m_pD3DDevice;
	//LPDIRECT3DTEXTURE8      m_pTextureSearchleave;      //纹理
	LPDIRECT3DTEXTURE8      m_pTextureSearchOver;      //纹理
	LPDIRECT3DVERTEXBUFFER8 m_pVBSearchPic;      //顶点缓冲区
	volatile bool			m_bMouseOver;
	volatile bool			m_bShowSearch;
	HWND					m_hGameWnd;

	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 m_vEyePt;
	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;

	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;

	int m_ixPos; //图片显示的位置
	int m_iyPos;
	char m_szPicFileName[MAX_PATH];

};


