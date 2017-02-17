#pragma  once

#include "d3d8.h"
#include "d3dx8.h"
#include <queue>
using namespace std;

#include <functional>

void CallBackProcFun();
void CallBackProcFun1();



class CIGA3DUIElement
{
public:
	CIGA3DUIElement();
	~CIGA3DUIElement();
public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Render();
	void	Destory();

	HRESULT SetSize( int width, int height );
	HRESULT SetPosition( int x, int y );
	void SetTexture( LPDIRECT3DTEXTURE8 pTex );
	bool IsInSide( int x, int y );

public:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;			//使用的四边形
	LPDIRECT3DTEXTURE8		m_pTex;			//使用的纹理
	int						m_x;
	int						m_y;
	int						m_iWidth;
	int						m_iHeight;

	struct _VUI
	{
		float x,y,z,rhw;
		DWORD color;
		float u,v;
	};
#define _D3DFVF_VUI ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

};

class CUITexture
{
public:
	CUITexture();
	~CUITexture();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	LPDIRECT3DTEXTURE8 GetTexture( int id );
	void Destory();

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DTEXTURE8      m_pTexUI;
	LPDIRECT3DTEXTURE8      m_pTexOK;
    LPDIRECT3DTEXTURE8      m_pTexOk1;
	LPDIRECT3DTEXTURE8      m_pTexCancel;
    LPDIRECT3DTEXTURE8      m_pTexCancel1;
};



class CIGAUIButton
{
public:
	typedef void(*LPFUN)();
	CIGAUIButton();
	~CIGAUIButton();

public:

	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, LPFUN pFun = 0 );
	void	Draw( int x, int y );	//鼠标位置
	void	Destory();

	void	SetPosition( int x, int y );
	void	SetTextureID( int tex_id = 1 );
	void	SetProcFun( LPFUN pFun );	//设置按钮的事件处理函数

private:
	
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	CIGA3DUIElement			m_UIElement;	//按钮图形对象
	LPFUN					m_pProcFun;		//按钮被点击后的处理函数
	int						m_iTexID;	
};



class IGA3DDialog
{
public:
	IGA3DDialog();
	~IGA3DDialog();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void    Render();
	void	Destory();

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	SUIElement*				m_pFrame;	//对话框的主框架
	CIGA3DUIElement		m_Frame;	//框架

	CIGAUIButton					m_ButtomOK;	//OK按钮
	CIGAUIButton					m_ButtomCancel;	//OK按钮

};


class CIGA3DMessageManager
{
public:
	CIGA3DMessageManager();
	~CIGA3DMessageManager();

public:
	HRESULT Init();
	void    Destory();
	void	UpdataMessage();
	bool    GetIGA3DMessage( SIGA3DMSG* msg );	//从消息队列里得到一个消息
	void	SendIGA3DMessage( SIGA3DMSG msg );	//把消息发送到消息队列里
	void	RemoveMsg();//移除顶部消息
	void	SetMsgManagerState( DWORD state );	//设置管理器状态
private:
	queue<SIGA3DMSG>		m_queueMsg;

public:
	int		m_xCursor;
	int		m_yCursor;

};

extern CUITexture			g_CUITexture;
extern CIGA3DMessageManager g_CIGA3DMessageManager;


