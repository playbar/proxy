#pragma once
#include "ListCtrlEx.h"
#include "VisualFx.h"

// CFormRight 窗体视图

enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};

typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					// 系统当前时间
	char			m_szFileName[MAX_PATH]; //下载文件名
	char			m_szIp[16];				// 客户端IP
	float			m_fDownSpeed;			//下载速度
	DOWNFILE_STATE	m_enuDownFileState;		// 是否成功下载
} CDownFileInfo;

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


