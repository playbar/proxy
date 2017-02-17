#include <d3d8.h>
#include <d3dx8.h>
#include <string>
#include "list"
using namespace std;

//UI面片
class CIGA3DUIElement
{
public:
	CIGA3DUIElement();
	~CIGA3DUIElement();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Render();
	void	Destory();

//基本操作
public:
	//设置大小
	HRESULT SetSize( int width, int height );

	//设置纹理坐标
	void SetTexCoord( int w, int h );	//输入的是纹理的大小

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
	int						m_iWidth,m_iHeight;

	struct _VUI
	{
		float x,y,z,rhw;
		DWORD color;
		float u,v;
	};
#define _D3DFVF_VUI ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

};

//纹理播放器
class CTexture8Play
{
public:
	CTexture8Play();
	~CTexture8Play();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Destroy();
	void PlayAVIFile( char* fileName );
	void PlayFlashFile( char* fileName );
	void Stop();
	void Pause();
	void PlayNext();

	void AddToPlayList( string name );
	
	int ShowPlayer( UINT type );	//在屏幕上显示的类型，显示图像并播放声音；仅显示一个按钮，但播放声音

	void SetPos( int x, int y );

//---------------------------------------------------------------------
protected:
	bool		m_bIsMin;	//是否最小化窗口
	bool		m_bPause;   //是否是暂停状态
	string		m_PlayFileName;	//当前播放的文件名称


//-------------------------------------------------------------------
//基本部件
protected:
	CIGA3DUIElement		m_MediaFrame; //媒体框架，就是外框
	float				m_FrameX, m_FrameY;		//框架的左上角点
	float				m_FrameWidth, m_FrameHeight; //播放器的大小

protected:
	CIGA3DUIElement		m_MediaScreen; //媒体屏幕
	float				m_ScreenX, m_ScreenY;		//媒体屏幕的左上角点
	float				m_ScreenWidth, m_ScreenHeight; //媒体屏幕的大小

protected:
	CIGA3DUIElement		m_MinScreen;  //最小化窗口框架
	float				m_MinX, m_MinY;		
	float				m_MinWidth, m_MinHeight; 

protected:
	CIGA3DUIElement		m_MinButton;  //最小化按钮
	float				m_MinButtonX, m_MinButtonY;		
	float				m_MinButtonWidth, m_MinButtonHeight; 

protected:
	CIGA3DUIElement		m_PlayButton;  //播放、暂停按钮
	float				m_PlayButtonX, m_PlayButtonY;		
	float				m_PlayButtonWidth, m_PlayButtonHeight; 

protected:
	CIGA3DUIElement		m_StopButton;  //停止按钮
	float				m_StopButtonX, m_StopButtonY;		
	float				m_StopButtonWidth, m_StopButtonHeight; 



protected:
	LPDIRECT3DTEXTURE8	m_pTexScreen;

	LPDIRECT3DTEXTURE8  m_pTexBackGround;

	LPDIRECT3DTEXTURE8  m_pFrameTex;	//视屏框架纹理
	LPDIRECT3DTEXTURE8  m_pMinTex;		//框架最小化纹理
	LPDIRECT3DTEXTURE8  m_pMinTex1;		//鼠标选中状态的纹理

	LPDIRECT3DTEXTURE8  m_pMinButtonTex;		//框架最小化纹理
	LPDIRECT3DTEXTURE8  m_pMinButtonTex1;		//鼠标选中状态的纹理

	LPDIRECT3DTEXTURE8  m_pPlayButtonTex;		//播放按钮纹理
	LPDIRECT3DTEXTURE8  m_pPlayButtonTex1;		//

	LPDIRECT3DTEXTURE8  m_pPauseButtonTex;		//暂停按钮纹理
	LPDIRECT3DTEXTURE8  m_pPauseButtonTex1;		//

	LPDIRECT3DTEXTURE8  m_pStopButtonTex;		//停止按钮纹理
	LPDIRECT3DTEXTURE8  m_pStopButtonTex1;		//

//-------------------------------------------------------------------
//底层对象
protected:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	list<string>			m_PlayList;
	list<string>::iterator	m_pNode;
	bool					m_bIsPlay;		//是否在播放

	CDirectXVideoTexture	m_VideoTexture;
	FlashSprite				m_FlashTexture;

	int						m_nFlag;	//播放类型标志

};

extern CTexture8Play g_CTexture8Play;
