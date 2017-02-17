#if !defined(AFX_CONVOLUTEDLG_H__7C70D8F6_EC43_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_CONVOLUTEDLG_H__7C70D8F6_EC43_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConvoluteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConvoluteDlg dialog

class CConvoluteDlg : public CDialog
{
// Construction
public:
	CConvoluteDlg(int nMin, int nMax, int nDef, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConvoluteDlg)
	enum { IDD = IDD_CONVOLUTE };
	CSliderCtrl	m_Streng;
	CString	m_strValue;
	int		m_nStreng;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int m_nMax;
	int m_nMin;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConvoluteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConvoluteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONVOLUTEDLG_H__7C70D8F6_EC43_11D2_9481_000021003EA5__INCLUDED_)
