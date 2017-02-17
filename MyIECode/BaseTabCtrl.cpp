// BaseTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BaseTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl
#define TC_UNSELBK		GetSysColor(COLOR_BTNFACE)
#define TC_UNSELFONT	GetSysColor(COLOR_GRAYTEXT)
#define TC_SELFONT		GetSysColor(COLOR_BTNTEXT)
#define TC_SEP			GetSysColor(COLOR_GRAYTEXT)

CBaseTabCtrl::CBaseTabCtrl()
{
	m_nTabStyle = 1;
	m_crUnSelColor		= TC_UNSELBK;
	m_crUnSelFontColor	= TC_UNSELFONT;
	m_crSelFontColor	= TC_SELFONT;
	m_crSepColor		= TC_SEP;

	SetTabStyle(m_nTabStyle);
}

CBaseTabCtrl::~CBaseTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CBaseTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CBaseTabCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl message handlers

void CBaseTabCtrl::SetUnSelColor(COLORREF color)
{
	if (color==-1)
		m_crUnSelColor = TC_UNSELBK;
	else
		m_crUnSelColor = color;
}

void CBaseTabCtrl::SetSelFontColor(COLORREF crSelFontColor)
{
	if (crSelFontColor==-1)
		m_crSelFontColor = TC_SELFONT;
	else
		m_crSelFontColor = crSelFontColor;
}

void CBaseTabCtrl::SetUnSelFontColor(COLORREF crUnSelFontColor)
{
	if (crUnSelFontColor==-1)
		m_crUnSelFontColor = TC_UNSELFONT;
	else
		m_crUnSelFontColor = crUnSelFontColor;
}

void CBaseTabCtrl::SetSepColor(COLORREF crSepColor)
{
	if (crSepColor==-1)
		m_crSepColor = TC_SEP;
	else
		m_crSepColor = crSepColor;
}

void CBaseTabCtrl::SetTabStyle(int nType)
{
	m_nTabStyle = nType;
	if (!::IsWindow(m_hWnd))
		return;
	if (m_nTabStyle==2)
		ModifyStyle(TCS_OWNERDRAWFIXED, TCS_BUTTONS|TCS_FLATBUTTONS|TCS_RAGGEDRIGHT);
	else
		ModifyStyle(TCS_BUTTONS|TCS_FLATBUTTONS|TCS_RAGGEDRIGHT, TCS_OWNERDRAWFIXED);
}

void CBaseTabCtrl::SetTabPos(int bTopTab)
{
	if(bTopTab)
		ModifyStyle(TCS_BOTTOM,0);
	else
		ModifyStyle(0, TCS_BOTTOM);
}

void CBaseTabCtrl::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

void CBaseTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

void CBaseTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

COLORREF CBaseTabCtrl::GetTabColor(BOOL /*bSelected*/)
{
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CBaseTabCtrl::GetTabTextColor(BOOL /*bSelected*/)
{
	return ::GetSysColor(COLOR_WINDOWTEXT);
}

void CBaseTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();

	if (m_nTabStyle != 0) 
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

BOOL CBaseTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if (m_nTabStyle != 1 || GetItemCount()<1)
	{
		return CTabCtrl::OnEraseBkgnd(pDC);
		//return TRUE;
	}
	CRect rClient;
	COLORREF crBack;

	CTabCtrl::OnEraseBkgnd(pDC);

	// calc total tab width
	GetClientRect(rClient);
	crBack = (m_crUnSelColor == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crUnSelColor;
	CRect rTemp;
	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	if (bBottom)
	{
		rTemp = rClient;
		rTemp.bottom = 1;
		pDC->FillSolidRect(rTemp, ::GetSysColor(COLOR_3DFACE));
		rTemp = rClient;
		rTemp.top = 2;
		pDC->FillSolidRect(rTemp, crBack);
	}
	else
	{
		rTemp = rClient;
		rTemp.top = rClient.bottom - 1;
		pDC->FillSolidRect(rTemp, ::GetSysColor(COLOR_3DFACE));
		rTemp = rClient;
		rTemp.bottom = rClient.bottom - 2;
		pDC->FillSolidRect(rTemp, crBack);
	}
	return TRUE;
}

void CBaseTabCtrl::OnPaint() 
{
	if( GetItemCount()== 0 )
	{
		CPaintDC dc(this); // device context for painting
		return;
	}
	if (m_nTabStyle != 1)
	{
		Default();
	}
	else
	{
		CPaintDC dc(this); // device context for painting

		// prepare dc
		dc.SelectObject(GetFont());

		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc.GetSafeHdc();
		dis.itemAction = ODA_DRAWENTIRE;

		// draw the rest of the border
		CRect rClient, rPage;
		GetClientRect(&dis.rcItem);
		rPage = dis.rcItem;
		AdjustRect(FALSE, rPage);
		BOOL bBottom = GetStyle() & TCS_BOTTOM ;
		if ( bBottom )
			dis.rcItem.bottom = rPage.bottom + 2;
		else
			dis.rcItem.top = rPage.top - 2;

		DrawMainBorder(&dis);

		// paint the tabs first and then the borders
		int nTab = GetItemCount();
		int nSel = GetCurSel();

		if (!nTab) // no pages added
			return;

		while (nTab--)
		{
			if (nTab != nSel)
			{
				dis.itemID = nTab;
				dis.itemState = 0;
				VERIFY(GetItemRect(nTab, &dis.rcItem));
				DrawItem(&dis);
				DrawItemBorder(&dis);
			}
		}

		// now selected tab
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;
		VERIFY(GetItemRect(nSel, &dis.rcItem));
		DrawItem(&dis);
		DrawItemBorder(&dis);
	}
}
