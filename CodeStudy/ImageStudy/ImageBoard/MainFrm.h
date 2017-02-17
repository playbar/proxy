// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E0DA1D88_E308_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_MAINFRM_H__E0DA1D88_E308_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaintToolbar.h"
#include "PaintParambar.h"

#define PANE_LENGTH_UNIT		1
#define PANE_CURSOR_POSITION	2
#define PANE_IMAGE_SIZE			3
#define PANE_BIT_COUNT			4

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndTextToolBar;
	CPaintToolbar  m_wndPaintToolBar;
	CPaintParambar m_wndPaintParamBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPainttool();
	afx_msg void OnUpdatePainttool(CCmdUI* pCmdUI);
	afx_msg void OnPaintparam();
	afx_msg void OnUpdatePaintparam(CCmdUI* pCmdUI);
	afx_msg void OnTexttool();
	afx_msg void OnUpdateTexttool(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E0DA1D88_E308_11D2_9481_000021003EA5__INCLUDED_)
