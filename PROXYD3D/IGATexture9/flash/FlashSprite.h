//////////////////////////////////////////////////////////////////////////////////////
// class FlashSprite
//////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <d3d9.h>

//typedef IDirect3DTexture9 TEXTURE;
//typedef IDirect3DDevice9  D3DEVICE;

class FlashPlayer;

class FlashSprite
{
public:
	FlashSprite(void);
	virtual ~FlashSprite(void);

public:
	static double			GetFlashVersion();

	BOOL					StartFlash(IDirect3DDevice9* pD3Device, LPCSTR lpszFileName, UINT nWidth, UINT nHeight);
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

	IDirect3DTexture9*      GetRenderTarget();

	void					Destory();

public:
	UINT			m_nWidth;
	UINT 			m_nHeight;

protected:
	IDirect3DTexture9*  m_pTextureTarget;
	FlashPlayer*		m_pFlashPlayer;

};
