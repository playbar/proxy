// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "IGASSee.h"

#include "MainFrm.h"
#include "FormLeft.h"
#include "FormRight.h"
#include "FormLeft1.h"
#include "FormRight1.h"
#include "FormLeft2.h"
#include "FormRight2.h"
#include "FormLeft3.h"
#include "FormRight3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("未能创建工具栏\n");
	//	return -1;      // 未能创建
	//}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	// TODO: 如果不需要工具栏可停靠，则删除这三行
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	this->SetMenu(NULL);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->SetIcon(m_hIcon, TRUE);
	this->SetIcon(m_hIcon, FALSE);
	
	this->SetWindowText(_T("IGA服务查看工具"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	TVisualObject *pTab = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd), TVisualObject::TOS_TABTOP);
	TVisualObject *pTabViewSplite = new TVisualObject(2,_T("游戏信息"),1,2,pContext);
	TVisualObject *pTabViewSplite1 = new TVisualObject(3,_T("观注度"),1,2,pContext);
	TVisualObject *pTabViewSplite2 = new TVisualObject(4,_T("硬件信息查看"),1,2,pContext);
	TVisualObject *pTabViewSplite3 = new TVisualObject(5,_T("SDK信息查看"),1,2,pContext);

	TVisualObject *pPaneLeft = new TVisualObject(21,0,0,pContext,RUNTIME_CLASS(CFormLeft),CSize(300,0));
	TVisualObject *pPaneRight = new TVisualObject(22,0,1,pContext,RUNTIME_CLASS(CFormRight), CSize(0,0));

	TVisualObject *pPaneLeft1 = new TVisualObject(23,0,0,pContext,RUNTIME_CLASS(CFormLeft1),CSize(300,0));
	TVisualObject *pPaneRight1 = new TVisualObject(24,0,1,pContext,RUNTIME_CLASS(CFormRight1), CSize(0,0));

	TVisualObject *pPaneLeft2 = new TVisualObject(25,0,0,pContext,RUNTIME_CLASS(CFormLeft2),CSize(300,0));
	TVisualObject *pPaneRight2 = new TVisualObject(26,0,1,pContext,RUNTIME_CLASS(CFormRight2), CSize(0,0));

	TVisualObject *pPaneLeft3 = new TVisualObject(27,0,0,pContext,RUNTIME_CLASS(CFormLeft3),CSize(300,0));
	TVisualObject *pPaneRight3 = new TVisualObject(28,0,1,pContext,RUNTIME_CLASS(CFormRight3), CSize(0,0));

	m_Framework.Add(pTab);

	m_Framework.Add(pTab, pTabViewSplite);
	m_Framework.Add(pTabViewSplite, pPaneLeft);
	m_Framework.Add(pTabViewSplite, pPaneRight);

	m_Framework.Add(pTab, pTabViewSplite1);
	m_Framework.Add(pTabViewSplite1, pPaneLeft1);
	m_Framework.Add(pTabViewSplite1, pPaneRight1);

	m_Framework.Add(pTab, pTabViewSplite2);
	m_Framework.Add(pTabViewSplite2, pPaneLeft2);
	m_Framework.Add(pTabViewSplite2, pPaneRight2);

	m_Framework.Add(pTab, pTabViewSplite3);
	m_Framework.Add(pTabViewSplite3, pPaneLeft3);
	m_Framework.Add(pTabViewSplite3, pPaneRight3);


	m_Framework.Create(this);

	// 创建INI文件
	CreateConfigFile();

	return TRUE;

	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_Framework.Destroy();
	
}

// 创建各个窗口时，为每个窗口指定相应的ID， iOffset是指定窗口的偏移ID
CWnd * CMainFrame::GetOffsetObject(CWnd * pWnd, int iOffset)
{
	if ((pWnd == NULL) || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return NULL;
	}
	
	DWORD dwId = m_Framework.GetObject(pWnd);

	dwId += iOffset;
	if ((dwId < 1) || (dwId > 100))
	{
		return NULL;
	}

	CWnd * pResultWnd = m_Framework.GetObject(dwId);
	
	return pResultWnd;
}
