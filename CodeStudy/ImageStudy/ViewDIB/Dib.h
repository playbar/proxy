#if !defined(AFX_DIB_H__33817561_072D_11D1_8C94_000021003EA5__INCLUDED_)
#define AFX_DIB_H__33817561_072D_11D1_8C94_000021003EA5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dib.h : header file
//

#include "DIBAPI.H"


////////////////////////////////////////////////////////////////////////////
// CDib
class CDib : public CObject
{                          
	DECLARE_SERIAL(CDib)

// Public member function
public: 
	// 构造函数
	CDib();
	// 拷贝构造函数
	CDib(CDib& other) 
	{ 
		// 1) 初始化 
		m_hDib		= NULL;
		m_hBitmap   = NULL;
		m_pPalette  = NULL;
		m_pBitmap   = NULL;

		// 2) 检查源对象是否有效 
		HDIB hDib = other.GetHandle();
		if (! hDib)
			return;
		LPBYTE lpDIB = (LPBYTE)GlobalLock(hDib);
		if (lpDIB == NULL)
		{
			GlobalUnlock(hDib);
			return; 
		}

		// 3) 创建对象 
		Create(lpDIB);
		GlobalUnlock(hDib);
	} 
	
	// 赋值函数 
	CDib& operator =(CDib& other) 
	{ 
		// 1) 检查自赋值 
		if(this == &other) 
			return *this; 

		// 2) 检查源对象是否有效 
		HDIB hDib = other.GetHandle();
		if (! hDib)
			return *this; 
		LPBYTE lpDIB = (LPBYTE)GlobalLock(hDib);
		if (lpDIB == NULL)
		{
			GlobalUnlock(hDib);
			return *this; 
		}

		// 3) 释放原有的内存资源 
		Destroy();
		m_hBitmap   = NULL;
		m_pPalette  = NULL;
		m_pBitmap   = NULL;

		// 4) 分配新的内存资源，并复制内容 
		Create(lpDIB);
		GlobalUnlock(hDib);

		// 4) 返回本对象的引用 
		return *this; 
	} 

	// create
	BOOL	Create(DWORD dwWidth, DWORD dwHeight);
	BOOL	Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount);
	BOOL	Create(LPBYTE lpDIB);
	BOOL	Create(LPBYTE lpDIB,		// DIB pointer
				   WORD  wBitCount);	// bits/pixel 
	BOOL	Create(HBITMAP hBitmap);	// DIBSection
	BOOL	Create(HBITMAP hBitmap,		// DIBSection
				   WORD  wBitCount);	// bits/pixel 
	BOOL	Create(HBITMAP hBitmap,		// Bitmap handle
				   HPALETTE hPalette);	// Palette handle
	BOOL	Create(HBITMAP hBitmap,		// Bitmap handle
				   HPALETTE hPalette,	// Palette handle
				   WORD  wBitCount);	// bits/pixel 
	BOOL	Create(CRect rcScreen);
	BOOL	Create(HWND hWnd, WORD fPrintArea);
	BOOL	Create(HWND hWnd, CRect rcClientArea);
	// load/save
    BOOL	Load(UINT uIDS, LPCTSTR lpszDibType);
    BOOL	Load(LPCTSTR lpszDibRes, LPCTSTR lpszDibType);
    BOOL	Load(LPCTSTR lpszDibFile);
    BOOL	Save(LPCTSTR lpszDibFile);
	BOOL	Read(CFile *pFile);
	BOOL	Write(CFile *pFile);

	// clone
	CDib *  Clone();

	// deconstructor
	virtual ~CDib();
	// destroy
	void	Destroy();

	// overlaying Serialize
	virtual void Serialize(CArchive &ar);
    
	// display
	BOOL	Display(CDC * pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
 				   int xSrc, int ySrc, DWORD dwRop=SRCCOPY);
	BOOL	Display(CDC * pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
				    int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop=SRCCOPY);
    BOOL	Display(CDC* pDC, int x, int y, DWORD dwRop=SRCCOPY);
	BOOL	Display(CDC* pDC, CRect rcDest, CRect rcSrc,DWORD dwRop=SRCCOPY);
	BOOL	DisplayPalette(CDC* pDC, CRect rc);

	// DC for modify DIB
	CDC*	BeginPaint(CDC *pDC);
	void	EndPaint();

	// DDB and palette
	BOOL	BuildBitmap();
	BOOL	BuildPalette();

	// attributes
	BOOL	IsEmpty();
	DWORD	GetCompression();
    WORD	GetBitCount();
    LONG	GetWidth();
    LONG	GetHeight();    
	LONG	GetWidthBytes();
    WORD	GetColorNumber();
	WORD	GetPaletteSize();
    CBitmap*  GetBitmap();
	CPalette* GetPalette();
	HANDLE	GetHandle();
	LPBYTE	GetBitsPtr();
	COLORREF GetPixel(LONG x, LONG y);
	LONG    GetPixelOffset(LONG x, LONG y);

// private member function
private:
	BOOL	UpdateInternal();

// public member data
public:
	HDIB		m_hDib;
	HBITMAP		m_hBitmap;	// handle of DIBSection
	CPalette* 	m_pPalette;
	CBitmap*	m_pBitmap;

// private member data
private:
	// for drawing in DIB
	CDC *		m_pMemDC;
	CBitmap*	m_pBitmapTmp;
	CPalette*	m_pPaletteTmp;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIB_H__33817561_072D_11D1_8C94_000021003EA5__INCLUDED_)
