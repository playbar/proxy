#if !defined(AFX_CANVASSIZE_H__F8B57F25_F05B_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_CANVASSIZE_H__F8B57F25_F05B_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanvasSize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCanvasSize dialog

class CCanvasSize : public CDialog
{
// Construction
public:
	CCanvasSize(int nWidth, int nHeight, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCanvasSize)
	enum { IDD = IDD_CANVASSIZE };
	CBitmapButton	m_btUR;
	CBitmapButton	m_btUp;
	CBitmapButton	m_btUL;
	CBitmapButton	m_btRight;
	CBitmapButton	m_btLeft;
	CBitmapButton	m_btDR;
	CBitmapButton	m_btDown;
	CBitmapButton	m_btDL;
	CBitmapButton	m_btCenter;
	BOOL	m_bProportional;
	CString	m_strHeight;
	CString	m_strWidth;
	//}}AFX_DATA

	double	m_fRatio;
	int		m_nPosition;
	int		m_nUnit;
	int		m_nWidth;
	int		m_nHeight;
	int		m_nWidth0;
	int		m_nHeight0;
	int		m_nPixelsPInchX;
	int		m_nPixelsPInchY;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvasSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	SetPositionButtonsImage(int nOldPosition);

	// Generated message map functions
	//{{AFX_MSG(CCanvasSize)
	virtual BOOL OnInitDialog();
	afx_msg void OnCenter();
	afx_msg void OnCm();
	afx_msg void OnDl();
	afx_msg void OnDown();
	afx_msg void OnDr();
	afx_msg void OnChangeHeight();
	afx_msg void OnInch();
	afx_msg void OnLeft();
	afx_msg void OnPixel();
	afx_msg void OnProportional();
	afx_msg void OnReset();
	afx_msg void OnRight();
	afx_msg void OnUl();
	afx_msg void OnUp();
	afx_msg void OnUr();
	afx_msg void OnChangeWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASSIZE_H__F8B57F25_F05B_11D2_9481_000021003EA5__INCLUDED_)
