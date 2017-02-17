// ColorGrid.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "ColorGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorGrid

CColorGrid::CColorGrid()
{
	m_hCursor = NULL;
}

CColorGrid::~CColorGrid()
{
}


BEGIN_MESSAGE_MAP(CColorGrid, CButton)
	//{{AFX_MSG_MAP(CColorGrid)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorGrid message handlers

void CColorGrid::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CBrush brush(m_crColor);
	
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
		DrawBorder(pDC);
	}
}

void CColorGrid::SetColor(COLORREF crColor)
{
	m_crColor = crColor;
	Invalidate();
}

void CColorGrid::DrawBorder(CDC * pDC)
{
	CPen pen1(PS_SOLID,1,::GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen pen2(PS_SOLID,1,::GetSysColor(COLOR_BTNSHADOW));
	CRect rect;
	GetWindowRect(&rect);
	CPen *pOldPen=pDC->SelectObject(&pen1);
	pDC->MoveTo(rect.Width()-1,0);
	pDC->LineTo(rect.Width()-1,rect.Height()-1);
	pDC->LineTo(0,rect.Height()-1);
	pDC->SelectObject(&pen2);
	pDC->LineTo(0,0);
	pDC->LineTo(rect.Width()-1,0);
	pDC->SelectObject(pOldPen);
}


BOOL CColorGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CColorGrid::SetCursorHandle(HCURSOR hCursor) 
{
	m_hCursor = hCursor;
}

