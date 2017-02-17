#if !defined(AFX_PAINTPARAMBAR_H__4732AF86_EE89_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_PAINTPARAMBAR_H__4732AF86_EE89_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaintParambar.h : header file
//

#include "ColorGrid.h"
#include "PenParamsList.h"

#define COLOR_NUMBER	256

#define PP_FILL_COLOR	0
#define PP_PEN_COLOR	1

/////////////////////////////////////////////////////////////////////////////
// CPaintParambar window

class CPaintParambar : public CDialogBar
{
// Construction
public:
	CPaintParambar();

// Attributes
public:
	CColorGrid		m_btColor[COLOR_NUMBER];
	CColorGrid		m_btPenColor;
	CColorGrid		m_btFillColor;
	CPenParamsList	m_listPenWidth;
	CPenParamsList	m_listPenStyle;
	HPALETTE		m_hPalette;
	int				m_nColors;
	int				m_nWidth;
	int				m_nHeight;
	int				m_nSelectColorMode;
	CRect			m_rcFillColor;
	CRect			m_rcPenColor;
	HCURSOR			m_hCursorSelectColor;

// Operations
public:
	void	ShowPenColor(COLORREF crColor);
	void	ShowFillColor(COLORREF crColor);
	void	ShowPenWidth(int nWidth);
	void	ShowPenStyle(int nStyle);
	void	ShowColorGrid(HPALETTE hPalette);
	CView*	GetView();
	void	SetSelectColorMode(int nSelectColorMode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintParambar)
	protected:
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaintParambar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaintParambar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangePenstyle();
	afx_msg void OnSelchangePenwidth();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAINTPARAMBAR_H__4732AF86_EE89_11D2_9481_000021003EA5__INCLUDED_)
