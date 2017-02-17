#if !defined(AFX_CANVASDLG_H__43F5D660_E76A_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_CANVASDLG_H__43F5D660_E76A_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanvasDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCanvasDlg dialog

class CCanvasDlg : public CDialog
{
// Construction
public:
	CCanvasDlg(int nWidth, int nHeight, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCanvasDlg)
	enum { IDD = IDD_CANVAS };
	BOOL	m_bProportionXY;
	int		m_nWidth;
	int		m_nHeight;
	//}}AFX_DATA

	double m_fRatio;
	int	   m_nPosition;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvasDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCanvasDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	afx_msg void OnRadio9();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnCheck1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASDLG_H__43F5D660_E76A_11D2_9481_000021003EA5__INCLUDED_)
