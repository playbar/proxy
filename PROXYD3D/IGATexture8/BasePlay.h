#pragma  once 

#include <d3d8.h>
#include <d3dx8.h>

class CBasePlay
{
public:
	virtual void PlayAVIFile( LPDIRECT3DDEVICE8 pd3dDevice, char* AVIFileName ){}
	virtual void PlayFlashFile( LPDIRECT3DDEVICE8 pd3dDevice, char* SWFFileName ){}
	virtual void FrameMove(){}
	virtual void Destory(){}

public:
	static CBasePlay* GetPlayerInstance();
protected:
	static CBasePlay*	m_Instance;
};

class CBackGroundPlayer : public CBasePlay
{
public:
	CBackGroundPlayer();
	~CBackGroundPlayer();

public:
	void	PlayAVIFile( LPDIRECT3DDEVICE8 pd3dDevice, char* AVIFileName );
	void    PlayFlashFile( LPDIRECT3DDEVICE8 pd3dDevice, char* SWFFileName );
	void	FrameMove();
	void	Destory();
	void	Stop();

protected:
	CDirectXVideoTexture	m_VideoPlayer;	//提取视屏到纹理
	FlashSprite				m_FlashTexture; //提取flash图像

	bool CheckDevice();
	bool m_bDrawBuf;

	LPDIRECT3DTEXTURE8 GetMediaInfor();	//根据目前播放的类型，获得相应的媒体信息
	void DrawToBkBuf( LPDIRECT3DTEXTURE8 pTexture );
	void DrawToMesh( LPDIRECT3DTEXTURE8 pTexture );

	int		m_nPlayType;

	bool	m_bPlayIsEnd;	//当前媒体是否播放完成

	int		m_X;
	int		m_Y;

	int		m_nMediaWidth;
	int		m_nMediaHeight;


	LPDIRECT3DDEVICE8	m_pd3dDevice;
	LPDIRECT3DSURFACE8	m_pBackGroundBuff;
	D3DSURFACE_DESC		m_BackGroundBufDesc;

//------------------------------------------
//使用网格绘制，有些游戏不能锁定后备缓冲区
protected:
	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	struct _SVB
	{
		float x, y, z, rhw;
		float u, v;
		const static DWORD FVF = D3DFVF_XYZRHW|D3DFVF_TEX1;
	};

private:
	RECT	m_LockRect;
	POINT	m_kCursor;

	int __x;
	int __y;
	bool _bLB;

	void	ComputeCursor();
};

