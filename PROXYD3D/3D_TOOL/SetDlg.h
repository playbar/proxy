#pragma once
#include "afxwin.h"


// CSetDlg 对话框

class CSetDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDlg();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_SET, IDH = IDR_HTML_SETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
private:
	CString m_cstrResourcePath;
	CString m_cstrResourceOutputPath;
	void SelectPath(char* path);

public:
	CString GetResourcePath(void);
	CString GetResourceOutputPath(void);
	afx_msg void OnBnClickedSetOk();
	afx_msg void OnBnClickedSetCancel();
	afx_msg void OnBnClickedButtonSetBrowse();
	afx_msg void OnBnClickedButtonSetResOutBrowse();
private:
	CEdit m_EditOutputPath;
public:
	void SetPath(CString ResourcePath, CString outputPath);
};
