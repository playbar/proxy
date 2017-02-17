// PenParamsList.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "PenParamsList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPenParamsList

CPenParamsList::CPenParamsList()
{
	m_nParamIndex = PEN_WIDTH;
}

CPenParamsList::~CPenParamsList()
{
}


BEGIN_MESSAGE_MAP(CPenParamsList, CListBox)
	//{{AFX_MSG_MAP(CPenParamsList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenParamsList message handlers

int CPenParamsList::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	if (lpCompareItemStruct->itemData1 == lpCompareItemStruct->itemData2)
		return 0;       // exact match
	else if (lpCompareItemStruct->itemData1 < lpCompareItemStruct->itemData2)
		return -1;      
	else
		return 1;  
	
	
	return 0;
}

void CPenParamsList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nWidth = lpDrawItemStruct->itemData; 
	int nStyle = lpDrawItemStruct->itemData; 
	if (m_nParamIndex == PEN_STYLE)
		nWidth = 1;
	
	CPen pen;
	COLORREF crLine=RGB(255,0,0);
	int nCenterPos=(CRect(lpDrawItemStruct->rcItem).Height()-nWidth)/2+CRect(lpDrawItemStruct->rcItem).TopLeft().y;

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		// Paint the color item in the color requested
		crLine=RGB(0,0,0);
		CBrush br(GetSysColor(COLOR_BTNFACE));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
	}

	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		crLine=RGB(0,0,0);
		CBrush br(GetSysColor(COLOR_BTNFACE));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
	}

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		crLine=RGB(255,0,0);
		CBrush br(RGB(0,0,255));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
	}

	if (m_nParamIndex == PEN_WIDTH)
	{
		pen.CreatePen(PS_SOLID, 1, crLine);
		CPen *pOldPen=pDC->SelectObject(&pen);
		for(int i=0;i<nWidth;i++)
		{
			pDC->MoveTo(5,nCenterPos+i);
			pDC->LineTo(CRect(lpDrawItemStruct->rcItem).Width()-5,nCenterPos+i);
		}
		pDC->SelectObject(pOldPen)->DeleteObject();
	}
	else if (m_nParamIndex == PEN_STYLE)
	{
		pen.CreatePen(nStyle, 1, crLine);
		CPen *pOldPen=pDC->SelectObject(&pen);
		pDC->MoveTo(5,nCenterPos);
		pDC->LineTo(CRect(lpDrawItemStruct->rcItem).Width()-5,nCenterPos);
		pDC->SelectObject(pOldPen)->DeleteObject();
	}

	DrawBorder(pDC);
}

void CPenParamsList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = ITEM_HEIGHT;
}

void CPenParamsList::AddLPItem(UINT uItem)
{
	AddString((LPCTSTR) uItem);
}

void CPenParamsList::DrawBorder(CDC *pDC)
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
