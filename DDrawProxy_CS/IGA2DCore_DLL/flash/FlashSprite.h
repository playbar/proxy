//////////////////////////////////////////////////////////////////////////////////////
// class FlashSprite
//////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <d3dx9.h>
#include "IGA_IDirectDrawSurface.h"

class FlashPlayer;

extern IGA_IDirectDrawSurface7		*g_pIDirectDrawSurface7;

class FlashSprite
{
public:
	FlashSprite(void);
	virtual ~FlashSprite(void);

public:
	static double			GetFlashVersion();

	BOOL					StartFlash(/*D3DEVICE* pD3Device,*/ LPCSTR lpszFileName, UINT nWidth, UINT nHeight);
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

	//TEXTURE*                GetRenderTarget();

protected:
	//TEXTURE*        m_pRenderTarget;
	FlashPlayer*	m_pFlashPlayer;
	UINT			m_nWidth;
	UINT 			m_nHeight;
};
