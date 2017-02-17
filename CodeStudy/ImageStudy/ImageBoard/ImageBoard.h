// ImageBoard.h : main header file for the IMAGEBOARD application
//

#if !defined(AFX_IMAGEBOARD_H__CEBADA04_ED34_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_IMAGEBOARD_H__CEBADA04_ED34_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "dib.h"


static char BASED_CODE szOpenFilter[] = "图象文件|*.bmp; *.dib; *.gif; *.jpg; *.jpe; *.jpeg; *.pcx; *.tga; *.tif; *.tiff|位图文件 (*.bmp;*.dib)|*.bmp; *.dib|GIF文件 (*.gif)|*.gif|JPEG文件 (*.jpg;*.jpe;*.jpeg)|*.jpg; *.jpe; *.jpeg|PCX文件 (*.pcx)|*.pcx|TGA文件 (*.tga)|*.tga|TIFF文件 (*.tif;*.tiff)|*.tif; *.tiff|All Files (*.*)|*.*||";
static char BASED_CODE szSaveFilter[] = "单色位图 (*.bmp;*.dib)|*.bmp; *.dib|16色位图 (*.bmp;*.dib)|*.bmp; *.dib|256色位图 (*.bmp;*.dib)|*.bmp; *.dib|24位位图 (*.bmp;*.dib)|*.bmp; *.dib|GIF文件 (*.gif)|*.gif|JPEG文件 (*.jpg;*.jpe;*.jpeg)|*.jpg; *.jpe; *.jpeg|PCX文件 (*.pcx)|*.pcx|TGA文件 (*.tga)|*.tga|TIFF文件 (*.tif;*.tiff)|*.tif; *.tif|All Files (*.*)|*.*||";

/////////////////////////////////////////////////////////////////////////////
// CImageBoardApp:
// See ImageBoard.cpp for the implementation of this class
//

class CImageBoardApp : public CWinApp
{
public:
	CImageBoardApp();
	void AddFile(HDIB hDIB, LPCTSTR lpszTitle);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageBoardApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageBoardApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnPasteasfile();
	afx_msg void OnUpdatePasteasfile(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEBOARD_H__CEBADA04_ED34_11D2_9481_000021003EA5__INCLUDED_)
