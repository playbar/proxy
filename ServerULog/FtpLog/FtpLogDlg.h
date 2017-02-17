// FtpLogDlg.h : header file
//

#if !defined(AFX_FTPLOGDLG_H__236F7534_1E14_4536_A3E9_538DF7A05EEE__INCLUDED_)
#define AFX_FTPLOGDLG_H__236F7534_1E14_4536_A3E9_538DF7A05EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFtpLogDlg dialog
#include "GarbageCollector.h"
#	include <atlbase.h>
#	define	MAP_BASE	CSimpleMap		
#	define	ARRAY_BASE	CSimpleArray

struct CDownFileInfo
{
public:
	CDownFileInfo():m_enuDownFileState(false),m_fDownSpeed(0)
	{
		memset(&m_st, 0, sizeof(SYSTEMTIME));
		memset(m_szFileName, 0, sizeof(m_szFileName));
		memset(m_szIp, 0, sizeof(m_szIp));
	}
	~CDownFileInfo(){}
public:
	SYSTEMTIME	m_st;				// 系统当前时间
	char		m_szFileName[MAX_PATH]; //下载文件名
	char		m_szIp[16];				// 客户端IP
	float		m_fDownSpeed;			//下载速度
	bool		m_enuDownFileState;				// 是否成功下载
} ;

typedef ARRAY_BASE<CDownFileInfo*>	CDownFileInfos;

class CFtpLogDlg : public CDialog
{
// Construction
public:
	CFtpLogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpLogDlg)
	enum { IDD = IDD_FTPLOG_DIALOG };
	CString	m_strLogPath;
	CString	m_strSavePath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFtpLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDownFileInfos m_DownFileInfo;
	CGarbageCollector m_gc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPLOGDLG_H__236F7534_1E14_4536_A3E9_538DF7A05EEE__INCLUDED_)
