#if !defined(AFX_COLORBITSDLG_H__A1CF1980_AFA3_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_COLORBITSDLG_H__A1CF1980_AFA3_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBitsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBitsDlg dialog

class CColorBitsDlg : public CDialog
{
// Construction
public:
	CColorBitsDlg(int nColorBits, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorBitsDlg)
	enum { IDD = IDD_COLORBITS };
	//}}AFX_DATA


	int m_nColorBits;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBitsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorBitsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio24();
	afx_msg void OnRadio4();
	afx_msg void OnRadio8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBITSDLG_H__A1CF1980_AFA3_11D2_9481_000021003EA5__INCLUDED_)
