// Tga.h: interface for the CTga class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGA_H__FA4FE7A1_D785_48BD_A54E_69825D7EDE9D__INCLUDED_)
#define AFX_TGA_H__FA4FE7A1_D785_48BD_A54E_69825D7EDE9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

struct TGA_FILE_HEADER
{
  BYTE  byIdLength;
  BYTE  byColorMapType;
  BYTE  byImageType;
  short nColorMapFirstIndex;
  short nColorMapLength;
  BYTE  byColorMapEntrySize;
  short nXOrigin;
  short nYOrigin;
  short nImageWidth;
  short nImageHeight;
  BYTE  byPixelDepth;
  BYTE  byImageDescriptor;
};

struct TGA_FILE_FOOTER
{
  long lExtensionAreaOffset;
  long lDeveloperDirectoryOffset;
  char cSignature[18];
};

class AFX_EXT_CLASS CTga  
{
public:
	CTga();
	CTga(CDib *pDib);
	virtual ~CTga();

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
	// Load TGA file
	BOOL Load(LPCTSTR lpstrFileName);
	// Save TGA file
	BOOL Save(LPCTSTR lpstrFileName, CDib* pDib);

protected:
	HDIB ReadTGAFile(CFile& file);
	BOOL SaveTGAFile(HDIB hDib, CFile& file);

private:
	CDib*	m_pDib;
};

#endif // !defined(AFX_TGA_H__FA4FE7A1_D785_48BD_A54E_69825D7EDE9D__INCLUDED_)
