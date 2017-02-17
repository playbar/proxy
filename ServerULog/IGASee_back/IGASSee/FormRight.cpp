// FormRight.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormRight.h"
#include "MainFrm.h"
#include "FormLeft.h"
#include ".\formright.h"


// CFormRight

IMPLEMENT_DYNCREATE(CFormRight, TVisualFormView)

CFormRight::CFormRight()
	: TVisualFormView(CFormRight::IDD)
{
}

CFormRight::~CFormRight()
{
}

void CFormRight::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RIGHT, m_listRight);
}

BEGIN_MESSAGE_MAP(CFormRight, TVisualFormView)
	ON_WM_SIZE()
	
END_MESSAGE_MAP()


// CFormRight 诊断

#ifdef _DEBUG
void CFormRight::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormRight::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormRight 消息处理程序

void CFormRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	m_listRight.SetExtendedStyle(
    LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );
	int nColCount = 0;
	m_listRight.InsertColumn(nColCount++, _T("序号"), LVCFMT_LEFT, 60);
	m_listRight.InsertColumn(nColCount++, _T("游戏ID"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("开始游戏时间"), LVCFMT_LEFT, 160);
	m_listRight.InsertColumn(nColCount++, _T("结束游戏时间"), LVCFMT_LEFT, 160);
	m_listRight.InsertColumn(nColCount++, _T("游戏运行时长"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("区号"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("MAC地址"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("IP地址"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("区域"), LVCFMT_LEFT, 100);

	CRect clientRect;
	this->GetClientRect(&clientRect);
	if (::IsWindow(m_listRight.GetSafeHwnd()))
	{
		m_listRight.MoveWindow(&clientRect);
	}

}

void CFormRight::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	if (::IsWindow(m_listRight.GetSafeHwnd()))
	{
		m_listRight.MoveWindow(0, 0, cx, cy);
	}
}

void CFormRight::UpdateList(CWnd *pWnd)
{
	ASSERT(pWnd != NULL);
	if (pWnd == NULL) return;

	m_listRight.DeleteAllItems();

	CString strPath = _T("");
	CTime timeStart, timeEnd;

	CFormLeft * pFormLeft = (CFormLeft *)pWnd;
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
		CLIENTPLAYINFO	cci;
		int len = sizeof(CLIENTPLAYINFO);
		memset(&cci, 0, len);
		FILE *pFile = NULL;
		CString strFilePath = _T("");
		CString strFileName = timeStart.Format(_T("%Y%m%d"));
		strFilePath.Format(_T("%s%s%s"), strPath, strFileName, PLAYINFO_EXT);

		CString strTemp = _T("");
		int nCol = 1;
		if ((pFile = fopen((LPCTSTR)strFilePath,  "rb")) != NULL)
		{
			while (fread(&cci, 1, len, pFile) == len)
			{
				strTemp.Format(_T("%d"), nRow + 1);
				int nItemInserted = m_listRight.InsertItem(LVIF_TEXT, nRow, strTemp, 0, 0, 0, 0);

				strTemp.Format(_T("%d"), cci.dwGameID);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
				CTime timeEnd(cci.dwCloseTime);
				CTimeSpan ts(cci.dwPlayTime/1000);
				CTime timeStart = timeEnd - ts;
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 0, 0, 0, 0);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")), 0, 0, 0, 0);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, ts.Format(_T("%D %H:%M:%S")), 0, 0, 0, 0);
				
				strTemp.Format(_T("%d"), cci.dwAreadID);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, cci.szMacID, 0, 0, 0, 0);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, cci.szAddress, 0, 0, 0, 0);
				strTemp.Format(_T("%d"), cci.dwChnID);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

				nRow++;
				nCol = 1;
				memset(&cci, 0, len);
			}

			fclose( pFile );
		}


		timeStart += ts;
		
	}while (timeStart < timeEnd);

	m_listRight.SetOneselfItemData();

}

