#if !defined(AFX_SPREADDLG_H__12ACA79E_09EA_4A8F_A4C9_AB11F0EF04C5__INCLUDED_)
#define AFX_SPREADDLG_H__12ACA79E_09EA_4A8F_A4C9_AB11F0EF04C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpreadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpreadDlg dialog

class CSpreadDlg : public CDialog
{
// Construction
public:
	CSpreadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpreadDlg)
	enum { IDD = IDD_SPREAD };
	int		m_nValue;
	CString	m_strValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpreadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpreadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPREADDLG_H__12ACA79E_09EA_4A8F_A4C9_AB11F0EF04C5__INCLUDED_)
