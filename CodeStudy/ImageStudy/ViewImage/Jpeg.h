// Jpeg.h: interface for the CJpeg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JPEG_H__63087660_0165_11D3_9481_000021003EA5__INCLUDED_)
#define AFX_JPEG_H__63087660_0165_11D3_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CJpeg  
{
public:
	CJpeg();
	CJpeg(CDib *pDib);
	virtual ~CJpeg();

// function
public:
	// Get CDib
	CDib * GetDib()
	{
		return m_pDib;
	}
	//Set CDib
	BOOL SetDib(CDib *pDib)
	{
		if (pDib == NULL)
			return FALSE;

		if (m_pDib != NULL)
			delete m_pDib;

		m_pDib = pDib->Clone();

		return (m_pDib != NULL);
	}
	// Load JPEG file
	BOOL Load(LPCSTR lpstrFileName);
	// save JPEG file
	BOOL Save(LPCSTR lpstrFileName, 
			  CDib* pDib = NULL,
			  BOOL bColor = TRUE,
			  int nQuality = 75);
	// get error string
	CString GetErrorString();

private:
	////////////////////////////////////////////////////////////////
	// read a JPEG file to an RGB buffer - 3 bytes per pixel
	// returns a ptr to a buffer .
	BYTE* ReadJPEGFile(LPCSTR lpstrFileName, UINT *uWidth, UINT *uHeight);
	////////////////////////////////////////////////////////////////
	// write a JPEG file from a 3-component, 1-byte per component buffer
	BOOL WriteJPEGFile(LPCTSTR lpstrFileName,	// path
					   BYTE *dataBuf,			// RGB buffer
					   UINT width,				// pixels
					   UINT height,				// rows
					   BOOL color,				// TRUE = RGB
												// FALSE = Grayscale
    				   int quality);			// 0 - 100

	////////////////////////////////////////////////////////////////
	// allocates a DWORD-aligned buffer, copies data buffer
	// caller is responsible for delete []'ing the buffer
	BYTE* MakeDwordAlign(BYTE *dataBuf,			// input buf
						 UINT widthPix,				// input pixels
						 UINT height,				// lines
						 UINT *uiOutWidthBytes);	// new width bytes
	// free allocate memory
	void FreeBuffer(BYTE *Buffer);
	////////////////////////////////////////////////////////////////
	// if you have a DWORD aligned buffer, this will copy the
	// RGBs out of it into a new buffer. new width is widthPix * 3 bytes
	// caller is responsible for delete []'ing the buffer
	BYTE *ClearDwordAlign(BYTE *inBuf,				// input buf
						UINT widthPix,				// input size
						UINT widthBytes,			// input size
						UINT height);
	BOOL VertFlipBuf(BYTE * inbuf,						// input buf
				   UINT widthBytes,							// input width bytes
				   UINT height);							// height
	BOOL BGRFromRGB(BYTE *buf,							// input buf
					UINT widthPix,								// width in pixels
					UINT height);								// lines

	// data
	CString m_strJPEGError;

//data
private:
	CDib*	m_pDib;
};

#endif // !defined(AFX_JPEG_H__63087660_0165_11D3_9481_000021003EA5__INCLUDED_)
