// FloatDibWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "ImageBoardDoc.h"
#include "ImageBoardView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatDibWnd

CFloatDibWnd::CFloatDibWnd(HDIB hDib, CRect rc, CWnd* pParentWnd)
{
	m_hDibFloat = hDib;
	m_bFirst = TRUE;
	m_hCursorFloat = AfxGetApp()->LoadCursor(IDC_CURSORMOVE);
	Create(AfxRegisterWndClass(NULL, m_hCursorFloat), 
		   "", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, 
		   rc, pParentWnd, 0x6612);
}

CFloatDibWnd::~CFloatDibWnd()
{
}


BEGIN_MESSAGE_MAP(CFloatDibWnd, CWnd)
	//{{AFX_MSG_MAP(CFloatDibWnd)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFloatDibWnd message handlers

void CFloatDibWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	CImageBoardView* pParent = (CImageBoardView *)GetParent();
	
	CRect rcDraw(1, 1, rc.Width()-1, rc.Height()-1);
	PaintDIB(dc.GetSafeHdc(), &rcDraw, m_hDibFloat, &rcDraw, 
		(HPALETTE)pParent->m_pDib->m_pPalette->GetSafeHandle(), SRCCOPY);
	
	CPen pen1(PS_SOLID, 1, RGB(255,255,255));
	CPen pen2(PS_DOT, 1, RGB(0,0,0));
	CPen* pOldPen = dc.SelectObject(&pen1);
	//int nOldRop = dc.SetROP2(R2_NOTMERGEPEN);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right-1, rc.top);
	dc.LineTo(rc.right-1, rc.bottom-1);
	dc.LineTo(rc.left, rc.bottom-1);
	dc.LineTo(rc.left, rc.top);

	dc.SelectObject(&pen2);
	//int nOldRop = dc.SetROP2(R2_NOTMERGEPEN);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right-1, rc.top);
	dc.LineTo(rc.right-1, rc.bottom-1);
	dc.LineTo(rc.left, rc.bottom-1);
	dc.LineTo(rc.left, rc.top);
	dc.SelectObject(pOldPen);
	//dc.SetROP2(nOldRop);
	// Do not call CWnd::OnPaint() for painting messages
}

UINT CFloatDibWnd::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}

BOOL CFloatDibWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(m_hCursorFloat);	
	
	return TRUE;//CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CFloatDibWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	DestroyDIB(m_hDibFloat);
}

void CFloatDibWnd::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);

	// eat the first WM_MOVE when creating
	if (m_bFirst)
	{
		m_bFirst = FALSE;
		return;
	}

	CImageBoardView* pParent = (CImageBoardView *)GetParent();
	pParent->CutSelectedRect();

	UpdateWindow();
}

