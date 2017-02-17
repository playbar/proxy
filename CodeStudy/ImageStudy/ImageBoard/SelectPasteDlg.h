#if !defined(AFX_SELECTPASTEDLG_H__0CDF5F80_B2C6_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_SELECTPASTEDLG_H__0CDF5F80_B2C6_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectPasteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectPasteDlg dialog

class CSelectPasteDlg : public CDialog
{
// Construction
public:
	CSelectPasteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectPasteDlg)
	enum { IDD = IDD_SELECTPASTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int	m_nCF;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectPasteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectPasteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioDib();
	afx_msg void OnRadioBitmap();
	afx_msg void OnRadioPalette();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTPASTEDLG_H__0CDF5F80_B2C6_11D2_9481_000021003EA5__INCLUDED_)
