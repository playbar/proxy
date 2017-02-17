#if !defined(AFX_MACROSBUTTON_H__2CC4B1EA_8120_42AB_A56E_CBED2C2A2D6C__INCLUDED_)
#define AFX_MACROSBUTTON_H__2CC4B1EA_8120_42AB_A56E_CBED2C2A2D6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacrosButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMacrosButton window

class CMacrosButton : public CButton
{
// Construction
public:
	CMacrosButton();
    
// Attributes
public:
   CBitmap   *m_pNormal,*m_pSelected,*m_pHover, *m_pDisabled;
   //BOOL      m_bHover;
   //按钮的状态
	BOOL m_bOver;	//鼠标位于按钮之上时该值为true，反之为flase
	BOOL m_bTracking;	//在鼠标按下没有释放时该值为true
	BOOL m_bSelected;	//按钮被按下是该值为true
	BOOL m_bFocus;	//按钮为当前焦点所在时该值为true
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacrosButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMacrosButton();

	// Generated message map functions
protected:
	void DrawBitmap(CDC * pDC, CRect rc,UINT nFlags,CBitmap * pBitmap);
	void DrawCaption(CDC*  pDC,CRect *pRect,int  stateItem, COLORREF crColor );
	//{{AFX_MSG(CMacrosButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACROSBUTTON_H__2CC4B1EA_8120_42AB_A56E_CBED2C2A2D6C__INCLUDED_)
