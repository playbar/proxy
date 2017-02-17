#if !defined(AFX_TOOLTIPCTRLEX_H__03290C01_6198_11D1_8FEC_000021003EA5__INCLUDED_)
#define AFX_TOOLTIPCTRLEX_H__03290C01_6198_11D1_8FEC_000021003EA5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolTipCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrlEx window

class CToolTipCtrlEx : public CToolTipCtrl
{
// Construction
public:
	CToolTipCtrlEx();

// Attributes
public:

// Operations
public:
	BOOL	AddRectangle(CWnd* pWnd,
						 LPCSTR pszTooltip,
						 LPCRECT pRect,
						 UINT uIDTool);
	BOOL	AddWindow(CWnd* pWnd,
					  LPCSTR pszTooltip);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTipCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipCtrlEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPCTRLEX_H__03290C01_6198_11D1_8FEC_000021003EA5__INCLUDED_)
