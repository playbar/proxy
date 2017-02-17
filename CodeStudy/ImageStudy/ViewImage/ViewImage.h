// ViewImage.h : main header file for the ViewImage application
//

#if !defined(AFX_ViewImage_H__E0DA1D84_E308_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_ViewImage_H__E0DA1D84_E308_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "dib.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CViewImageApp:
// See ViewImage.cpp for the implementation of this class
//

class CViewImageApp : public CWinApp
{
public:
	CViewImageApp();

	void AddFile(HDIB hDIB, LPCTSTR lpszTitle);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewImageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CViewImageApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ViewImage_H__E0DA1D84_E308_11D2_9481_000021003EA5__INCLUDED_)
