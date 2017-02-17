#if !defined(AFX_EXPOSUREDLG_H__C6751477_98B7_4212_93DE_A0BC28269406__INCLUDED_)
#define AFX_EXPOSUREDLG_H__C6751477_98B7_4212_93DE_A0BC28269406__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExposureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExposureDlg dialog

class CExposureDlg : public CDialog
{
// Construction
public:
	CExposureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExposureDlg)
	enum { IDD = IDD_EXPOSURE };
	UINT	m_nThreshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExposureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExposureDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPOSUREDLG_H__C6751477_98B7_4212_93DE_A0BC28269406__INCLUDED_)
