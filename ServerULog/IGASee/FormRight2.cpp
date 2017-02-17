// FormRight2.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormRight2.h"
#include "MainFrm.h"
#include "FormLeft2.h"


// CFormRight2

IMPLEMENT_DYNCREATE(CFormRight2, CFormView)

CFormRight2::CFormRight2()
	: CFormView(CFormRight2::IDD)
{
}

CFormRight2::~CFormRight2()
{
}

void CFormRight2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RIGHT2, m_listRight2);
}

BEGIN_MESSAGE_MAP(CFormRight2, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFormRight2 诊断

#ifdef _DEBUG
void CFormRight2::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormRight2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormRight2 消息处理程序

void CFormRight2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_listRight2.SetExtendedStyle(
    LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );

	int nColCount = 0;
	m_listRight2.InsertColumn(nColCount++, _T("序号"), LVCFMT_LEFT, 60);
	m_listRight2.InsertColumn(nColCount++, _T("日期"), LVCFMT_LEFT, 150);
	m_listRight2.InsertColumn(nColCount++, _T("游戏ID"), LVCFMT_LEFT, 80);
	m_listRight2.InsertColumn(nColCount++, _T("操作系统"), LVCFMT_LEFT, 100);
	m_listRight2.InsertColumn(nColCount++, _T("CPU"), LVCFMT_LEFT, 100);
	m_listRight2.InsertColumn(nColCount++, _T("内存"), LVCFMT_LEFT, 50);
	m_listRight2.InsertColumn(nColCount++, _T("显卡"), LVCFMT_LEFT, 100);
	m_listRight2.InsertColumn(nColCount++, _T("3D驱动"), LVCFMT_LEFT, 100);
	m_listRight2.InsertColumn(nColCount++, _T("MAC地址"), LVCFMT_LEFT, 80);
	m_listRight2.InsertColumn(nColCount++, _T("外部IP地址"), LVCFMT_LEFT, 100);
	m_listRight2.InsertColumn(nColCount++, _T("区域"), LVCFMT_LEFT, 100);

	//ZZhenFei 2008.06.18  11.01
	m_listRight2.InsertColumn(nColCount++, _T("内部IP地址"), LVCFMT_LEFT, 100);
	
	CRect clientRect;
	this->GetClientRect(&clientRect);
	if (::IsWindow(m_listRight2.GetSafeHwnd()))
	{
		m_listRight2.MoveWindow(&clientRect);
	}
}

void CFormRight2::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (::IsWindow(m_listRight2.GetSafeHwnd()))
	{
		m_listRight2.MoveWindow(0, 0, cx, cy);
	}
}

void CFormRight2::UpdateList(CWnd *pWnd)
{
	ASSERT(pWnd != NULL);
	if (pWnd == NULL) return;

	m_listRight2.DeleteAllItems();

	CString strPath = _T("");
	CTime timeStart, timeEnd;

	CFormLeft2 * pFormLeft = (CFormLeft2 *)pWnd;
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
		CString strFileName = timeStart.Format(_T("%Y%m%d"));
		strFilePath.Format(_T("%s%s_%s%s"), strPath, IDS_SECTION_HARDWAREINFO, strFileName, LOG_EXT);

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
				
				map<int, CString> hardInfo;
				while((iPos = strLine.Find('|', iStart)) != -1)
				{
					hardInfo[iIndex++] = strLine.Mid(iStart, iPos-iStart).Trim();
					iStart = iPos+1;
				}

				//ZZhenFei 2008.06.18  11.01
				//if (iIndex >= 10)
				if (iIndex >= 11)
				{
					strTemp.Format(_T("%d"), nRow + 1);
					int nItemInserted = m_listRight2.InsertItem(nRow, strTemp);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[0], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[1], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[4], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[6], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[3], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[7], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[8], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[2], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[5], 0, 0, 0, 0);
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[9], 0, 0, 0, 0);
					
					//ZZhenFei 2008.06.18  11.01
					m_listRight2.SetItem(nItemInserted, nCol++, LVIF_TEXT, hardInfo[10], 0, 0, 0, 0);

					nRow++;
					nCol = 1;
				}
				
			}

			sf.Close();
		}


		timeStart += ts;
		
	}while (timeStart < timeEnd);

	m_listRight2.SetOneselfItemData();
}
