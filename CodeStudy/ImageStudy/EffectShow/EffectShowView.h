// EffectShowView.h : interface of the CEffectShowView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTSHOWVIEW_H__8170300E_E560_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_EFFECTSHOWVIEW_H__8170300E_E560_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEffectShowView : public CScrollView
{
protected: // create from serialization only
	CEffectShowView();
	DECLARE_DYNCREATE(CEffectShowView)

// Attributes
public:
	CEffectShowDoc* GetDocument();
	int		m_nEffect;

// Operations
public:
	void EffectDisplay(int nEffect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectShowView)
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
	virtual ~CEffectShowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEffectShowView)
	afx_msg void OnFadein();
	afx_msg void OnUpdateFadein(CCmdUI* pCmdUI);
	afx_msg void OnHblind();
	afx_msg void OnUpdateHblind(CCmdUI* pCmdUI);
	afx_msg void OnHcrossmove();
	afx_msg void OnUpdateHcrossmove(CCmdUI* pCmdUI);
	afx_msg void OnHraster();
	afx_msg void OnUpdateHraster(CCmdUI* pCmdUI);
	afx_msg void OnHsplitmove();
	afx_msg void OnUpdateHsplitmove(CCmdUI* pCmdUI);
	afx_msg void OnHsplitscan();
	afx_msg void OnUpdateHsplitscan(CCmdUI* pCmdUI);
	afx_msg void OnMosaic();
	afx_msg void OnUpdateMosaic(CCmdUI* pCmdUI);
	afx_msg void OnMovedown();
	afx_msg void OnUpdateMovedown(CCmdUI* pCmdUI);
	afx_msg void OnMoveleft();
	afx_msg void OnUpdateMoveleft(CCmdUI* pCmdUI);
	afx_msg void OnMoveright();
	afx_msg void OnUpdateMoveright(CCmdUI* pCmdUI);
	afx_msg void OnMoveup();
	afx_msg void OnUpdateMoveup(CCmdUI* pCmdUI);
	afx_msg void OnScandown();
	afx_msg void OnUpdateScandown(CCmdUI* pCmdUI);
	afx_msg void OnScanleft();
	afx_msg void OnUpdateScanleft(CCmdUI* pCmdUI);
	afx_msg void OnScanright();
	afx_msg void OnUpdateScanright(CCmdUI* pCmdUI);
	afx_msg void OnScanup();
	afx_msg void OnUpdateScanup(CCmdUI* pCmdUI);
	afx_msg void OnVblind();
	afx_msg void OnUpdateVblind(CCmdUI* pCmdUI);
	afx_msg void OnVcrossmove();
	afx_msg void OnUpdateVcrossmove(CCmdUI* pCmdUI);
	afx_msg void OnVraster();
	afx_msg void OnUpdateVraster(CCmdUI* pCmdUI);
	afx_msg void OnVsplitmove();
	afx_msg void OnUpdateVsplitmove(CCmdUI* pCmdUI);
	afx_msg void OnVsplitscan();
	afx_msg void OnUpdateVsplitscan(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EffectShowView.cpp
inline CEffectShowDoc* CEffectShowView::GetDocument()
   { return (CEffectShowDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTSHOWVIEW_H__8170300E_E560_11D2_9481_000021003EA5__INCLUDED_)
