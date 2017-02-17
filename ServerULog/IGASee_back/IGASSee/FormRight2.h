#pragma once
#include "ListCtrlEx.h"



// CFormRight2 窗体视图

class CFormRight2 : public CFormView
{
	DECLARE_DYNCREATE(CFormRight2)

protected:
	CFormRight2();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormRight2();

public:
	enum { IDD = IDD_FORMRIGHT2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CListCtrlEx m_listRight2;
	void UpdateList(CWnd * pWnd);
};


