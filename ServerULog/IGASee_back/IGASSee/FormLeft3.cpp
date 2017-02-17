// FormLeft3.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormLeft3.h"
#include "FormRight3.h"
#include "MainFrm.h"
#include "PathDialog.h"
#include ".\formleft3.h"


// CFormLeft3

IMPLEMENT_DYNCREATE(CFormLeft3, CFormView)

CFormLeft3::CFormLeft3()
	: CFormView(CFormLeft3::IDD)
{
}

CFormLeft3::~CFormLeft3()
{
}

void CFormLeft3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_POSITION3, m_combPosition3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START3, m_DTCtrlStart3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END3, m_DTCtrlEnd3);
}

BEGIN_MESSAGE_MAP(CFormLeft3, CFormView)
	ON_BN_CLICKED(IDC_BTN_BROWSE3, OnBnClickedBtnBrowse3)
	ON_BN_CLICKED(IDC_BTN_SEE3, OnBnClickedBtnSee3)
END_MESSAGE_MAP()


// CFormLeft3 诊断

#ifdef _DEBUG
void CFormLeft3::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormLeft3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormLeft3 消息处理程序

void CFormLeft3::OnBnClickedBtnBrowse3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath;
	CString strCaption(_T("文件位置"));
	CString strTitle(_T("路径:"));

	m_combPosition3.GetWindowText(strPath);

	CPathDialog dlg(strCaption, strTitle, strPath);
	if(dlg.DoModal()==IDOK)
	{
		m_combPosition3.SetWindowText(dlg.GetPathName());
	}
}

void CFormLeft3::OnBnClickedBtnSee3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_combPosition3.GetWindowText(m_strPath);
	m_DTCtrlStart3.GetTime(m_tmStart);
	m_DTCtrlEnd3.GetTime(m_tmEnd);

	// zzhenfei添加 判断时间
	if ( m_tmEnd < m_tmStart )
	{
		AfxMessageBox("请确定是否填写正确的查询时间！");
		return ;

	}
	if (m_strPath.IsEmpty()) return;

	int nIndex = 0;
	BOOL bIsInserted = FALSE;
	if ((nIndex= m_combPosition3.FindString(nIndex, m_strPath)) != CB_ERR)
	{
		m_combPosition3.InsertString(0, m_strPath);
		m_combPosition3.SetCurSel(0);
		bIsInserted = TRUE;
		m_combPosition3.DeleteString( ++nIndex );
	}
	else
	{
		int nCount = m_combPosition3.GetCount();
		if (nCount >= MAX_PATH_KEY)
		{
			m_combPosition3.DeleteString(MAX_PATH_KEY - 1);

		}
	}

	if (!bIsInserted)
	{
		m_combPosition3.InsertString(0, m_strPath);
	}

	SavePath();

	CMainFrame *pMainWnd = (CMainFrame *)(::AfxGetApp()->GetMainWnd());
	ASSERT(pMainWnd != NULL);
	if (pMainWnd == NULL) return;

	CFormRight3 * pFormRight = (CFormRight3 *)(pMainWnd->GetOffsetObject(this, 1));
	if ((pFormRight != NULL) && ::IsWindow(pFormRight->GetSafeHwnd()))
	{
		pFormRight->UpdateList(this);
	}
}

void CFormLeft3::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_config.SetFileName(GetConfigFileName());
	m_config.SetSection(_T("FORM_LEFT3"));
	UpdatePath();
}

void CFormLeft3::SavePath()
{
	int nCount = m_combPosition3.GetCount();

	CString strKey = _T("");
	CString strValue = _T("");
	for(int i=0; i<nCount; i++)
	{
		m_combPosition3.GetLBText(i, strValue);
		strKey.Format(_T("path_%02d"), i);
		m_config.SetValue(strKey, strValue);
	}

}

void CFormLeft3::UpdatePath()
{
	m_combPosition3.ResetContent();


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

		m_combPosition3.InsertString(i, strValue);
	}

	m_combPosition3.SetCurSel(0);
}

