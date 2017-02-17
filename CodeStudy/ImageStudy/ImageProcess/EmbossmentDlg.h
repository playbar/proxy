#if !defined(AFX_EMBOSSMENGDLG_H__AC082C9A_BB38_458F_9C63_0E7B7B898950__INCLUDED_)
#define AFX_EMBOSSMENGDLG_H__AC082C9A_BB38_458F_9C63_0E7B7B898950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmbossmengDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmbossmentDlg dialog

class CEmbossmentDlg : public CDialog
{
// Construction
public:
	CEmbossmentDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEmbossmentDlg)
	enum { IDD = IDD_EMBOSSMENT };
	CString	m_strValue;
	int		m_nValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmbossmentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmbossmentDlg)
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMBOSSMENGDLG_H__AC082C9A_BB38_458F_9C63_0E7B7B898950__INCLUDED_)
