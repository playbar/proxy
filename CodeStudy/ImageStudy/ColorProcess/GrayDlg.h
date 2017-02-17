#if !defined(AFX_GRAYDLG_H__6CC3FA63_9687_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_GRAYDLG_H__6CC3FA63_9687_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrayDlg dialog

class CGrayDlg : public CDialog
{
// Construction
public:
	CGrayDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGrayDlg)
	enum { IDD = IDD_GRAY };
	CString	m_strWeightR;
	CString	m_strWeightG;
	CString	m_strWeightB;
	//}}AFX_DATA

	int m_nMethod;
	double m_fWeightR;
	double m_fWeightG;
	double m_fWeightB;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGrayDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAYDLG_H__6CC3FA63_9687_11D2_9481_000021003EA5__INCLUDED_)
