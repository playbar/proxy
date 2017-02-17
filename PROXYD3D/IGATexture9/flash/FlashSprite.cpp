
#pragma  once

#include "stdafx.h"

#include "FlashSprite.h"
#include "FlashPlayer.h"

double FlashSprite::GetFlashVersion() {
	return FlashPlayer::GetFlashVersion();
}

FlashSprite::FlashSprite(void) :
	m_pTextureTarget(NULL),
	m_pFlashPlayer(NULL),
	m_nWidth(0),
	m_nHeight(0)
{

}

FlashSprite::~FlashSprite(void)
{
	Destory();
}

void FlashSprite::Destory()
{

	if (m_pFlashPlayer != NULL)
	{
		delete m_pFlashPlayer;
		m_pFlashPlayer = NULL;
	}

	if ( m_pTextureTarget != NULL )
	{
		m_pTextureTarget->Release();
		m_pTextureTarget = NULL;
	}

}

BOOL FlashSprite::StartFlash(IDirect3DDevice9* pD3Device, LPCSTR lpszFileName, UINT nWidth, UINT nHeight)
{

	// 创建纹理
	if ( m_pTextureTarget != NULL )
	{
		m_pTextureTarget->Release();
		m_pTextureTarget = NULL;
	}
	if(FAILED(D3DXCreateTexture( pD3Device, nWidth, nHeight, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTextureTarget)))
		return FALSE;

	if (m_pTextureTarget == NULL)
		return FALSE;

	m_nWidth  = nWidth;
	m_nHeight = nHeight;

	// 创建FLASH播放器
	if (m_pFlashPlayer == NULL)
	{
		m_pFlashPlayer = new FlashPlayer;
		if (m_pFlashPlayer == NULL)
			return FALSE;
		if ( FAILED( m_pFlashPlayer->InitFlashOCX() ))
		{
			delete m_pFlashPlayer;
			return FALSE;
		}
	}

	/*char sAppPath[MAX_PATH];
	if (strlen(lpszFileName) >= 2 && lpszFileName[1]==':')
	{
		strcpy(sAppPath, lpszFileName);
	}
	else
	{
		GetModuleFileName(GetModuleHandle(NULL), sAppPath, sizeof(sAppPath));
		char *  lpTmp= strrchr(sAppPath, '\\');
		if (lpTmp)
		{
			*(lpTmp + 1) = 0;
		}
		strcat(sAppPath, lpszFileName);
	}*/

	m_pFlashPlayer->StartAnimation(lpszFileName, nWidth, nHeight, NULL);
	m_pFlashPlayer->SetQuality(FlashPlayer::QUALITY_HIGH);
	m_pFlashPlayer->GotoFrame(0);

	return TRUE;
}
void FlashSprite::SetQuality(BYTE byQuality)
{
	if (m_pFlashPlayer)
	{
		if (byQuality == 0)
			m_pFlashPlayer->SetQuality(FlashPlayer::QUALITY_LOW);
		else if (byQuality == 1)
			m_pFlashPlayer->SetQuality(FlashPlayer::QUALITY_MEDIUM);
		else
			m_pFlashPlayer->SetQuality(FlashPlayer::QUALITY_HIGH);
	}
}

bool FlashSprite::IsPlaying()
{
	if (m_pFlashPlayer)
		return m_pFlashPlayer->IsPlaying() == true;
	return false;
}
void FlashSprite::Pause()
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->Pause();
}
void FlashSprite::Unpause()
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->Unpause();

}
void FlashSprite::Back()
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->Back();

}
void FlashSprite::Rewind()
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->Rewind();

}
void FlashSprite::Forward()
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->Forward();

}
void FlashSprite::GotoFrame(int nFrame)
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->GotoFrame(nFrame);

}
int FlashSprite::GetCurrentFrame()
{
	if (m_pFlashPlayer)
		return m_pFlashPlayer->GetCurrentFrame();
	return -1;
}
int FlashSprite::GetTotalFrames()
{
	if (m_pFlashPlayer)
		return m_pFlashPlayer->GetTotalFrames();
	return -1;
}
BOOL FlashSprite::GetLoopPlay()
{
	if (m_pFlashPlayer)
		return m_pFlashPlayer->GetLoopPlay();
	return FALSE;
}
void FlashSprite::SetLoopPlay(BOOL bLoop)
{
	if (m_pFlashPlayer)
		m_pFlashPlayer->SetLoopPlay(bLoop);
}
BOOL FlashSprite::Update()
{
	if (m_pFlashPlayer == NULL)
		return FALSE;
	
	m_pFlashPlayer->Update();
	if (m_pFlashPlayer->Render())
	{
		INT TexturePitch;
		UINT nWidth  = m_nWidth;
		UINT nHeight = m_nHeight;

		DWORD* pdwFlashData = (DWORD*)m_pFlashPlayer->GetFlashFrameBuffer();
		DWORD* pdwRenderTarget;

		D3DLOCKED_RECT LockedRect;
		if (FAILED(m_pTextureTarget->LockRect(0, &LockedRect, 0, 0)))
			return FALSE;

		pdwRenderTarget = (DWORD*)LockedRect.pBits;
		TexturePitch = LockedRect.Pitch >> 2;

		for (UINT y = 0; y < nHeight; y++) 
		{
		//	memcpy(&pdwRenderTarget[y*tex_w], &pdwFlashData[y * nWidth], sizeof(DWORD)*nWidth);
			CopyMemory(pdwRenderTarget, pdwFlashData, nWidth << 2);
			pdwRenderTarget += TexturePitch;
			pdwFlashData += nWidth;
		}

		if (FAILED(m_pTextureTarget->UnlockRect(0)))
			return FALSE;

		return TRUE;
	}
	return FALSE;
}

IDirect3DTexture9* FlashSprite::GetRenderTarget()
{
	return m_pTextureTarget;
}