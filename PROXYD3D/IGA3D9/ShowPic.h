#pragma once

//#include "ximage.h"

typedef struct tagRAY
{
	D3DXVECTOR3	vOrig;
	D3DXVECTOR3 vVector;
}RAY,*LPRAY;

struct CUSTOMVERTEX
{
	FLOAT x, y, z, w;    //顶点位置  
	FLOAT u,v ;		  //顶点纹理坐标
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZRHW | D3DFVF_TEX1 )

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

	HRESULT InitPic();
	HRESULT InitTranslationMatrix();
	HRESULT RenderPic();
	HRESULT DestroyPic();
	LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ScreenPosToWorldPos(const HWND hWnd,const D3DXMATRIXA16 &matProj,const D3DXMATRIXA16& matView,int x, int y, D3DXVECTOR3 &vWorldPos);

	void GetRay(HWND hWnd, D3DXMATRIXA16 &matProj, D3DXMATRIXA16& matView, D3DXVECTOR3 vPos, int x, int y, LPRAY ray );

	bool  RayCheck(D3DXVECTOR3 vMin,D3DXVECTOR3 vMax,float fScaling, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir);

	void AppVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans );

	LPSTR GetPicFileName()  { return m_szPicFileName; }

private:
	D3DXMATRIXA16			m_matWorld;		
	D3DXMATRIXA16			m_matView;
	D3DXMATRIXA16			m_matProj;
	CComPtr<IDirect3DDevice9>		m_spD3DDevice;
	CComPtr<IDirect3DTexture9>      m_spTextureSearch;      //纹理
	CComPtr<IDirect3DVertexBuffer9> m_spVBSearchPic;      //顶点缓冲区
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
	int m_iWidth;
	int m_iHeight;
	char m_szPicFileName[MAX_PATH];//视频文件全路径

	//CxImage *m_image;		

};


