#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "VisualFx.h"
#include "Config.h"



// CFormLeft 窗体视图

class CFormLeft : public TVisualFormView
{
	DECLARE_DYNCREATE(CFormLeft)

protected:
	CFormLeft();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormLeft();

public:
	enum { IDD = IDD_FORMLEFT };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnSee();

	CComboBox m_combPosition;
	CDateTimeCtrl m_DTCtrlStart;
	CDateTimeCtrl m_DTCtrlEnd;
	CINIConfig m_config;

public:
	CString m_strPath;
	CTime m_tmStart;
	CTime m_tmEnd;
	virtual void OnInitialUpdate();
	void SavePath();
	void UpdatePath();
};


