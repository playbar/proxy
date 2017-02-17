// Dib.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windowsx.h>		// especially for GlobalAllocPtr

#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CDib, CObject, 0)

////////////////////////////////////////////////////////////////////////////
CDib::CDib()
{
	m_hDib		= NULL;
	m_hBitmap   = NULL;
	m_pPalette  = NULL;
	m_pBitmap   = NULL;
}           
                                      
CDib::~CDib()
{
	Destroy();
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	if (m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
}

void CDib::Destroy()
{
	if (m_hDib != NULL)
	{
		DestroyDIB(m_hDib);
		m_hDib = NULL;
	}
}

BOOL CDib::Create(DWORD dwWidth, DWORD dwHeight)
{
	HDIB hDib = CreateDefaultDIB(dwWidth, dwHeight);
	if (! hDib)
		return FALSE;

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
	HDIB hDib = CreateDIB(dwWidth, dwHeight, wBitCount);
	if (! hDib)
		return FALSE;

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(LPBYTE lpDIB)
{
	if (lpDIB == NULL)
		return FALSE;
	
	DWORD dwSize = DIBlockSize(lpDIB);

    HDIB hDib  = GlobalAlloc(GHND, dwSize); 
    // Check that DIB handle is valid 
    if (! hDib) 
        return FALSE; 
 
    LPBYTE lpbi  = (LPBYTE)GlobalLock(hDib); 
	if (! lpbi)
        return FALSE; 
		
	CopyMemory(lpbi, lpDIB, dwSize);
	GlobalUnlock(hDib);

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(LPBYTE lpDIB,
				  WORD  wBitCount)		// bits/pixel 
{
	if (lpDIB == NULL)
		return FALSE;
	if (! Create(lpDIB))
		return FALSE;

	WORD wBits = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	if (wBitCount == wBits)
		return TRUE;

	HDIB hNewDib = ConvertDIBFormat(m_hDib, wBitCount, NULL); 
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;
	return UpdateInternal();
}

BOOL CDib::Create(HBITMAP hBitmap)		// DIB Section
{
	if (! hBitmap)
        return FALSE; 

	HDIB hDib = DIBSectionToDIB(hBitmap); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(HBITMAP hBitmap,		// DIB Section
				  WORD  wBitCount)		// bits/pixel 
{
	HDIB hNewDib;

	if (! hBitmap)
        return FALSE; 

	HDIB hDib = DIBSectionToDIB(hBitmap); 
    if (! hDib) 
        return FALSE; 

	DIBSECTION ds;
	GetObject(hBitmap, sizeof(DIBSECTION), &ds);

	if (wBitCount == ds.dsBmih.biBitCount)
		hNewDib = hDib;
	else
	{
		hNewDib = ConvertDIBFormat(hDib, wBitCount, NULL); 
		// cleanup hDib
		GlobalFree(hDib);
	}
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;
	return UpdateInternal();
}

BOOL CDib::Create(HBITMAP hBitmap,		// DDB bitmap
			      HPALETTE hPalette)	// DDB palette
{
	if (! hBitmap)
        return FALSE; 

	HDIB hDib = BitmapToDIB(hBitmap, hPalette); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(HBITMAP hBitmap,		// DDB bitmap
			      HPALETTE hPalette,	// DDB palette
				  WORD  wBitCount)		// bits/pixel 
{
	if (! hBitmap)
        return FALSE; 

	HDIB hDib = BitmapToDIB(hBitmap, hPalette, wBitCount); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(CRect rcScreen)
{
	HDIB hDib = CopyScreenToDIB(rcScreen); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(HWND hWnd, WORD fPrintArea)
{
	HDIB hDib = CopyWindowToDIB(hWnd, fPrintArea); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

BOOL CDib::Create(HWND hWnd, CRect rcClientArea)
{
	HDIB hDib = CopyClientRectToDIB(hWnd, rcClientArea); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

void CDib::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	ar.Flush();
	if (ar.IsStoring())
	{
		Write(ar.GetFile());
	}
    else
    {
   		Read(ar.GetFile());
    }
}

BOOL CDib::Load(UINT uIDS, LPCTSTR lpszDibType)
{                                
	LPCTSTR lpszDibRes = MAKEINTRESOURCE(uIDS);

	return Load(lpszDibRes, lpszDibType);
}

BOOL CDib::Load(LPCTSTR lpszDibRes, LPCTSTR lpszDibType)
{                                
	HINSTANCE hInst = AfxGetInstanceHandle();
	HRSRC   hRes    = ::FindResource(hInst, lpszDibRes, lpszDibType);
	HGLOBAL hData   = ::LoadResource(hInst, hRes);

	// if resource ok?
	if (hRes == NULL || hData == NULL)
		return FALSE;

	// get resource buffer
	LPBYTE lpBuf = (LPBYTE)::LockResource(hData);
	// is DIB ?
	if (((LPBITMAPFILEHEADER)lpBuf)->bfType != DIB_HEADER_MARKER/*"BM"*/)
		return FALSE;

	// use this buffer to create CDib
	LPBYTE lpDIB = lpBuf + sizeof(BITMAPFILEHEADER);
	return Create(lpDIB);
}

BOOL CDib::Load(LPCTSTR lpszDibFile)
{                                
	TRY
	{
		CFile file(lpszDibFile, CFile::modeRead|CFile::shareDenyNone);

		if (! Read(&file))
			return FALSE;
	}
	CATCH (CException, e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}               

BOOL CDib::Save(LPCSTR lpszDibFile)
{
	TRY
	{
		CFile file(lpszDibFile, CFile::modeCreate|CFile::modeWrite);

		if (! Write(&file))
			return FALSE;
	}
	CATCH (CException, e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CDib::Read(CFile *pFile)
{
	WaitCursorBegin();

	LPBITMAPINFOHEADER lpbi;
	DWORD dwSize;
	TRY
	{
		// read DIB file header
		BITMAPFILEHEADER bmfHdr;
		pFile->Read(&bmfHdr, sizeof(BITMAPFILEHEADER));
		// is DIB file?
		if (bmfHdr.bfType != DIB_HEADER_MARKER/*"BM"*/)
		{
			WaitCursorEnd();
			return FALSE;
		}
		DWORD dwLength = pFile->GetLength();
		if (bmfHdr.bfSize != dwLength)
			bmfHdr.bfSize = dwLength;

		// read DIB buffer
		dwSize = bmfHdr.bfSize - sizeof(BITMAPFILEHEADER);
		lpbi = (LPBITMAPINFOHEADER)GlobalAllocPtr(GHND, dwSize);
		DWORD dwCount = pFile->Read(lpbi, dwSize);
		// read ok?
		if (dwCount != dwSize)
		{
			GlobalFreePtr(lpbi);
			WaitCursorEnd();
			return FALSE;
		}

		// Check to see that it's a Windows DIB -- an OS/2 DIB would cause 
		// strange problems with the rest of the DIB API since the fields 
		// in the header are different and the color table entries are 
		// smaller. 
		// 
		// If it's not a Windows DIB (e.g. if biSize is wrong), return NULL. 
	    if (lpbi->biSize != sizeof(BITMAPINFOHEADER)) 
		{
			GlobalFreePtr(lpbi);
			WaitCursorEnd();
			return FALSE;
		}
		
		// fill color num item
		int nNumColors = (UINT)lpbi->biClrUsed;
		if (nNumColors == 0) 
		{ 
			// no color table for 24-bit, default size otherwise 
	        if (lpbi->biBitCount != 24) 
		        nNumColors = 1 << lpbi->biBitCount; // standard size table 
		} 
 
		// fill in some default values if they are zero 
	    if (lpbi->biClrUsed == 0) 
		    lpbi->biClrUsed = nNumColors; 
		if (lpbi->biSizeImage == 0) 
			lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3) * lpbi->biHeight; 
 	}
	CATCH (CException, e)
	{
		GlobalFreePtr(lpbi);
		WaitCursorEnd();
		return FALSE;
	}
	END_CATCH

	// create CDib with DIB buffer
	BOOL bSuccess = Create((LPBYTE)lpbi);
	GlobalFreePtr(lpbi);
	WaitCursorEnd();

	return bSuccess;
}

BOOL CDib::Write(CFile *pFile)
{
	WaitCursorBegin();

    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file 
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
    DWORD               dwDIBSize; 

	// Get a pointer to the DIB memory, the first of which contains 
    // a BITMAPINFO structure 
    lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
		WaitCursorEnd();
        return FALSE; 
	}
 
    // Check to see if we're dealing with an OS/2 DIB.  If so, don't 
    // save it because our functions aren't written to deal with these 
    // DIBs. 
    if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
    { 
        GlobalUnlock(m_hDib); 
		WaitCursorEnd();
        return FALSE; 
    } 
 
    // Fill in the fields of the file header 
 
    // Fill in file type (first 2 bytes must be "BM" for a bitmap) 
 
    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM" 
 
    // Calculating the size of the DIB is a bit tricky (if we want to 
    // do it right).  The easiest way to do this is to call GlobalSize() 
    // on our global handle, but since the size of our global memory may have 
    // been padded a few bytes, we may end up writing out a few too 
    // many bytes to the file (which may cause problems with some apps, 
    // like HC 3.0). 
    // 
    // So, instead let's calculate the size manually. 
    // 
    // To do this, find size of header plus size of color table.  Since the 
    // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains 
    // the size of the structure, let's use this. 
 
    // Partial Calculation 
 
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
 
    // Now calculate the size of the image 
 
    // It's an RLE bitmap, we can't calculate size, so trust the biSizeImage 
    // field 
 
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only 
 
        // It's not RLE, so size is Width (DWORD aligned) * Height 
 
        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
 
        dwDIBSize += dwBmBitsSize; 
 
        // Now, since we have calculated the correct size, why don't we 
        // fill in the biSizeImage field (this will fix any .BMP files which  
        // have this field incorrect). 
 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
 
 
    // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER) 
                    
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
 
    // Now, calculate the offset the actual bitmap bits will be in 
    // the file -- It's the Bitmap file header plus the DIB header, 
    // plus the size of the color table. 
     
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
 	TRY
	{
	    // Write the file header 
		pFile->Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
		// write DIB buffer
		pFile->Write(lpBI, dwDIBSize);
	}
	CATCH (CException, e)
	{
        GlobalUnlock(m_hDib); 
		WaitCursorEnd();
		return FALSE;
	}
	END_CATCH

	GlobalUnlock(m_hDib); 
	WaitCursorEnd();
	
	return TRUE;
}

BOOL CDib::Display(CDC* pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
 				   int xSrc, int ySrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->BitBlt( xDest, yDest, 
							nWidthDest, nHeightDest,
						    &MemDC, 
							xSrc, ySrc, 
							dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::Display(CDC * pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
				   int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->StretchBlt( xDest, yDest, 
								nWidthDest, nHeightDest,
						        &MemDC, 
								xSrc, ySrc, 
								nWidthSrc, nHeightSrc, 
								dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::Display(CDC * pDC, int x, int y, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->BitBlt(x, y, 
								GetWidth(), GetHeight(),
								&MemDC, 
								0, 0, 
								dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::Display(CDC* pDC, CRect rcDest, CRect rcSrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->StretchBlt( rcDest.left, rcDest.top, 
								rcDest.Width(), rcDest.Height(),
						        &MemDC, 
								rcSrc.left, rcSrc.top, 
								rcSrc.Width(), rcSrc.Height(),
								dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

BOOL CDib::BuildBitmap()
{
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
		m_hBitmap = NULL;
	}
	m_hBitmap = DIBToDIBSection(m_hDib);
	if (m_hBitmap == NULL)
		return FALSE;
	m_pBitmap = new CBitmap;
	m_pBitmap->Attach(m_hBitmap);

	return TRUE;
}

BOOL CDib::BuildPalette()
{
	if (m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
	HPALETTE hPalette = CreateDIBPalette(m_hDib);
	if (hPalette == NULL)
		return FALSE;
	m_pPalette = new CPalette;
	m_pPalette->Attach(hPalette);

	return TRUE;
}

BOOL CDib::UpdateInternal()
{
	BuildPalette();
	return BuildBitmap();
}

CPalette* CDib::GetPalette()
{
	return m_pPalette;
}

CBitmap* CDib::GetBitmap()
{
	return m_pBitmap;
}

BOOL CDib::IsEmpty()
{
	if (m_hDib == NULL)
		return TRUE;

	if (! GlobalLock(m_hDib))
		return TRUE;

	GlobalUnlock(m_hDib);
	return FALSE;
}

DWORD CDib::GetCompression()
{
    LPBITMAPINFOHEADER lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
        return 0; 
	}
 
	DWORD dwCompression = lpBI->biCompression;
	GlobalUnlock(m_hDib);

	return dwCompression;
}

WORD CDib::GetBitCount()
{
    LPBITMAPINFOHEADER lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
        return 0; 
	}
 
	WORD wBitCount = lpBI->biBitCount;
	GlobalUnlock(m_hDib);

	return wBitCount;
}

LONG CDib::GetWidth()
{
	// get DIB buffer pointer
    LPBYTE lpDIB = (LPBYTE)GlobalLock(m_hDib); 
	if (! lpDIB)
	{
		GlobalUnlock(m_hDib);
		return 0;
	}

	LONG lWidth = (LONG)DIBWidth(lpDIB);
	GlobalUnlock(m_hDib);

	return lWidth; 
}

LONG CDib::GetHeight()
{
	// get DIB buffer pointer
    LPBYTE lpDIB = (LPBYTE)GlobalLock(m_hDib); 
	if (! lpDIB)
	{
		GlobalUnlock(m_hDib);
		return 0;
	}

	LONG lHeight = (LONG)DIBHeight(lpDIB);
	GlobalUnlock(m_hDib);

	return lHeight; 
}

LONG CDib::GetWidthBytes()
{
	return WIDTHBYTES((GetWidth())*((DWORD)GetBitCount()));
}

COLORREF CDib::GetPixel(LONG x, LONG y)
{
	COLORREF cColor;
	switch (GetBitCount())
	{
		case 1 :	if (1<<(7-x%8) & 
						*(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)))
						cColor = RGB(255,255,255);
					else
						cColor = RGB(0,0,0);
					break;
		case 4 :	
				{
					PALETTEENTRY PaletteColors[16];
					m_pPalette->GetPaletteEntries(0, 16, PaletteColors);
					int nIndex = (*(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)) & 
								   (x%2 ? 0x0f : 0xf0)) >> (x%2 ? 0 : 4);
					cColor = RGB(PaletteColors[nIndex].peRed,
								 PaletteColors[nIndex].peGreen,
								 PaletteColors[nIndex].peBlue);
				}
					break;
		case 8 :	
				{
					PALETTEENTRY PaletteColors[256];
					m_pPalette->GetPaletteEntries(0, 256, PaletteColors);
					int nIndex = *(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y));
					cColor = RGB(PaletteColors[nIndex].peRed,
								 PaletteColors[nIndex].peGreen,
								 PaletteColors[nIndex].peBlue);
				}
					break;
		default:	cColor = RGB(*(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)),
								 *(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)+1),
								 *(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)+2));
					break;
	}
	return cColor;
}

LONG CDib::GetPixelOffset(LONG x, LONG y)
{
	return (GetHeight()-y-1)*GetWidthBytes()+(x*GetBitCount())/8;
}

LPBYTE CDib::GetBitsPtr()
{
    LPBYTE lpDIB = (LPBYTE)GlobalLock(m_hDib); 
    if (! lpDIB) 
	{
		GlobalUnlock(m_hDib);
		return NULL;
	}

	LPBYTE lpData = FindDIBBits(lpDIB);
	GlobalUnlock(m_hDib);

	return lpData;
}

HANDLE CDib::GetHandle()
{
	return m_hDib;
}

WORD CDib::GetColorNumber()
{
    LPBYTE lpBI = (LPBYTE)GlobalLock(m_hDib); 
    if (! lpBI) 
	{
		GlobalUnlock(m_hDib);
		return 0;
	}
 
	WORD wColors = DIBNumColors(lpBI);
	GlobalUnlock(m_hDib);

	return wColors;
}

WORD CDib::GetPaletteSize()
{
    LPBYTE lpBI = (LPBYTE)GlobalLock(m_hDib); 
    if (! lpBI) 
	{
		GlobalUnlock(m_hDib);
		return 0;
	}
 
	WORD wPalSize = PaletteSize(lpBI);
	GlobalUnlock(m_hDib);

	return wPalSize;
}

CDC* CDib::BeginPaint(CDC *pDC)
{
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pPaletteTmp = m_pMemDC->SelectPalette(m_pPalette, TRUE);
	m_pMemDC->RealizePalette();
	m_pBitmapTmp = (CBitmap *)m_pMemDC->SelectObject(m_pBitmap);
	return m_pMemDC;
}

void CDib::EndPaint()
{
	m_pMemDC->SelectObject(m_pBitmapTmp);
	m_pMemDC->SelectPalette(m_pPaletteTmp, TRUE);
	delete m_pMemDC;

	Create(m_hBitmap);
}

BOOL CDib::DisplayPalette(CDC* pDC, CRect rc)
{
	return ::DisplayPalette(pDC->GetSafeHdc(), &rc, (HPALETTE)m_pPalette->GetSafeHandle());
}

CDib * CDib::Clone()
{
	if (m_hDib == NULL)
		return NULL;

	HDIB hDIB = CopyHandle(m_hDib);
	if (hDIB == NULL)
		return NULL;

	CDib *pDib = new CDib;
	pDib->m_hDib = hDIB;
	pDib->BuildPalette();
	pDib->BuildBitmap();

	return pDib;
}

