// PaletteWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "math.h"
#include "DibView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteWnd

CPaletteWnd::CPaletteWnd(LPCSTR lpstrTitle, CWnd* pParentWnd)
{
	Create(NULL, lpstrTitle, WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, 
		   CRect(0,0,160,160), pParentWnd, 0x1208);
}

CPaletteWnd::~CPaletteWnd()
{
}


BEGIN_MESSAGE_MAP(CPaletteWnd, CWnd)
	//{{AFX_MSG_MAP(CPaletteWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaletteWnd message handlers

void CPaletteWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// parent window
	CDibView* pParent = (CDibView *)GetParent();

	// if no palette, return
	if (pParent->m_pDib->m_pPalette == NULL)
		return;

	// window size
	CRect rcWnd, rc;
	GetWindowRect(&rcWnd);
	GetClientRect(&rc);
	int nWidth = rc.Width();
	int nHeight = rc.Height();

	//DisplayPalette(dc.GetSafeHdc(), &rc, pParent->m_pDib->m_hPalette);
	int nEntries;
	PALETTEENTRY pe[256];
	nEntries = pParent->m_pDib->m_pPalette->GetPaletteEntries(0, 256, pe);

	int nSqr = (int)sqrt((double)nEntries);

	int nWidthBox = rc.Width()/nSqr;
	int nHeightBox = rc.Height()/nSqr;
	rc.right = rc.left + nWidthBox*nSqr;
	rc.bottom = rc.top + nHeightBox*nSqr;

	CPalette* pOldPal = dc.SelectPalette(pParent->m_pDib->m_pPalette, FALSE);
	dc.RealizePalette();

	int x, y;
	for (int i=0; i<nEntries; ++i)
	{
		x = i%nSqr;
		y = i/nSqr;
		CRect rcBox(rc.left + x*nWidthBox, 
				    rc.top + y*nHeightBox, 
				    rc.left + (x+1)*nWidthBox, 
				    rc.top + (y+1) *nHeightBox);
		CString strColor;
		strColor.Format("RGB(%d,%d,%d)", pe[i].peRed, pe[i].peGreen, pe[i].peBlue);
		m_ToolTip.AddRectangle(this, strColor, rcBox, i);

		CPen pen(PS_SOLID, 1, RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue));
		CPen* pOldPen = dc.SelectObject(&pen);
		CBrush brush;
		brush.CreateSolidBrush(RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue));
		CBrush* pOldBrush = dc.SelectObject(&brush);
		dc.Rectangle(&rcBox);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	}
	
	dc.SelectPalette(pOldPal, FALSE);

	int nDeltaX = rc.Width()-nWidth;
	int nDeltaY = rc.Height()-nHeight;
	if (nDeltaX || nDeltaY)
		SetWindowPos(NULL,0,0,
					 rcWnd.Width()+nDeltaX,
					 rcWnd.Height()+nDeltaY,
					 SWP_NOMOVE|SWP_NOZORDER);

	// Do not call CWnd::OnPaint() for painting messages
}

int CPaletteWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// no palette, not create this window
	CDibView* pParent = (CDibView *)GetParent();
	if (! pParent->m_pDib->m_pPalette)
		return -1;

	// create color value tip
	m_ToolTip.Create(this);
	
	// change window ex-style to toolwindow
	ShowWindow(SW_HIDE);
	long lExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	lExStyle &= ~WS_EX_APPWINDOW;	// get rid of AppWindow ex-style
	lExStyle |= WS_EX_TOOLWINDOW;	// add ToolWindow ex-style
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, lExStyle);
	ShowWindow(SW_SHOWNA);

	// success return
	return 0;
}


void CPaletteWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDibView* pParent = (CDibView *)GetParent();
	int nEntries;
	PALETTEENTRY pe[256];
	nEntries = pParent->m_pDib->m_pPalette->GetPaletteEntries(0, 256, pe);
	int nSqr = (int)sqrt((double)nEntries);
	CRect rc;
	GetClientRect(&rc);
	int nW = rc.Width()/nSqr;
	int nH = rc.Height()/nSqr;
	int x = point.x/nW;
	int y = point.y/nH;
	int i = y*nSqr + x;
	m_crSelected = RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue);

	CWnd::OnLButtonDblClk(nFlags, point);
}
