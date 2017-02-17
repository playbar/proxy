//#include "StdAfx.h"
#include "FlashSprite.h"
#include "FlashPlayer.h"


double FlashSprite::GetFlashVersion() {
	return FlashPlayer::GetFlashVersion();
}

FlashSprite::FlashSprite(void) :
	//m_pRenderTarget(NULL),
	m_pFlashData(NULL),
	//m_pIDirectDrawSurface7(NULL),
	m_pFlashPlayer(NULL),
	m_nWidth(0),
	m_nHeight(0)
{

}

FlashSprite::~FlashSprite(void)
{
	if (m_pFlashPlayer != NULL)
		delete m_pFlashPlayer;

	/*if (m_pRenderTarget != NULL)
		m_pRenderTarget->Release();*/
}

BOOL FlashSprite::StartFlash(/*IDirectDraw7* pIDirectDraw,*/ LPCSTR lpszFileName, UINT nWidth, UINT nHeight)
{
	m_nWidth  = nWidth;
	m_nHeight = nHeight;
	if (m_pFlashPlayer == NULL)
	{
		m_pFlashPlayer = new FlashPlayer;
		if (m_pFlashPlayer == NULL)
			return FALSE;
	}
	char sAppPath[MAX_PATH];
	if (strlen(lpszFileName) >=2 && lpszFileName[1] == ':')
	{
		strcpy(sAppPath, lpszFileName);
	}
	else
	{
		GetModuleFileName(GetModuleHandle(NULL), sAppPath, sizeof(sAppPath));
		char *lpTmp = strrchr(sAppPath,'\\');
		if (lpTmp != NULL )
		{
			*(lpTmp + 1) = 0;
		}
		strcat(sAppPath, lpszFileName);
	}

	m_pFlashPlayer->StartAnimation(sAppPath, nWidth, nHeight, NULL);
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

BOOL FlashSprite::IsPlaying()
{
	if (m_pFlashPlayer)
		return m_pFlashPlayer->IsPlaying() == true;
	return FALSE;
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
	char chOutputString[256];
	m_pFlashPlayer->Update();
	if (m_pFlashPlayer->Render())
	{
		//INT TexturePitch;
		//UINT nWidth  = m_nWidth;
		//UINT nHeight = m_nHeight;

		m_pFlashData = (void *)m_pFlashPlayer->GetFlashFrameBuffer();
		sprintf(chOutputString,"successe FlashSprite::Update(),m_pFlashData = %p", m_pFlashData);
		OutputDebugString(chOutputString);
		//memcpy(chOutputString, m_pFlashData,20);
		//strcat(chOutputString,"aa");
		//OutputDebugString(chOutputString);
		//UCHAR* pdwFlashData = (UCHAR*)m_pFlashPlayer->GetFlashFrameBuffer();
		//UCHAR* pdwRenderTarget;

		//DDSURFACEDESC2 ddsd;  //  direct draw surface description 
		//ddsd.dwSize = sizeof(ddsd);
		//m_pIDirectDrawSurface7->Lock(NULL,
		//	&ddsd,
		//	DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		//	NULL);

		//// assign a pointer to the memory surface for manipulation
		//pdwRenderTarget = (UCHAR *)ddsd.lpSurface;
		//for (int index_y=0; index_y<m_nHeight; index_y++)
		//{
		//	// copy next line of data to destination
		//	memcpy(pdwRenderTarget, pdwFlashData,m_nWidth);

		//	// advance pointers
		//	pdwFlashData   += (ddsd.lPitch); // (bob->width+bob->width_fill);
		//	pdwRenderTarget +=m_nWidth;
		//} 

		//m_pIDirectDrawSurface7->Unlock(NULL);

		// clear out the structure and set the size field 

		//D3DLOCKED_RECT LockedRect;
		//if (FAILED(m_pRenderTarget->LockRect(0, &LockedRect, 0, 0)))
		//	return FALSE;

		//pdwRenderTarget = (DWORD*)LockedRect.pBits;
		//TexturePitch = LockedRect.Pitch >> 2;

		//for (int y = 0; y < nHeight; y++) {
		////	memcpy(&pdwRenderTarget[y*tex_w], &pdwFlashData[y * nWidth], sizeof(DWORD)*nWidth);
		//	CopyMemory(pdwRenderTarget, pdwFlashData, nWidth << 2);
		//	pdwRenderTarget += TexturePitch;
		//	pdwFlashData += nWidth;
		//}

		//if (FAILED(m_pRenderTarget->UnlockRect(0)))
		//	return FALSE;

		return TRUE;
	}
	OutputDebugString("Failed FlashSprite::Update()");
	return FALSE;
}

//TEXTURE* FlashSprite::GetRenderTarget()
//{
//	return m_pRenderTarget;
//}