#if !defined(AFX_SCALE_H__F8B57F26_F05B_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_SCALE_H__F8B57F26_F05B_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Scale.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScale dialog

class CScale : public CDialog
{
// Construction
public:
	CScale(int nWidth, int nHeight, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScale)
	enum { IDD = IDD_SCALE };
	BOOL	m_bProportional;
	CString	m_strHeight;
	CString	m_strWidth;
	//}}AFX_DATA

	double	m_fRatio;
	int		m_nUnit;
	int		m_nWidth;
	int		m_nHeight;
	int		m_nWidth0;
	int		m_nHeight0;
	int		m_nPixelsPInchX;
	int		m_nPixelsPInchY;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScale)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScale)
	afx_msg void OnCm();
	afx_msg void OnChangeHeight();
	afx_msg void OnInch();
	afx_msg void OnPixel();
	afx_msg void OnProportional();
	afx_msg void OnReset();
	afx_msg void OnChangeWidth();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALE_H__F8B57F26_F05B_11D2_9481_000021003EA5__INCLUDED_)
