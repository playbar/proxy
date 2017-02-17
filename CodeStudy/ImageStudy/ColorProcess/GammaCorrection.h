#if !defined(AFX_GAMMACORRECTION_H__EACFE343_C350_4906_81CF_81D786EC2617__INCLUDED_)
#define AFX_GAMMACORRECTION_H__EACFE343_C350_4906_81CF_81D786EC2617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GammaCorrection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGammaCorrection dialog

class CGammaCorrection : public CDialog
{
// Construction
public:
	CGammaCorrection(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGammaCorrection)
	enum { IDD = IDD_GAMMA };
	CString	m_strGamma;
	int		m_nGamma;
	//}}AFX_DATA

	float m_fGamma;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGammaCorrection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGammaCorrection)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMMACORRECTION_H__EACFE343_C350_4906_81CF_81D786EC2617__INCLUDED_)
