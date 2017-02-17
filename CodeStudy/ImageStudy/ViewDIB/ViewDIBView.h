// ViewDIBView.h : interface of the CViewDIBView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWDIBVIEW_H__E0DA1D8E_E308_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_VIEWDIBVIEW_H__E0DA1D8E_E308_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewDIBView : public CScrollView
{
protected: // create from serialization only
	CViewDIBView();
	DECLARE_DYNCREATE(CViewDIBView)

// Attributes
public:
	CViewDIBDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDIBView)
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
	virtual ~CViewDIBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CViewDIBView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ViewDIBView.cpp
inline CViewDIBDoc* CViewDIBView::GetDocument()
   { return (CViewDIBDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDIBVIEW_H__E0DA1D8E_E308_11D2_9481_000021003EA5__INCLUDED_)
