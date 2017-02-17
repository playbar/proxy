// PaintParambar.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "ImageBoardDoc.h"
#include "ImageBoardView.h"
#include "PaintParambar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaintParambar

CPaintParambar::CPaintParambar()
{
	m_hPalette = NULL;
	m_nSelectColorMode = PP_PEN_COLOR;
}

CPaintParambar::~CPaintParambar()
{
}


BEGIN_MESSAGE_MAP(CPaintParambar, CDialogBar)
	//{{AFX_MSG_MAP(CPaintParambar)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_PEN_STYLE, OnSelchangePenstyle)
	ON_LBN_SELCHANGE(IDC_PEN_WIDTH, OnSelchangePenwidth)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaintParambar message handlers

int CPaintParambar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hCursorSelectColor = AfxGetApp()->LoadCursor(IDC_CURSORPICKER);
	
	m_listPenWidth.Create(LBS_NOINTEGRALHEIGHT|LBS_OWNERDRAWVARIABLE|LBS_NOTIFY,
						  CRect(8,8,29,58),
						  this,
						  IDC_PEN_WIDTH);
	m_listPenWidth.AddLPItem(1);
	m_listPenWidth.AddLPItem(2);
	m_listPenWidth.AddLPItem(3);
	m_listPenWidth.AddLPItem(4);
	m_listPenWidth.AddLPItem(5);
	m_listPenWidth.ShowWindow(SW_SHOWNORMAL);
	m_listPenWidth.SetCurSel(0);
	
	m_listPenStyle.m_nParamIndex = PEN_STYLE;
	m_listPenStyle.Create(LBS_NOINTEGRALHEIGHT|LBS_OWNERDRAWVARIABLE|LBS_NOTIFY,
						  CRect(32,8,75,58),
						  this,
						  IDC_PEN_STYLE);
	m_listPenStyle.AddLPItem(PS_SOLID);
	m_listPenStyle.AddLPItem(PS_DASH);
	m_listPenStyle.AddLPItem(PS_DOT);
	m_listPenStyle.AddLPItem(PS_DASHDOT);
	m_listPenStyle.AddLPItem(PS_DASHDOTDOT);
	m_listPenStyle.ShowWindow(SW_SHOWNORMAL);
	m_listPenStyle.SetCurSel(0);

	m_rcPenColor = CRect(8,61,40,89);
	m_btPenColor.Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON, 
				CRect(9,62,39,88), this, IDC_PEN_COLOR);
	m_btPenColor.SetColor(RGB(0,0,0));
	m_btPenColor.ShowWindow(SW_SHOWNORMAL);

	m_rcFillColor = CRect(43,61,75,89);
	m_btFillColor.Create(NULL, BS_OWNERDRAW | BS_PUSHBUTTON, 
				CRect(44,62,74,88), this, IDC_FILL_COLOR);
	m_btFillColor.SetColor(RGB(0,0,0));
	m_btFillColor.ShowWindow(SW_SHOWNORMAL);

	for(int i=0; i<COLOR_NUMBER; i++)
	{
		m_btColor[i].Create(NULL, BS_OWNERDRAW | BS_PUSHBUTTON, 
				CRect(5+i%8*9,93+i/8*9,14+i%8*9,102+i/8*9), this, IDC_COLORTABLE_BASE+i);
		m_btColor[i].ShowWindow(SW_HIDE);
	}

	CRect rc;
	GetWindowRect(&rc);
	m_nWidth = rc.Width();
	m_nHeight = rc.Height();
	ShowColorGrid(m_hPalette);

	return 0;
}

void CPaintParambar::ShowPenColor(COLORREF crColor)
{
	m_btPenColor.SetColor(crColor);
}

void CPaintParambar::ShowFillColor(COLORREF crColor)
{
	m_btFillColor.SetColor(crColor);
}

void CPaintParambar::ShowPenWidth(int nWidth)
{
	m_listPenWidth.SetCurSel(nWidth-1);
}

void CPaintParambar::ShowPenStyle(int nStyle)
{
	m_listPenStyle.SetCurSel(nStyle-PS_SOLID);
}

void CPaintParambar::ShowColorGrid(HPALETTE hPalette)
{
	m_hPalette = hPalette;
	m_nColors = 0;
	int i;
	if (m_hPalette != NULL)
	{
		PALETTEENTRY pe[256];
		m_nColors = GetPaletteEntries(m_hPalette, 0, 256, pe);
		for(i=0; i<m_nColors; i++)
		{
			m_btColor[i].SetColor(RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue));
			m_btColor[i].ShowWindow(SW_SHOWNORMAL);
			m_btColor[i].SetCursorHandle(m_hCursorSelectColor);
		}
	}
	for (i=m_nColors; i<COLOR_NUMBER; ++i)
		if (IsWindow(m_btColor[i].m_hWnd))
			m_btColor[i].ShowWindow(SW_HIDE);

	m_nHeight = 102+m_nColors/8*9;
	if (m_nColors == 0)
		m_nHeight -= 6;
	else if (m_nColors == 2)
		m_nHeight += 8;

	SetWindowPos(&wndTop,0,0,m_nWidth,m_nHeight, SWP_NOMOVE);
}


CSize CPaintParambar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	return CSize(m_nWidth,m_nHeight);
}

CSize CPaintParambar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	return CSize(m_nWidth,m_nHeight);
}


CView* CPaintParambar::GetView() 
{
	// Get the active MDI child window.
	CMDIFrameWnd *pFrame =  (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	if (pFrame != NULL)
	{
		CMDIChildWnd *pChild = 
			 (CMDIChildWnd *) pFrame->GetActiveFrame();

		// Get the active view attached to the active MDI child// window.
		return (CImageBoardView *) pChild->GetActiveView();
	}

	return NULL;
}

void CPaintParambar::OnSelchangePenstyle() 
{
	// Get the active view attached to the active MDI child// window.
	CImageBoardView *pView = (CImageBoardView* )GetView();
	if (pView != NULL)
		pView->m_nPenStyle = m_listPenStyle.GetItemData(m_listPenStyle.GetCurSel());
}

void CPaintParambar::OnSelchangePenwidth() 
{
	// Get the active view attached to the active MDI child// window.
	CImageBoardView *pView = (CImageBoardView* )GetView();
	if (pView != NULL)
		pView->m_nPenWidth = m_listPenWidth.GetItemData(m_listPenWidth.GetCurSel());
}

void CPaintParambar::SetSelectColorMode(int nSelectColorMode)
{
	m_nSelectColorMode = nSelectColorMode;
	CRect rc;
	rc.UnionRect(&m_rcFillColor, &m_rcPenColor);
	InvalidateRect(&rc);
}

void CPaintParambar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen *pOldPen = dc.SelectObject(&pen);
	CRect rc;
	if (m_nSelectColorMode == PP_PEN_COLOR)
		dc.Rectangle(&m_rcPenColor);
	else if (m_nSelectColorMode == PP_FILL_COLOR)
		dc.Rectangle(&m_rcFillColor);

	dc.SelectObject(pOldPen);
	
	// Do not call CDialogBar::OnPaint() for painting messages
}
