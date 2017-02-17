#if !defined(AFX_JPEGFORMATDLG_H__61649D80_017D_11D3_9481_000021003EA5__INCLUDED_)
#define AFX_JPEGFORMATDLG_H__61649D80_017D_11D3_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JpegFormatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJpegFormatDlg dialog

class CJpegFormatDlg : public CDialog
{
// Construction
public:
	CJpegFormatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJpegFormatDlg)
	enum { IDD = IDD_JPEGFORMAT };
	CSliderCtrl	m_Quality;
	int		m_nQuality;
	CString	m_strQuality;
	//}}AFX_DATA

	BOOL m_bColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJpegFormatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJpegFormatDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JPEGFORMATDLG_H__61649D80_017D_11D3_9481_000021003EA5__INCLUDED_)
