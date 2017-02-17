#if !defined(AFX_BCDLG_H__A8EC2D60_A748_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_BCDLG_H__A8EC2D60_A748_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCDlg dialog

class CBCDlg : public CDialog
{
// Construction
public:
	CBCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBCDlg)
	enum { IDD = IDD_BC };
	CScrollBar	m_ScrollContrast;
	CScrollBar	m_ScrollBright;
	CString	m_strBright;
	CString	m_strContrast;
	//}}AFX_DATA

	int	m_nValue1;
	int	m_nValue2;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCDLG_H__A8EC2D60_A748_11D2_9481_000021003EA5__INCLUDED_)
