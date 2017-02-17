//////////////////////////////////////////////////////////////////////////////////////
// class FlashSprite
//////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <d3dx9.h>
#include <ddraw.h>
#include "FlashPlayer.h"
//typedef IDirect3DTexture9 TEXTURE;
//typedef IDirect3DDevice9  D3DEVICE;

//class FlashPlayer;

class FlashSprite
{
public:
	FlashSprite(void);
	virtual ~FlashSprite(void);

public:
	static double			GetFlashVersion();

	BOOL					StartFlash(/*IDirectDraw7* pIDirectDraw,*/ LPCSTR lpszFileName, UINT nWidth, UINT nHeight);
	void					SetQuality(BYTE byQuality);

	BOOL					IsPlaying();	
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

public:
	void *			m_pFlashData;
	//IDirectDrawSurface7 * m_pIDirectDrawSurface7;
	FlashPlayer*	m_pFlashPlayer;
	UINT			m_nWidth;
	UINT 			m_nHeight;
};
