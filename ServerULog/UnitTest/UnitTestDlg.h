// UnitTestDlg.h : header file
//

#if !defined(AFX_UNITTESTDLG_H__C044A7B8_3B7D_4ED8_B5DC_27926D73462B__INCLUDED_)
#define AFX_UNITTESTDLG_H__C044A7B8_3B7D_4ED8_B5DC_27926D73462B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUnitTestDlg dialog

class CUnitTestDlg : public CDialog
{
// Construction
public:
	CUnitTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUnitTestDlg)
	enum { IDD = IDD_UNITTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnitTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUnitTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNITTESTDLG_H__C044A7B8_3B7D_4ED8_B5DC_27926D73462B__INCLUDED_)
