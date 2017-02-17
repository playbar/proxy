#include "Surface.h"

Surface::Surface()
{
	m_pkSurface = NULL;
	m_ulColorKey = -1;
}

Surface::Surface(LPDIRECTDRAW7 pkDD, HINSTANCE hInst, const char *pcFileName, int iWidth, int iHeight, unsigned long ulColorKey)
{
	m_pkSurface = NULL;
	m_ulColorKey = -1;
	
	Create(pkDD, iWidth, iHeight, ulColorKey);
	LoadBitmap(hInst, pcFileName);
}

Surface::~Surface()
{
	if(m_pkSurface)
	{
		m_pkSurface->Release();
		m_pkSurface = NULL;
	}
}

void Surface::Destroy()
{
	if(m_pkSurface != NULL)
	{
		m_pkSurface->Release();
		m_pkSurface = NULL;
	}
}

bool Surface::Create(LPDIRECTDRAW7 pkDD, int iWidth, int iHeight, unsigned long ulColorKey)
{
    DDSURFACEDESC2 kDDSurfDesc;
    DDCOLORKEY kDDCK;

    ZeroMemory(&kDDSurfDesc, sizeof(kDDSurfDesc));
    kDDSurfDesc.dwSize = sizeof(kDDSurfDesc);
	kDDSurfDesc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    kDDSurfDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	kDDSurfDesc.dwWidth = iWidth;
    kDDSurfDesc.dwHeight = iHeight;

    HRESULT hRet = pkDD->CreateSurface(&kDDSurfDesc, &m_pkSurface, NULL);
    if(hRet != DD_OK)
	{
		if(hRet == DDERR_OUTOFVIDEOMEMORY)
		{
			kDDSurfDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			hRet = pkDD->CreateSurface(&kDDSurfDesc, &m_pkSurface, NULL );
		}

		if(hRet != DD_OK)
			return false;
	}

	if((int)ulColorKey != -1)
	{
		kDDCK.dwColorSpaceLowValue = ulColorKey;
		kDDCK.dwColorSpaceHighValue = 0;
		m_pkSurface->SetColorKey(DDCKEY_SRCBLT, &kDDCK);
	}

	m_ulColorKey = ulColorKey;
	m_uiWidth  = iWidth;
	m_uiHeight = iHeight;

	return true;
}

bool Surface::LoadBitmap(HINSTANCE hInst, const char *pcFileName, int iX, int iY, int iWidth, int iHeight)
{
	HBITMAP	kHBitmap = (HBITMAP)LoadImage(hInst, pcFileName, IMAGE_BITMAP, iWidth, iHeight, LR_LOADFROMFILE);
	if(kHBitmap == NULL || m_pkSurface == NULL)
		return false;

	HDC hImage = CreateCompatibleDC(NULL);
	if (!hImage)
		return false;

	BITMAP kBitmap;
	SelectObject(hImage, kHBitmap);
	GetObject(kHBitmap, sizeof(kBitmap), &kBitmap);

	if(iWidth == 0)
		iWidth = kBitmap.bmWidth;

	if(iHeight == 0)
		iHeight = kBitmap.bmHeight;

	DDSURFACEDESC2 kDDSurfDesc;
	kDDSurfDesc.dwSize = sizeof(kDDSurfDesc);
	kDDSurfDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_pkSurface->Restore();
	m_pkSurface->GetSurfaceDesc(&kDDSurfDesc);

	HDC hDC;
	if(m_pkSurface->GetDC(&hDC) == DD_OK)
	{
		StretchBlt(hDC, 0, 0, kDDSurfDesc.dwWidth, kDDSurfDesc.dwHeight, hImage, iX, iY, iWidth, iHeight, SRCCOPY);
		m_pkSurface->ReleaseDC(hDC);
	}
	DeleteDC(hImage);

	return true;
}

bool Surface::Draw(LPDIRECTDRAWSURFACE7 pkDest, int iDestX, int iDestY, int iSrcX, int iSrcY, int iWidth, int iHeight)
{
	RECT kRect;
	HRESULT	hRet;

	if(iWidth == 0)
		iWidth = m_uiWidth;

	if(iHeight == 0)
		iHeight = m_uiHeight;

	kRect.left = iSrcX;
	kRect.top = iSrcY;
	kRect.right = iWidth  + iSrcX;
	kRect.bottom = iHeight + iSrcY;

	while(1)
	{
		if((int)m_ulColorKey < 0)
			hRet = pkDest->BltFast(iDestX, iDestY, m_pkSurface, &kRect,  DDBLTFAST_NOCOLORKEY);
		else
			hRet = pkDest->BltFast(iDestX, iDestY, m_pkSurface, &kRect,  DDBLTFAST_SRCCOLORKEY);

		if(hRet == DD_OK)
			break;

		if(hRet == DDERR_SURFACELOST)
			Restore();
		else
		{
			if(hRet != DDERR_WASSTILLDRAWING)
				return false;
		}
	}

	return true;
}
