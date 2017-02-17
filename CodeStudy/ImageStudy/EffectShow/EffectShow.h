// EffectShow.h : main header file for the EFFECTSHOW application
//

#if !defined(AFX_EFFECTSHOW_H__81703004_E560_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_EFFECTSHOW_H__81703004_E560_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEffectShowApp:
// See EffectShow.cpp for the implementation of this class
//

class CEffectShowApp : public CWinApp
{
public:
	CEffectShowApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectShowApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEffectShowApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTSHOW_H__81703004_E560_11D2_9481_000021003EA5__INCLUDED_)
