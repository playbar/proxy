#if !defined(AFX_PENWIDTH_H__FDEDE6A3_EE65_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_PENWIDTH_H__FDEDE6A3_EE65_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PenWidth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPenWidth dialog

class CPenWidth : public CDialog
{
// Construction
public:
	CPenWidth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPenWidth)
	enum { IDD = IDD_PENWIDTH };
	UINT	m_nPenWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenWidth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPenWidth)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PENWIDTH_H__FDEDE6A3_EE65_11D2_9481_000021003EA5__INCLUDED_)
