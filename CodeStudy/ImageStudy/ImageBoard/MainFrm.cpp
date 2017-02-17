// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ImageBoard.h"

#include "ImageBoardDoc.h"
#include "ImageBoardView.h"
#include "MainFrm.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_COMMAND(IDM_PAINTTOOL, OnPainttool)
	ON_UPDATE_COMMAND_UI(IDM_PAINTTOOL, OnUpdatePainttool)
	ON_COMMAND(IDM_PAINTPARAM, OnPaintparam)
	ON_UPDATE_COMMAND_UI(IDM_PAINTPARAM, OnUpdatePaintparam)
	ON_COMMAND(IDM_TEXTTOOL, OnTexttool)
	ON_UPDATE_COMMAND_UI(IDM_TEXTTOOL, OnUpdateTexttool)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT BASED_CODE painttool[] =
{
	// same order as in the bitmap 'painttool.bmp'
	IDM_SELECT,
	IDM_POINT,
	IDM_LINE,
	IDM_FREELINE,
	IDM_RECT_H,
	IDM_RECT_F,
	IDM_ROUNDRECT_H,
	IDM_ROUNDRECT_F,
	IDM_ELLIPSE_H,
	IDM_ELLIPSE_F,
	IDM_FILL,
	IDM_TEXT
};

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		// length unit
	ID_SEPARATOR,
		// cursor position
	ID_SEPARATOR,
		// image size
	ID_SEPARATOR,
		// bit count
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	EnableDocking(CBRS_ALIGN_ANY);
	
	// initialize m_wndToolBar.....
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetWindowText(_T("工具栏"));
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	// initialize m_wndStatusBar.....
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// add panes
    m_wndStatusBar.SetPaneInfo( PANE_LENGTH_UNIT, ID_SEPARATOR, SBPS_NORMAL, 25);
    m_wndStatusBar.SetPaneInfo( PANE_CURSOR_POSITION, ID_SEPARATOR, SBPS_NORMAL, 72);
    m_wndStatusBar.SetPaneInfo( PANE_IMAGE_SIZE, ID_SEPARATOR, SBPS_NORMAL, 72);
    m_wndStatusBar.SetPaneInfo( PANE_BIT_COUNT, ID_SEPARATOR, SBPS_NORMAL, 28);

	// initialize m_wndPaintToolBar.....
	if (!m_wndPaintToolBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_SIZE_FIXED|CBRS_LEFT|CBRS_TOOLTIPS, 0x1208) ||
		!m_wndPaintToolBar.LoadToolBar(IDR_PAINTTOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;       // fail to create
	}

	m_wndPaintToolBar.SetWindowText(_T("工具箱"));
	m_wndPaintToolBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndPaintToolBar);
	m_wndPaintToolBar.SetColumns(2);

	// Initialize m_wndPaintParamBar.....
	if (!m_wndPaintParamBar.Create(this, IDD_PAINTPARAM, CBRS_TOOLTIPS|CBRS_RIGHT|CBRS_FLYBY|CBRS_HIDE_INPLACE,0x1209))
	{
		TRACE0("Failed to create dialogbar m_wndPaintParamBar\n");
		return -1;		// fail to create
	}

	m_wndPaintParamBar.SetWindowText(_T("设置盒"));
	m_wndPaintParamBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndPaintParamBar);

	// initialize text toolbar...
	if (!m_wndTextToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0, 0, 0, 0), 0x1210) ||
		!m_wndTextToolBar.LoadToolBar(IDR_TEXTTOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndTextToolBar.SetWindowText(_T("文字工具栏"));
	m_wndTextToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndTextToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CMDIFrameWnd::OnPaletteChanged(pFocusWnd);
	
	// always realize the palette for the active view
	CMDIChildWnd* pMDIChildWnd = MDIGetActive();
	if (pMDIChildWnd == NULL)
		return; // no active MDI child frame
	CView* pView = pMDIChildWnd->GetActiveView();
	ASSERT(pView != NULL);

	// notify all child windows that the palette has changed
	SendMessageToDescendants(WM_REALIZEPAL, (WPARAM)pView->m_hWnd);
}

BOOL CMainFrame::OnQueryNewPalette() 
{
	// always realize the palette for the active view
	CMDIChildWnd* pMDIChildWnd = MDIGetActive();
	if (pMDIChildWnd == NULL)
		return FALSE; // no active MDI child frame (no new palette)
	CView* pView = pMDIChildWnd->GetActiveView();
	ASSERT(pView != NULL);

	// just notify the target view
	pView->SendMessage(WM_REALIZEPAL, (WPARAM)pView->m_hWnd);
	return TRUE;
}


void CMainFrame::OnPainttool() 
{
	BOOL bVisible = ((m_wndPaintToolBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndPaintToolBar, !bVisible, FALSE);
	RecalcLayout();

	GetMenu()->CheckMenuItem(IDM_PAINTTOOL, 
		(bVisible ? MF_UNCHECKED : MF_CHECKED)|MF_BYCOMMAND);
}

void CMainFrame::OnUpdatePainttool(CCmdUI* pCmdUI) 
{
	BOOL bVisible = ((m_wndPaintToolBar.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnPaintparam() 
{
	BOOL bVisible = ((m_wndPaintParamBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndPaintParamBar, !bVisible, FALSE);
	RecalcLayout();

	GetMenu()->CheckMenuItem(IDM_PAINTPARAM, 
		(bVisible ? MF_UNCHECKED : MF_CHECKED)|MF_BYCOMMAND);
}

void CMainFrame::OnUpdatePaintparam(CCmdUI* pCmdUI) 
{
	BOOL bVisible = ((m_wndPaintParamBar.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}


void CMainFrame::OnTexttool() 
{
	BOOL bVisible = ((m_wndTextToolBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndTextToolBar, !bVisible, FALSE);
	RecalcLayout();

	GetMenu()->CheckMenuItem(IDM_TEXTTOOL, 
		(bVisible ? MF_UNCHECKED : MF_CHECKED)|MF_BYCOMMAND);
}

void CMainFrame::OnUpdateTexttool(CCmdUI* pCmdUI) 
{
	BOOL bVisible = ((m_wndTextToolBar.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

