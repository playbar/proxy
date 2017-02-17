#if !defined(AFX_MASTATIC_H__15283874_5E75_4A51_8F68_FBB5D7BDED5E__INCLUDED_)
#define AFX_MASTATIC_H__15283874_5E75_4A51_8F68_FBB5D7BDED5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaStatic window

class CMaStatic : public CStatic
{
// Construction
public:
	CMaStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMaStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMaStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASTATIC_H__15283874_5E75_4A51_8F68_FBB5D7BDED5E__INCLUDED_)
