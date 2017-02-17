// ShowDIBView.h : interface of the CShowDIBView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWDIBVIEW_H__512EDD08_DFEB_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_SHOWDIBVIEW_H__512EDD08_DFEB_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShowDIBView : public CScrollView
{
protected: // create from serialization only
	CShowDIBView();
	DECLARE_DYNCREATE(CShowDIBView)

// Attributes
public:
	CShowDIBDoc* GetDocument();
	CRect	m_rcDIB;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowDIBView)
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
	virtual ~CShowDIBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShowDIBView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ShowDIBView.cpp
inline CShowDIBDoc* CShowDIBView::GetDocument()
   { return (CShowDIBDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWDIBVIEW_H__512EDD08_DFEB_11D2_9481_000021003EA5__INCLUDED_)
