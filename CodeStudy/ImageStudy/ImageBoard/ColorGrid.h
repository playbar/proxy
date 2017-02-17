#if !defined(AFX_COLORGRID_H__4732AF84_EE89_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_COLORGRID_H__4732AF84_EE89_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorGrid window

class CColorGrid : public CButton
{
// Construction
public:
	CColorGrid();

// Attributes
public:

// Operations
public:
	void	SetColor(COLORREF crGround);
	void	SetCursorHandle(HCURSOR hCursor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorGrid)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorGrid();

	// Generated message map functions
protected:
	HCURSOR		m_hCursor;
	COLORREF	m_crColor;
	void		DrawBorder(CDC *pDC);

	//{{AFX_MSG(CColorGrid)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORGRID_H__4732AF84_EE89_11D2_9481_000021003EA5__INCLUDED_)
