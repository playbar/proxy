#if !defined(AFX_ADJUSTCOLORDLG_H__6CC3FA65_9687_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_ADJUSTCOLORDLG_H__6CC3FA65_9687_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjustColorDlg dialog

class CAdjustColorDlg : public CDialog
{
// Construction
public:
	CAdjustColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdjustColorDlg)
	enum { IDD = IDD_COLOR };
	CScrollBar	m_ScrollBar2;
	CScrollBar	m_ScrollBar3;
	CScrollBar	m_ScrollBar1;
	CString	m_strValue1;
	CString	m_strValue2;
	CString	m_strValue3;
	CString	m_strName1;
	CString	m_strName2;
	CString	m_strName3;
	//}}AFX_DATA

	int m_nColorModel;
	int	m_nValue1;
	int	m_nValue2;
	int	m_nValue3;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjustColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRadio4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTCOLORDLG_H__6CC3FA65_9687_11D2_9481_000021003EA5__INCLUDED_)
