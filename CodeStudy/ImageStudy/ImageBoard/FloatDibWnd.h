#if !defined(AFX_FLOATDIBWND_H__0B12E5E1_AFF2_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_FLOATDIBWND_H__0B12E5E1_AFF2_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatDibWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloatDibWnd window

class CFloatDibWnd : public CWnd
{
// Construction
public:
	CFloatDibWnd(HDIB hDib, CRect rc, CWnd* pParentWnd=NULL);

// Attributes
public:
	HDIB	m_hDibFloat;
	HCURSOR	m_hCursorFloat;
	BOOL	m_bFirst;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatDibWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFloatDibWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFloatDibWnd)
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATDIBWND_H__0B12E5E1_AFF2_11D2_9481_000021003EA5__INCLUDED_)
