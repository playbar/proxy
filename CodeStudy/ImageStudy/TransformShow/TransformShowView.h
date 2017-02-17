// TransformShowView.h : interface of the CTransformShowView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORMSHOWVIEW_H__E4CDE1B2_E755_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_TRANSFORMSHOWVIEW_H__E4CDE1B2_E755_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTransformShowView : public CScrollView
{
protected: // create from serialization only
	CTransformShowView();
	DECLARE_DYNCREATE(CTransformShowView)

// Attributes
public:
	CTransformShowDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformShowView)
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
	virtual ~CTransformShowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTransformShowView)
	afx_msg void OnCanvassize();
	afx_msg void OnUpdateCanvassize(CCmdUI* pCmdUI);
	afx_msg void OnCrop();
	afx_msg void OnUpdateCrop(CCmdUI* pCmdUI);
	afx_msg void OnHmirror();
	afx_msg void OnUpdateHmirror(CCmdUI* pCmdUI);
	afx_msg void OnRotate();
	afx_msg void OnUpdateRotate(CCmdUI* pCmdUI);
	afx_msg void OnRotate180();
	afx_msg void OnUpdateRotate180(CCmdUI* pCmdUI);
	afx_msg void OnRotate270();
	afx_msg void OnUpdateRotate270(CCmdUI* pCmdUI);
	afx_msg void OnScale();
	afx_msg void OnUpdateScale(CCmdUI* pCmdUI);
	afx_msg void OnVmirror();
	afx_msg void OnUpdateVmirror(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnRotateAngle();
	afx_msg void OnUpdateRotateAngle(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TransformShowView.cpp
inline CTransformShowDoc* CTransformShowView::GetDocument()
   { return (CTransformShowDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMSHOWVIEW_H__E4CDE1B2_E755_11D2_9481_000021003EA5__INCLUDED_)
