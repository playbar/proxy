#if !defined(AFX_PALETTEWND_H__0CDF5F83_B2C6_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_PALETTEWND_H__0CDF5F83_B2C6_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaletteWnd window

class CPaletteWnd : public CWnd
{
// Construction
public:
	CPaletteWnd(LPCSTR lpstrTitle, CWnd* pParentWnd);

// Attributes
public:
	CToolTipCtrlEx  m_ToolTip;
	COLORREF		m_crSelected;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaletteWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaletteWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTEWND_H__0CDF5F83_B2C6_11D2_9481_000021003EA5__INCLUDED_)
