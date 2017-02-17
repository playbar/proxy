// ToolTipCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrlEx

CToolTipCtrlEx::CToolTipCtrlEx()
{
}

CToolTipCtrlEx::~CToolTipCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CToolTipCtrlEx, CToolTipCtrl)
	//{{AFX_MSG_MAP(CToolTipCtrlEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrlEx message handlers

BOOL CToolTipCtrlEx::AddWindow(CWnd* pWnd, LPCSTR pszTooltip)
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS|TTF_IDISHWND;
	ti.hwnd = pWnd->GetParent()->GetSafeHwnd();
	ti.uId = (UINT)pWnd->GetSafeHwnd();;
	ti.hinst = AfxGetInstanceHandle();
	ti.lpszText = (LPSTR)pszTooltip;

	return (BOOL)SendMessage(TTM_ADDTOOL,0, (LPARAM)&ti);
}

BOOL CToolTipCtrlEx::AddRectangle(CWnd* pWnd, LPCSTR pszTooltip, LPCRECT pRect, UINT uIDTool)
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = pWnd->GetSafeHwnd();
	ti.uId = uIDTool;
	ti.hinst = AfxGetInstanceHandle();
	ti.lpszText = (LPSTR)pszTooltip;
	::CopyRect(&ti.rect, pRect);

	return (BOOL)SendMessage(TTM_ADDTOOL,0, (LPARAM)&ti);
}
