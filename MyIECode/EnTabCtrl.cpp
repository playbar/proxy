//_ **********************************************************
//_ 
//_ Name: EnTabCtrl.cpp 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ Copyright (c) 1998 Brilliant Digital Entertainment Inc. 
//_ 
//_ **********************************************************

// EnTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EnTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnTabCtrl

DWORD CEnTabCtrl::s_dwCustomLook = 0;

enum { PADDING = 3, EDGE = 20};

extern BOOL g_bShowDot;
//////////////////////////////////////////////////////////////////////////////
// helpers
CEnTabCtrl::CEnTabCtrl()
{
}

CEnTabCtrl::~CEnTabCtrl()
{
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CEnTabCtrl message handlers

void CEnTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);

	CRect rItem(lpdis->rcItem);

	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	if (m_nTabStyle==0 && !bSelected)
	{
		if (bBottom)
		{
			rItem.left -= 1;
			rItem.top -= 2;
		}
		else
		{
			rItem.left -= 1;
			rItem.bottom += 2;
		}
	}
	else if (m_nTabStyle==1)
	{
		if (bBottom)
		{
			rItem.bottom += 2;
			if (bSelected)
				rItem.top -= 1;
		}
		else
		{
			rItem.top -= 2;
			if (bSelected)
				rItem.bottom += 1;
		}
	}

	// blend from back color to COLOR_3DFACE if 16 bit mode or better
	COLORREF crFrom = GetTabColor(bSelected);
	
	// simple solid fill
	pDC->FillSolidRect(rItem, crFrom);

	// text & icon
	rItem.left += PADDING;
	rItem.top += PADDING + (bSelected ? 1 : 0);

	pDC->SetBkMode(TRANSPARENT);

	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();

	// icon
	if (hilTabs && tci.iImage>=0)
	{
		ImageList_Draw(hilTabs, tci.iImage, *pDC, rItem.left, rItem.top-1, ILD_TRANSPARENT);
		rItem.left += 11 + PADDING;//16+
	}

	// text
	rItem.right -= PADDING;
	//FormatText(sTemp, pDC, rItem.Width());

	if (bSelected)
		pDC->SetTextColor(m_crSelFontColor);
	else
		pDC->SetTextColor(m_crUnSelFontColor);
	pDC->SelectObject(&m_UnselFont);
	if (g_bShowDot)
		pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_END_ELLIPSIS );
	else
		pDC->DrawText(sTemp, rItem, DT_NOPREFIX  );
}

void CEnTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	ASSERT (s_dwCustomLook & ETC_FLAT);

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);

	CRect rItem(lpdis->rcItem);
	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	if (bBottom)
	{
		rItem.bottom += 2;
	}
	else
	{
		rItem.top -= 2;
	}
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor(bSelected);

	if (bSelected)
	{
		BOOL bBottom = GetStyle() & TCS_BOTTOM;
		if (bBottom)
			rItem.top -= 1;
		else
			rItem.bottom += 1;

		// edges
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), 0xFFFFFF);
		if (bBottom)
			pDC->FillSolidRect(CRect(rItem.left, rItem.bottom, rItem.right, rItem.bottom - 1), 0x433F3E);
		else
			pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), 0xFFFFFF);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), 0x433F3E);
	}
	else // draw simple dividers
	{
		pDC->FillSolidRect(CRect(rItem.right, rItem.top+4, rItem.right+1, rItem.bottom-4), m_crSepColor);
	}
}

void CEnTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	if (bBottom)
	{
		rItem.top = rItem.bottom-1;
		pDC->FillSolidRect(rItem, 0);
	}
	else
	{
		rItem.bottom = rItem.top+1 ;
		pDC->FillSolidRect(rItem, 0xFFFFFF);
	}
}

COLORREF CEnTabCtrl::GetTabColor(BOOL bSelected)
{
	if (!bSelected)
		return m_crUnSelColor;
	else
		return ::GetSysColor(COLOR_3DFACE);
}

void CEnTabCtrl::EnableCustomLook(BOOL bEnable, DWORD dwStyle)
{
	if (!bEnable)
		dwStyle = 0;

	s_dwCustomLook = dwStyle;
}

void CEnTabCtrl::PreSubclassWindow() 
{
	CBaseTabCtrl::PreSubclassWindow();

	if (s_dwCustomLook)
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

void CEnTabCtrl::SetFonts(int nSelWeight,   BOOL bSelItalic,   BOOL bSelUnderline,
						  int nUnselWeight, BOOL bUnselItalic, BOOL bUnselUnderline)
{
	// Free any memory currently used by the fonts.
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	LOGFONT lFont;
	ZeroMemory ((PVOID) &lFont,sizeof (LOGFONT));
	NONCLIENTMETRICS nm;
	nm.cbSize = sizeof (NONCLIENTMETRICS);
	VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
	lFont =  nm.lfMenuFont;

	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bSelItalic;
	lFont.lfUnderline = bSelUnderline;
	m_SelFont.CreateFontIndirect(&lFont);

	// Create the "Unselected" font
	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bUnselItalic;
	lFont.lfUnderline = bUnselUnderline;
	m_UnselFont.CreateFontIndirect(&lFont);

	SetFont( (nSelWeight > nUnselWeight)? &m_SelFont : &m_UnselFont);
}
