// FtpLog.h : main header file for the FTPLOG application
//

#if !defined(AFX_FTPLOG_H__C33EF870_E361_46EC_A68D_61DDCB5680ED__INCLUDED_)
#define AFX_FTPLOG_H__C33EF870_E361_46EC_A68D_61DDCB5680ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFtpLogApp:
// See FtpLog.cpp for the implementation of this class
//

class CFtpLogApp : public CWinApp
{
public:
	CFtpLogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpLogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFtpLogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPLOG_H__C33EF870_E361_46EC_A68D_61DDCB5680ED__INCLUDED_)
