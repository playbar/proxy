#if !defined(AFX_SCALEDLG_H__43F5D661_E76A_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_SCALEDLG_H__43F5D661_E76A_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog

class CScaleDlg : public CDialog
{
// Construction
public:
	CScaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScaleDlg)
	enum { IDD = IDD_SCALE };
	CString	m_strXRatio;
	BOOL	m_bProportionXY;
	CString	m_strYRatio;
	//}}AFX_DATA

	double	m_fRatioX;
	double	m_fRatioY;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScaleDlg)
	afx_msg void OnCheck1();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEDLG_H__43F5D661_E76A_11D2_9481_000021003EA5__INCLUDED_)
