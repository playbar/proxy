// FormRight1.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormRight1.h"
#include ".\formright1.h"
#include "FormLeft1.h"
#include "Winsock2.h"



// CFormRight1

IMPLEMENT_DYNCREATE(CFormRight1, CFormView)

CFormRight1::CFormRight1()
	: CFormView(CFormRight1::IDD)
{
}

CFormRight1::~CFormRight1()
{
}

void CFormRight1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RIGHT1, m_listRight1);
}

BEGIN_MESSAGE_MAP(CFormRight1, CFormView)
//	ON_WM_SHOWWINDOW()
ON_WM_SIZE()
END_MESSAGE_MAP()


// CFormRight1 诊断

#ifdef _DEBUG
void CFormRight1::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormRight1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CFormRight1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_listRight1.SetExtendedStyle(
    LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );

	int nColCount = 0;
	m_listRight1.InsertColumn(nColCount++, _T("序号"), LVCFMT_LEFT, 60);
	m_listRight1.InsertColumn(nColCount++, _T("游戏ID"), LVCFMT_LEFT, 60);
	m_listRight1.InsertColumn(nColCount++, _T("MAC地址"), LVCFMT_LEFT, 80);
	m_listRight1.InsertColumn(nColCount++, _T("广告数量"), LVCFMT_LEFT, 60);
	m_listRight1.InsertColumn(nColCount++, _T("区号"), LVCFMT_LEFT, 60);
	m_listRight1.InsertColumn(nColCount++, _T("地址"), LVCFMT_LEFT, 100);
	m_listRight1.InsertColumn(nColCount++, _T("广告ID"), LVCFMT_LEFT, 60);
	m_listRight1.InsertColumn(nColCount++, _T("下载次数"), LVCFMT_LEFT, 80);
	m_listRight1.InsertColumn(nColCount++, _T("加载次数"), LVCFMT_LEFT, 80);
	m_listRight1.InsertColumn(nColCount++, _T("总曝光次数"), LVCFMT_LEFT, 80);
	m_listRight1.InsertColumn(nColCount++, _T("最清晰曝光次数"), LVCFMT_LEFT, 80);
	m_listRight1.InsertColumn(nColCount++, _T("最清晰整播次数"), LVCFMT_LEFT, 80);
	m_listRight1.InsertColumn(nColCount++, _T("最清晰曝光时长"), LVCFMT_LEFT, 120);
	m_listRight1.InsertColumn(nColCount++, _T("清晰曝光次数"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("清晰整播次数"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("清晰曝光时长"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("比较清晰曝光次数"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("比较清晰整播次数"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("比较清晰曝光时长"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("模糊曝光次数"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("模糊整播次数"), LVCFMT_LEFT, 50);
	m_listRight1.InsertColumn(nColCount++, _T("模糊曝光时长"), LVCFMT_LEFT, 50);

	CRect clientRect;
	this->GetClientRect(&clientRect);
	if (::IsWindow(m_listRight1.GetSafeHwnd()))
	{
		m_listRight1.MoveWindow(&clientRect);
	}
}

void CFormRight1::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (::IsWindow(m_listRight1.GetSafeHwnd()))
	{
		m_listRight1.MoveWindow(0, 0, cx, cy);
	}
}


void CFormRight1::UpdateList(CWnd * pWnd)
{
	ASSERT(pWnd != NULL);
	if (pWnd == NULL) return;

	m_listRight1.DeleteAllItems();

	CString strPath = _T("");
	CTime timeStart, timeEnd;

	CFormLeft1 * pFormLeft = (CFormLeft1 *)pWnd;
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
		FILE *pFile = NULL;
		CString strFilePath = _T("");
		CString strFileName = timeStart.Format(_T("%Y%m%d"));
		strFilePath.Format(_T("%s%s%s"), strPath, strFileName, FOCUSINFO_EXT);

		if ((pFile = fopen((LPCTSTR)strFilePath,  "rb")) != NULL)
		{

			int len = 0;
			tagPacketHeader packetHeader;

			typedef	FOCUSREQUEST_T<tagRequestHeader>	FOCUSREQUEST;
			FOCUSREQUEST * pRequestHeader = NULL;
			
			while(1)
			{
				memset(&packetHeader, 0, sizeof(tagPacketHeader));
			
				len = sizeof(tagPacketHeader);
				// 读包头
				if (fread(&packetHeader, 1, len, pFile) != len) break;

				len = packetHeader.dwLength - len;
				if (len < sizeof(FOCUSREQUEST)) 
				{
					// 包的数据错误
					break;
				}
				
				char * buffer = new char [len + 1];
				ASSERT(buffer != NULL);
				
				memset(buffer, 0, len + 1 );
				// 读包体
				if (fread(buffer, 1, len, pFile) != len)
				{
					if (buffer != NULL)
					{
						delete []buffer;
						buffer = NULL;
					}

					break;
				}
				
				pRequestHeader = (FOCUSREQUEST *)buffer;
				DWORD dwAdCount = pRequestHeader->dwAdCount;

				if (dwAdCount > 0)
				{
					DWORD dwTargetSize = dwAdCount * sizeof(tagFocus)*100;
					char * pp = new char [dwTargetSize];
					ASSERT(pp != NULL);
					memset(pp, 0, dwTargetSize);

					DWORD dwSourceSize = len - sizeof(FOCUSREQUEST);
					if (packetHeader.byFlags == 1) // 表示focus数据经过了压缩处理
					{
						// 解压到pp
						int ret = uncompress((Bytef*)pp, (uLongf*)&dwTargetSize, (Bytef*)(buffer + sizeof(FOCUSREQUEST)), (uLongf)dwSourceSize);
						if (dwTargetSize != dwAdCount * sizeof(tagFocus)) 
						{
							if (pp != NULL)
							{
								delete []pp;
								pp = NULL;
							}


							if (buffer != NULL)
							{
								delete []buffer;
								buffer = NULL;
							}

							continue;
						}
					}
					else
					{
						memcpy(pp, buffer + sizeof(FOCUSREQUEST), dwSourceSize);
					}

					CString strTemp = _T("");
					
					int nCol = 1;
					LPFOCUS pFocus = (LPFOCUS)pp;
					for(int i=0; i<(int)dwAdCount; i++)
					{
						// 插入数据到列表
						
						strTemp.Format(_T("%d"), nRow+1);
						int nItemInserted = m_listRight1.InsertItem(nRow, strTemp);

						strTemp.Format(_T("%d"), pRequestHeader->dwGameID);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, pRequestHeader->szMacID, 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pRequestHeader->dwAdCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pRequestHeader->dwAreaID);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						in_addr ip;
						ip.S_un.S_addr = pRequestHeader->dwAddress;
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, ::inet_ntoa(ip), 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->dwAdID);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->dwDownloadCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->dwLoadCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->dwClickCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->pi[0].dwPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						strTemp.Format(_T("%d"), pFocus->pi[0].dwFullPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						CTimeSpan ts2(pFocus->pi[0].dwPlayTime/1000);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, ts2.Format(_T("%D %H:%M:%S")), 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->pi[1].dwPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						strTemp.Format(_T("%d"), pFocus->pi[1].dwFullPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						CTimeSpan ts3(pFocus->pi[1].dwPlayTime/1000);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, ts3.Format(_T("%D %H:%M:%S")), 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->pi[2].dwPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						strTemp.Format(_T("%d"), pFocus->pi[2].dwFullPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						CTimeSpan ts4(pFocus->pi[2].dwPlayTime/1000);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, ts4.Format(_T("%D %H:%M:%S")), 0, 0, 0, 0);

						strTemp.Format(_T("%d"), pFocus->pi[3].dwPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						strTemp.Format(_T("%d"), pFocus->pi[3].dwFullPlayCount);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
						CTimeSpan ts5(pFocus->pi[3].dwPlayTime/1000);
						m_listRight1.SetItem(nItemInserted, nCol++, LVIF_TEXT, ts5.Format(_T("%D %H:%M:%S")), 0, 0, 0, 0);

						pFocus++;
						nRow++;
						nCol = 1;
						
					}

					if (pp != NULL)
					{
						delete []pp;
						pp = NULL;
					}

				}

				if (buffer != NULL)
				{
					delete []buffer;
					buffer = NULL;
				}


			}// endwhile(0)

			fclose( pFile );
		}

		timeStart += ts;
		
	}while (timeStart < timeEnd);

	m_listRight1.SetOneselfItemData();
}
