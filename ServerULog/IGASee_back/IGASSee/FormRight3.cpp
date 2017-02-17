// FormRight3.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormRight3.h"
#include "MainFrm.h"
#include "FormLeft3.h"


// CFormRight3

IMPLEMENT_DYNCREATE(CFormRight3, CFormView)

CFormRight3::CFormRight3()
	: CFormView(CFormRight3::IDD)
{
}

CFormRight3::~CFormRight3()
{
}

void CFormRight3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RIGHT3, m_listRight3);
}

BEGIN_MESSAGE_MAP(CFormRight3, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFormRight3 诊断

#ifdef _DEBUG
void CFormRight3::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormRight3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormRight3 消息处理程序

void CFormRight3::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_listRight3.SetExtendedStyle(
    LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );

	int nColCount = 0;
	m_listRight3.InsertColumn(nColCount++, _T("序号"), LVCFMT_LEFT, 60);
	m_listRight3.InsertColumn(nColCount++, _T("日期"), LVCFMT_LEFT, 160);
	m_listRight3.InsertColumn(nColCount++, _T("游戏程序名称"), LVCFMT_LEFT, 100);
	m_listRight3.InsertColumn(nColCount++, _T("游戏版本"), LVCFMT_LEFT, 100);
	m_listRight3.InsertColumn(nColCount++, _T("SDK版本"), LVCFMT_LEFT, 100);
	m_listRight3.InsertColumn(nColCount++, _T("IP地址"), LVCFMT_LEFT, 100);
	m_listRight3.InsertColumn(nColCount++, _T("MAC地址"), LVCFMT_LEFT, 80);
	m_listRight3.InsertColumn(nColCount++, _T("类型"), LVCFMT_LEFT, 80);
	m_listRight3.InsertColumn(nColCount++, _T("区域"), LVCFMT_LEFT, 100);
	
	CRect clientRect;
	this->GetClientRect(&clientRect);
	if (::IsWindow(m_listRight3.GetSafeHwnd()))
	{
		m_listRight3.MoveWindow(&clientRect);
	}
}

void CFormRight3::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (::IsWindow(m_listRight3.GetSafeHwnd()))
	{
		m_listRight3.MoveWindow(0, 0, cx, cy);
	}
}

void CFormRight3::UpdateList(CWnd *pWnd)
{
	ASSERT(pWnd != NULL);
	if (pWnd == NULL) return;

	m_listRight3.DeleteAllItems();

	CString strPath = _T("");
	CTime timeStart, timeEnd;

	CFormLeft3 * pFormLeft = (CFormLeft3 *)pWnd;
	if ((pFormLeft != NULL) && ::IsWindow(pFormLeft->GetSafeHwnd()))
	{
		 strPath = pFormLeft->m_strPath;
		 timeStart = pFormLeft->m_tmStart;
		 timeEnd = pFormLeft->m_tmEnd;
	}

	AUTOAPPENDBRK(strPath);
	CTimeSpan ts(1, 0, 0, 0);
	int nRow = 0;

	do 
	{
		CString strFilePath = _T("");
		CString strFileName = timeStart.Format(_T("%Y%m%d"));;
		strFilePath.Format(_T("%s%s_%s%s"), strPath, IDS_SECTION_SDK, strFileName, LOG_EXT);

		CString strTemp = _T("");
		int nCol = 1;
		CStdioFile sf;
		
		if (sf.Open(strFilePath, CFile::modeRead|CFile::typeText|CFile::shareDenyNone))
		{
			CString strLine = _T("");
			while (sf.ReadString(strLine))
			{
				// 解析行
				int iStart = 0;
				int iPos = 0;
				int iIndex = 0;
				map<int, CString> sdkInfo;
				while((iPos = strLine.Find('|', iStart)) != -1)
				{
					sdkInfo[iIndex++] = strLine.Mid(iStart, iPos-iStart).Trim();
					iStart = iPos+1;
				}

				if (iIndex >= 8)
				{
					strTemp.Format(_T("%d"), nRow + 1);
					int nItemInserted = m_listRight3.InsertItem(nRow, strTemp);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[0], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[1], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[2], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[3], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[4], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[5], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, g_szGP[atoi(sdkInfo[6])], 0, 0, 0, 0);
					m_listRight3.SetItem(nItemInserted, nCol++, LVIF_TEXT, sdkInfo[7], 0, 0, 0, 0);
					
					nRow++;
					nCol = 1;
				}
				
			}

			sf.Close();
		}


		timeStart += ts;
		
	}while (timeStart < timeEnd);

	m_listRight3.SetOneselfItemData();
}