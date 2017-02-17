// ColorProcessView.h : interface of the CColorProcessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORPROCESSVIEW_H__6BF5FA32_EA88_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_COLORPROCESSVIEW_H__6BF5FA32_EA88_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CColorProcessView : public CScrollView
{
protected: // create from serialization only
	CColorProcessView();
	DECLARE_DYNCREATE(CColorProcessView)

// Attributes
public:
	CColorProcessDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorProcessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CColorProcessView)
	afx_msg void OnAdjustcolor();
	afx_msg void OnUpdateAdjustcolor(CCmdUI* pCmdUI);
	afx_msg void OnBc();
	afx_msg void OnUpdateBc(CCmdUI* pCmdUI);
	afx_msg void OnColorbits();
	afx_msg void OnUpdateColorbits(CCmdUI* pCmdUI);
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnBlue();
	afx_msg void OnUpdateBlue(CCmdUI* pCmdUI);
	afx_msg void OnGetblue();
	afx_msg void OnUpdateGetblue(CCmdUI* pCmdUI);
	afx_msg void OnGetgreen();
	afx_msg void OnUpdateGetgreen(CCmdUI* pCmdUI);
	afx_msg void OnGetred();
	afx_msg void OnUpdateGetred(CCmdUI* pCmdUI);
	afx_msg void OnGreen();
	afx_msg void OnUpdateGreen(CCmdUI* pCmdUI);
	afx_msg void OnRed();
	afx_msg void OnUpdateRed(CCmdUI* pCmdUI);
	afx_msg void OnCorrectgamma();
	afx_msg void OnUpdateCorrectgamma(CCmdUI* pCmdUI);
	afx_msg void OnNegative();
	afx_msg void OnUpdateNegative(CCmdUI* pCmdUI);
	afx_msg void OnExposure();
	afx_msg void OnUpdateExposure(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ColorProcessView.cpp
inline CColorProcessDoc* CColorProcessView::GetDocument()
   { return (CColorProcessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPROCESSVIEW_H__6BF5FA32_EA88_11D2_9481_000021003EA5__INCLUDED_)
