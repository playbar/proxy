// TransformShow.h : main header file for the TRANSFORMSHOW application
//

#if !defined(AFX_TRANSFORMSHOW_H__E4CDE1A8_E755_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_TRANSFORMSHOW_H__E4CDE1A8_E755_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTransformShowApp:
// See TransformShow.cpp for the implementation of this class
//

class CTransformShowApp : public CWinApp
{
public:
	CTransformShowApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformShowApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTransformShowApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMSHOW_H__E4CDE1A8_E755_11D2_9481_000021003EA5__INCLUDED_)
