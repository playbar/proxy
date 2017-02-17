// CompressFileDlg.h : header file
//

#pragma once

#include "MacrosButton.h"
#include "MaStatic.h"


// CCompressFileDlg dialog
class CCompressFileDlg : public CDialog
{
// Construction
public:
	CCompressFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COMPRESSFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCompress();

	CBitmap			m_bmpBackground;
	CMacrosButton   m_btnPatch;
	CMacrosButton   m_btnOk;
	CMaStatic       m_StcInfo;
	CMaStatic		m_StcPatch;
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
