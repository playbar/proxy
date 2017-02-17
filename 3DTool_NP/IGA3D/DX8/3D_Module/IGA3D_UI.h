#include "d3d8.h"
#include "d3dx8.h"

//UI面片
class CIGA3D_UI_Element
{
public:
	CIGA3D_UI_Element();
	~CIGA3D_UI_Element();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Render();
	void	Destory();

//基本操作
public:
	//设置大小
	HRESULT SetSize( int width, int height );

	//设置位置
	HRESULT SetPosition( int x, int y );

	//设置使用的纹理
	void SetTexture( LPDIRECT3DTEXTURE8 pTex );

	bool IsInSide( int x, int y );


public:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;			//使用的四边形
	LPDIRECT3DTEXTURE8		m_pTex;			//使用的纹理

	int						m_x,m_y;
	int						m_width,m_height;

	struct _VUI
	{
		float x,y,z,rhw;
		DWORD color;
		float u,v;
	};
#define _D3DFVF_VUI ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

};

CIGA3D_UI_Element* GetUI();

//------------------------------------------------------
//UI纹理
class CUI_Tex
{
public:
	CUI_Tex();
	~CUI_Tex();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	LPDIRECT3DTEXTURE8 GetTexture( int id );

	void Destory();

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DTEXTURE8      m_pTex;
	LPDIRECT3DTEXTURE8      m_pTex1;
    LPDIRECT3DTEXTURE8      m_pTex2;

	LPDIRECT3DTEXTURE8      m_pTex3;
    LPDIRECT3DTEXTURE8      m_pTex4;
};

CUI_Tex* GetUITexture();

//---------------------------------------------
//按钮对象
typedef void(*LPFUN)();
class CButtom
{
public:
	CButtom();
	~CButtom();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, LPFUN pFun = 0 );
	
	void	Draw( int x, int y );	//鼠标位置

	void	Destory();

public:
	void	SetPosition( int x, int y );
	void	SetTextureID( int tex_id = 1 );
	void	SetProcFun( LPFUN pFun );	//设置按钮的事件处理函数

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;

	CIGA3D_UI_Element		m_Buttom;		//按钮图形对象

	LPFUN					m_pProcFun;		//按钮被点击后的处理函数

private:
	int						m_TexID;	
};

void CallBackProcFun();
void CallBackProcFun1();


//-----------------------------------------------------
//
struct _UIElement
{
	DWORD	id;
	int		tex_id;	//使用的纹理ID
	RECT	rect;
	
	_UIElement*  pFirstChildNode; //子节点
	_UIElement*  pSiblingNode;    //兄弟节点

	_UIElement()
	{
		id		= 0;
		tex_id	= 0;
		rect.top		= 0;
		rect.left		= 0;
		rect.right		= 30;
		rect.bottom		= 50;

		pFirstChildNode = NULL;
		pSiblingNode	= NULL;
	}

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

	_UIElement*				m_pFrame;	//对话框的主框架

	CIGA3D_UI_Element		m_Frame;	//框架
	
	CButtom					m_ButtomOK;	//OK按钮
	CButtom					m_ButtomCancel;	//OK按钮

};
IGA3DDialog* GetDialog();

//------------------------------------------------
//消息事件
#include <queue>
#include <functional>

enum IGA3D_MSG
{
	IGA3D_MSG_UNKNOW = -1,
	IGA3D_MSG_LBUTTONDOWN,
	IGA3D_MSG_LBUTTONUP,
	IGA3D_MSG_RBUTTONDOWN,
	IGA3D_MSG_RBUTTONUP,

};



struct SIGA3DMSG
{
	bool	bIsProc;	//是否被处理过
	DWORD	type;		//消息的类型
	int     key;	    //键值

	int		x;		//鼠标的位置
	int		y;

	SIGA3DMSG()
	{
		bIsProc = false;
		type	= IGA3D_MSG_UNKNOW;
		key		= IGA3D_MSG_UNKNOW;
		x		= 0;
		y       = 0;
	}

	SIGA3DMSG( const SIGA3DMSG& obj )
	{
		bIsProc = obj.bIsProc;
		type	= obj.type;
		key		= obj.key;
		x		= obj.x;
		y       = obj.y;
	}

	~SIGA3DMSG()
	{
	
	}


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
	typedef queue<SIGA3DMSG>	IGAMSG;
	IGAMSG		m_Msg;

public:
	int		m_xCursor;
	int		m_yCursor;

};
CIGA3DMessageManager* GetMessageManager();

