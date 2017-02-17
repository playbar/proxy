// Pcx.h: interface for the CPcx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCX_H__8A4C8A60_0726_11D3_9481_000021003EA5__INCLUDED_)
#define AFX_PCX_H__8A4C8A60_0726_11D3_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

typedef struct tagPCXHEADER
{
  char manufacturer;
  char version;
  char encoding;
  char bitsPerPixel;
  WORD xmin, ymin;
  WORD xmax, ymax;
  WORD hres, vres;
  char palette[48];
  char reserved;
  char colourPlanes;
  WORD bytesPerLine;
  WORD paletteType;
  char filler[58];
} PCXHEADER;

typedef struct tagPCXPALETTE
{
  BYTE rgbRed;
  BYTE rgbGreen;
  BYTE rgbBlue;
} PCXPALETTE;


class AFX_EXT_CLASS CPcx  
{
public:
	CPcx();
	CPcx(CDib *pDib);
	virtual ~CPcx();

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
	// Load PCX file
	BOOL Load(LPCTSTR lpstrFileName);
	// Save PCX file
	BOOL Save(LPCTSTR lpstrFileName, CDib* pDib = NULL);

protected:
	BOOL ReadPCXLine(int bytes, BYTE *p, FILE *fp);
	BOOL WritePCXLine(int bytes, BYTE *p, FILE *fp);
	BOOL VertFlipBuf(BYTE* inbuf, UINT widthBytes, UINT height);

private:
	CDib*	m_pDib;
};

#endif // !defined(AFX_PCX_H__8A4C8A60_0726_11D3_9481_000021003EA5__INCLUDED_)
