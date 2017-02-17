#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "Config.h"


// CFormLeft2 窗体视图

class CFormLeft2 : public CFormView
{
	DECLARE_DYNCREATE(CFormLeft2)

protected:
	CFormLeft2();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormLeft2();

public:
	enum { IDD = IDD_FORMLEFT2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CComboBox m_combPosition2;
	CDateTimeCtrl m_DTCtrlEnd2;
	CDateTimeCtrl m_DTCtrlStart2;
	CConfig m_config;
public:
	afx_msg void OnBnClickedBtnBrowse2();
	afx_msg void OnBnClickedBtnSee2();

public:
	CString m_strPath;
	CTime m_tmStart;
	CTime m_tmEnd;

	virtual void OnInitialUpdate();
	void SavePath();
	void UpdatePath();
};


