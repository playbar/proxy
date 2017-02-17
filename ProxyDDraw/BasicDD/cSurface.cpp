// cSurface.cpp: implementation of the cSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cSurface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cSurface::cSurface()
{
	m_pSurface = NULL;
	m_ColorKey = -1;
}

cSurface::cSurface(LPDIRECTDRAW7 hDD, HINSTANCE hInst, UINT nResource, int nWidth, int nHeight, COLORREF dwColorKey)
{
	m_pSurface = NULL;
	m_ColorKey = -1;
	
	Create(hDD, nWidth, nHeight, dwColorKey);
	this->LoadBitmap(hInst, nResource);
	
}

cSurface::~cSurface()
{
	if(m_pSurface != NULL)
	{
		OutputDebugString("Surface Destroyed\n");
		m_pSurface->Release();
		m_pSurface = NULL;
	}
}

BOOL cSurface::LoadBitmap(HINSTANCE hInst, UINT nRes, int nX, int nY, int nWidth, int nHeight)
{
    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hr;

	HBITMAP	hbm;

	hbm = (HBITMAP) LoadImage(hInst, MAKEINTRESOURCE(nRes), IMAGE_BITMAP, nWidth, nHeight, 0L);

    if (hbm == NULL || m_pSurface == NULL)
        return FALSE;

    // Make sure this surface is restored.
    m_pSurface->Restore();

    // Select bitmap into a memoryDC so we can use it.
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        return FALSE;

    SelectObject(hdcImage, hbm);

    // Get size of the bitmap
    GetObject(hbm, sizeof(bm), &bm);

	if(nWidth == 0)
		nWidth = bm.bmWidth;
	
	if(nHeight == 0)
		nHeight = bm.bmHeight;
    
    // Get size of surface.
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    m_pSurface->GetSurfaceDesc(&ddsd);

    if ((hr = m_pSurface->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, nX, nY,
                   nWidth, nHeight, SRCCOPY);
        m_pSurface->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);

	m_srcInfo.m_hInstance = hInst;
	m_srcInfo.m_nResource = nRes;
	m_srcInfo.m_iX		  = nX;
	m_srcInfo.m_iY		  = nY;
	m_srcInfo.m_iWidth    = nWidth;
	m_srcInfo.m_iHeight	  = nHeight;
	
	return TRUE;
}

BOOL cSurface::Create(LPDIRECTDRAW7 hDD, int nWidth, int nHeight, COLORREF dwColorKey)
{
    DDSURFACEDESC2		ddsd;
	HRESULT				hRet;
    DDCOLORKEY          ddck;


    ZeroMemory( &ddsd, sizeof( ddsd ) );

    ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |
						  DDSCAPS_VIDEOMEMORY;
	
	ddsd.dwWidth  = nWidth;
    ddsd.dwHeight = nHeight;

    hRet = hDD->CreateSurface(&ddsd, &m_pSurface, NULL );
    if( hRet != DD_OK )
	{
		
		if(hRet == DDERR_OUTOFVIDEOMEMORY)
		{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |
					  DDSCAPS_SYSTEMMEMORY;
		
		hRet = hDD->CreateSurface(&ddsd, &m_pSurface, NULL );
		}

		if( hRet != DD_OK )
		{
			return FALSE;
		}
	}

	if((int)dwColorKey != -1)
	{
		ddck.dwColorSpaceLowValue = dwColorKey;
		ddck.dwColorSpaceHighValue = 0;
		m_pSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	m_ColorKey = dwColorKey;
	m_Width  = nWidth;
	m_Height = nHeight;

	return TRUE;
}

BOOL cSurface::Draw(LPDIRECTDRAWSURFACE7 lpDest, int iDestX, int iDestY, int iSrcX, int iSrcY, int nWidth, int nHeight)
{
	RECT	rcRect;
	HRESULT	hRet;

	if(nWidth == 0)
		nWidth = m_Width;

	if(nHeight == 0)
		nHeight = m_Height;

	rcRect.left   = iSrcX;
	rcRect.top    = iSrcY;
	rcRect.right  = nWidth  + iSrcX;
	rcRect.bottom = nHeight + iSrcY;

	while(1)
	{
		if((int)m_ColorKey < 0)
		{
			hRet = lpDest->BltFast(iDestX, iDestY, m_pSurface, &rcRect,  DDBLTFAST_NOCOLORKEY);
		}
		else
		{
			hRet = lpDest->BltFast(iDestX, iDestY, m_pSurface, &rcRect,  DDBLTFAST_SRCCOLORKEY);
		}

		if(hRet == DD_OK)
			break;

		if(hRet == DDERR_SURFACELOST)
		{
			Restore();
		}
		else
		{
			if(hRet != DDERR_WASSTILLDRAWING)
				return FALSE;
		}
	}

	return TRUE;
}

void cSurface::Destroy()
{

	if(m_pSurface != NULL)
	{
		m_pSurface->Release();
		m_pSurface = NULL;
	}
}

UINT cSurface::Height()
{
	return m_Height;
}

UINT cSurface::Width()
{
	return m_Width;
}

LPDIRECTDRAWSURFACE7 cSurface::GetSurface()
{
	return m_pSurface;
}

void cSurface::Restore()
{
	m_pSurface->Restore();
}

