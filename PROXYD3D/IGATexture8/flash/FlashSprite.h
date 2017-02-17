//////////////////////////////////////////////////////////////////////////////////////
// class FlashSprite
//////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <d3dx8.h>

typedef IDirect3DTexture8 TEXTURE;
typedef IDirect3DDevice8  D3DEVICE;

class FlashPlayer;

class FlashSprite
{
public:
	FlashSprite(void);
	virtual ~FlashSprite(void);

public:
	static double			GetFlashVersion();

	BOOL					StartFlash(D3DEVICE* pD3Device, LPCSTR lpszFileName, UINT nWidth, UINT nHeight);
	void					SetQuality(BYTE byQuality);

	bool					IsPlaying();	
	void					Pause();
	void					Unpause();
	void					Back();
	void					Rewind();
	void					Forward();	
	void					GotoFrame(int nFrame);

	int						GetCurrentFrame();
	int						GetTotalFrames();

	BOOL					GetLoopPlay();
	void					SetLoopPlay(BOOL bLoop);

	BOOL					Update();

	TEXTURE*                GetRenderTarget();

	void					Destory();

public:
	UINT			m_nWidth;
	UINT 			m_nHeight;

protected:
	TEXTURE*        m_pRenderTarget;
	FlashPlayer*	m_pFlashPlayer;

};
