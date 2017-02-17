// FormLeft1.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormLeft1.h"
#include "FormRight1.h"
#include "MainFrm.h"
#include "PathDialog.h"
#include ".\formleft1.h"


// CFormLeft1

IMPLEMENT_DYNCREATE(CFormLeft1, CFormView)

CFormLeft1::CFormLeft1()
	: CFormView(CFormLeft1::IDD)
{
}

CFormLeft1::~CFormLeft1()
{
}

void CFormLeft1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_POSITION1, m_combPosition1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START1, m_DTCtrlStart1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END1, m_DTCtrlEnd1);
}

BEGIN_MESSAGE_MAP(CFormLeft1, CFormView)
	ON_BN_CLICKED(IDC_BTN_BROWSE1, OnBnClickedBtnBrowse1)
	ON_BN_CLICKED(IDC_BTN_SEE1, OnBnClickedBtnSee1)
END_MESSAGE_MAP()


// CFormLeft1 诊断

#ifdef _DEBUG
void CFormLeft1::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormLeft1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormLeft1 消息处理程序

void CFormLeft1::OnBnClickedBtnBrowse1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath;
	CString strCaption(_T("文件位置"));
	CString strTitle(_T("路径:"));

	m_combPosition1.GetWindowText(strPath);

	CPathDialog dlg(strCaption, strTitle, strPath);
	if(dlg.DoModal()==IDOK)
	{
		m_combPosition1.SetWindowText(dlg.GetPathName());
	}
}

void CFormLeft1::OnBnClickedBtnSee1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_combPosition1.GetWindowText(m_strPath);
	m_DTCtrlStart1.GetTime(m_tmStart);
	m_DTCtrlEnd1.GetTime(m_tmEnd);

	// zzhenfei添加 判断时间
	if ( m_tmEnd < m_tmStart )
	{
		AfxMessageBox("请确定是否填写正确的查询时间！");
		return ;
	}

	if (m_strPath.IsEmpty()) return;

	int nIndex = 0;
	BOOL bIsInserted = FALSE;
	if ((nIndex= m_combPosition1.FindString(nIndex, m_strPath)) != CB_ERR)
	{
		m_combPosition1.InsertString(0, m_strPath);
		m_combPosition1.SetCurSel(0);
		bIsInserted = TRUE;
		m_combPosition1.DeleteString( ++nIndex );
	}
	else
	{
		int nCount = m_combPosition1.GetCount();
		if (nCount >= MAX_PATH_KEY)
		{
			m_combPosition1.DeleteString(MAX_PATH_KEY - 1);

		}
	}

	if (!bIsInserted)
	{
		m_combPosition1.InsertString(0, m_strPath);
	}

	SavePath();

	CMainFrame *pMainWnd = (CMainFrame *)(::AfxGetApp()->GetMainWnd());
	ASSERT(pMainWnd != NULL);
	if (pMainWnd == NULL) return;

	CFormRight1 * pFormRight = (CFormRight1 *)(pMainWnd->GetOffsetObject(this, 1));
	if ((pFormRight != NULL) && ::IsWindow(pFormRight->GetSafeHwnd()))
	{
		pFormRight->UpdateList(this);
	}
}

void CFormLeft1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_config.SetFileName(GetConfigFileName());
	m_config.SetSection(_T("FORM_LEFT1"));
	UpdatePath();
}

void CFormLeft1::SavePath()
{
	int nCount = m_combPosition1.GetCount();

	CString strKey = _T("");
	CString strValue = _T("");
	for(int i=0; i<nCount; i++)
	{
		m_combPosition1.GetLBText(i, strValue);
		strKey.Format(_T("path_%02d"), i);
		m_config.SetValue(strKey, strValue);
	}

}

void CFormLeft1::UpdatePath()
{
	m_combPosition1.ResetContent();


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

		m_combPosition1.InsertString(i, strValue);
	}

	m_combPosition1.SetCurSel(0);
}