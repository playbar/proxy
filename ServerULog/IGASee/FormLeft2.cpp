// FormLeft2.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormLeft2.h"
#include "FormRight2.h"
#include "MainFrm.h"
#include "PathDialog.h"
#include ".\formleft2.h"


// CFormLeft2

IMPLEMENT_DYNCREATE(CFormLeft2, CFormView)

CFormLeft2::CFormLeft2()
	: CFormView(CFormLeft2::IDD)
{
}

CFormLeft2::~CFormLeft2()
{
}

void CFormLeft2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_POSITION2, m_combPosition2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START2, m_DTCtrlStart2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END2, m_DTCtrlEnd2);
}

BEGIN_MESSAGE_MAP(CFormLeft2, CFormView)
	ON_BN_CLICKED(IDC_BTN_BROWSE2, OnBnClickedBtnBrowse2)
	ON_BN_CLICKED(IDC_BTN_SEE2, OnBnClickedBtnSee2)
END_MESSAGE_MAP()


// CFormLeft2 诊断

#ifdef _DEBUG
void CFormLeft2::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormLeft2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormLeft2 消息处理程序

void CFormLeft2::OnBnClickedBtnBrowse2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath;
	CString strCaption(_T("文件位置"));
	CString strTitle(_T("路径:"));

	m_combPosition2.GetWindowText(strPath);

	CPathDialog dlg(strCaption, strTitle, strPath);
	if(dlg.DoModal()==IDOK)
	{
		m_combPosition2.SetWindowText(dlg.GetPathName());
	}
}

void CFormLeft2::OnBnClickedBtnSee2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_combPosition2.GetWindowText(m_strPath);
	m_DTCtrlStart2.GetTime(m_tmStart);
	m_DTCtrlEnd2.GetTime(m_tmEnd);


	// zzhenfei添加 判断时间
	if ( m_tmEnd < m_tmStart )
	{
		AfxMessageBox("请确定是否填写正确的查询时间！");
		return ;
	}

	if (m_strPath.IsEmpty()) return;

	int nIndex = 0;
	BOOL bIsInserted = FALSE;
	if ((nIndex= m_combPosition2.FindString(nIndex, m_strPath)) != CB_ERR)
	{
		m_combPosition2.InsertString(0, m_strPath);
		m_combPosition2.SetCurSel(0);
		bIsInserted = TRUE;
		m_combPosition2.DeleteString( ++nIndex );
	}
	else
	{
		int nCount = m_combPosition2.GetCount();
		if (nCount >= MAX_PATH_KEY)
		{
			m_combPosition2.DeleteString(MAX_PATH_KEY - 1);

		}
	}

	if (!bIsInserted)
	{
		m_combPosition2.InsertString(0, m_strPath);
	}

	SavePath();


	CMainFrame *pMainWnd = (CMainFrame *)(::AfxGetApp()->GetMainWnd());
	ASSERT(pMainWnd != NULL);
	if (pMainWnd == NULL) return;

	CFormRight2 * pFormRight = (CFormRight2 *)(pMainWnd->GetOffsetObject(this, 1));
	if ((pFormRight != NULL) && ::IsWindow(pFormRight->GetSafeHwnd()))
	{
		pFormRight->UpdateList(this);
	}
}

void CFormLeft2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_config.SetFileName(GetConfigFileName());
	m_config.SetSection(_T("FORM_LEFT2"));
	UpdatePath();

}

void CFormLeft2::SavePath()
{
	int nCount = m_combPosition2.GetCount();

	CString strKey = _T("");
	CString strValue = _T("");
	for(int i=0; i<nCount; i++)
	{
		m_combPosition2.GetLBText(i, strValue);
		strKey.Format(_T("path_%02d"), i);
		m_config.SetValue(strKey, strValue);
	}

}

void CFormLeft2::UpdatePath()
{
	m_combPosition2.ResetContent();


	CString strKey = _T("");
	CString strValue = _T("");
	for(int i=0; i<MAX_PATH_KEY; i++)
	{
		strKey.Format(_T("path_%02d"), i);
		m_config.GetValue(strKey, strValue);
		if (strValue.IsEmpty())
		{
			break;
		}

		m_combPosition2.InsertString(i, strValue);
	}

	m_combPosition2.SetCurSel(0);
}

