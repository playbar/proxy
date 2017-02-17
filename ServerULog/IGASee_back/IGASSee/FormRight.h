#pragma once
#include "ListCtrlEx.h"
#include "VisualFx.h"

// CFormRight 窗体视图

class CFormRight : public TVisualFormView
{
	DECLARE_DYNCREATE(CFormRight)

protected:
	CFormRight();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormRight();

public:
	enum { IDD = IDD_FORMRIGHT };
;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	

public:
	virtual void OnInitialUpdate();
	void UpdateList(CWnd *pWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CListCtrlEx m_listRight;

};


