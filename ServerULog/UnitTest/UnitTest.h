// UnitTest.h : main header file for the UNITTEST application
//

#if !defined(AFX_UNITTEST_H__C3F4A35C_7FFA_41B8_81DD_FF0BB5B0B36F__INCLUDED_)
#define AFX_UNITTEST_H__C3F4A35C_7FFA_41B8_81DD_FF0BB5B0B36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnitTestApp:
// See UnitTest.cpp for the implementation of this class
//

class CUnitTestApp : public CWinApp
{
public:
	CUnitTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnitTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnitTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNITTEST_H__C3F4A35C_7FFA_41B8_81DD_FF0BB5B0B36F__INCLUDED_)
