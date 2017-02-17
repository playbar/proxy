#if !defined(AFX_ROTATEANGLE_H__1154C2A1_9A13_11D3_A4E9_000021003EA5__INCLUDED_)
#define AFX_ROTATEANGLE_H__1154C2A1_9A13_11D3_A4E9_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateAngle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotateAngle dialog

class CRotateAngle : public CDialog
{
// Construction
public:
	CRotateAngle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotateAngle)
	enum { IDD = IDD_ROTATEANGLE };
	double	m_fAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotateAngle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRotateAngle)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEANGLE_H__1154C2A1_9A13_11D3_A4E9_000021003EA5__INCLUDED_)
