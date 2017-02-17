// cSurface.h: interface for the cSurface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSURFACE_H__0C439CBB_20C0_487F_84A6_31D8F436B74A__INCLUDED_)
#define AFX_CSURFACE_H__0C439CBB_20C0_487F_84A6_31D8F436B74A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddraw.h>



struct SURFACE_SOURCE_INFO
{
	HINSTANCE	m_hInstance;
	UINT		m_nResource;
	int			m_iX;
	int			m_iY;
	int			m_iWidth;
	int			m_iHeight;

};

class cSurface  
{

public:

	SURFACE_SOURCE_INFO m_srcInfo;
	void Restore();
	LPDIRECTDRAWSURFACE7 GetSurface();
	UINT Width();
	UINT Height();
	void Destroy();
	BOOL Draw(LPDIRECTDRAWSURFACE7 lpDest, int iDestX = 0, int iDestY = 0, int iSrcX = 0, int iSrcY = 0, int nWidth = 0, int nHeight = 0);
	BOOL Create(LPDIRECTDRAW7 hDD, int nWidth, int nHeight, COLORREF dwColorKey = -1);
	BOOL LoadBitmap(HINSTANCE hInst, UINT nRes, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	cSurface(LPDIRECTDRAW7 hDD, HINSTANCE hInst, UINT nResource, int nWidth, int nHeight, COLORREF dwColorKey = -1);
	cSurface();
	virtual ~cSurface();

protected:
	COLORREF m_ColorKey;
	UINT m_Height;
	UINT m_Width;
	LPDIRECTDRAWSURFACE7 m_pSurface;
};

#endif // !defined(AFX_CSURFACE_H__0C439CBB_20C0_487F_84A6_31D8F436B74A__INCLUDED_)
