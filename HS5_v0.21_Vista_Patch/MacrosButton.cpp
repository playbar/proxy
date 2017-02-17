// MacrosButton.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "MacrosButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMacrosButton

CMacrosButton::CMacrosButton()
{
	m_pNormal = new CBitmap;
	m_pNormal->LoadBitmap(IDB_BUTTON_NORMAL);

	m_pSelected = new CBitmap;
	m_pSelected->LoadBitmap(IDB_BUTTON_MOUSEDOWN);

	m_pHover = new CBitmap;
	m_pHover->LoadBitmap(IDB_BUTTON_MOUSEON);

	m_pDisabled = new CBitmap;
	m_pDisabled->LoadBitmap(IDB_BUTTON_NORMAL);
	
	//m_bHover = FALSE;
	m_bOver = m_bSelected = m_bTracking = m_bFocus = FALSE;
}

CMacrosButton::~CMacrosButton()
{
	delete m_pNormal;
	delete m_pSelected;
	delete m_pHover;
	delete m_pDisabled;
}


BEGIN_MESSAGE_MAP(CMacrosButton, CButton)
	//{{AFX_MSG_MAP(CMacrosButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacrosButton message handlers

void CMacrosButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC*   pDC = CDC::FromHandle(	lpDrawItemStruct->hDC);
	CRect  rect;
	GetClientRect(rect);

    //获取按钮的状态
	int state = lpDrawItemStruct->itemState;
	if (state & ODS_FOCUS || state & ODS_SELECTED || state & ODS_DEFAULT)
	{
		m_bFocus = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
	}

	if (state & ODS_SELECTED)
	{
		m_bSelected = TRUE;
	}
	else
	{
		m_bSelected = FALSE;
	}
	
	CBitmap * pBmp ;
    if( state & ODS_DISABLED)
		pBmp = m_pDisabled;
	else if( m_bSelected)
		pBmp =m_pSelected;
	else if( m_bOver)
		pBmp = m_pHover;
	else
		pBmp = m_pNormal;
    
	DrawBitmap(pDC,rect,0,pBmp);

	COLORREF crColor;
	if( state & ODS_SELECTED)
		crColor =RGB(255,0,255);
	else if( m_bOver)
		crColor = RGB(255,255,0);
	else
		crColor = RGB(0,0,255);
	DrawCaption(pDC,&rect,state,crColor);
}

void CMacrosButton::DrawBitmap(CDC *pDC, CRect rc, UINT nFlags, CBitmap *pBitmap)
{
	CDC memDC;
	CBitmap* pOld=NULL;
	memDC.CreateCompatibleDC(pDC);
	BITMAP	bmpInfo;

	ASSERT(pBitmap->m_hObject!=NULL);
	pBitmap->GetBitmap(&bmpInfo);
	
	pOld=memDC.SelectObject((CBitmap*) pBitmap);
	if (pOld==NULL) return; //Destructors will clean up
	
	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&memDC,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
	
	memDC.SelectObject(pOld);	
}
void CMacrosButton::DrawCaption(CDC *pDC, CRect *pRect, int  stateItem,COLORREF crColor )
{
     int  nOldMode = pDC->SetBkMode(TRANSPARENT);
	 pDC->SetTextColor(crColor);

	 
	 CString  strText;
	 GetWindowText(strText);

	 //使用父窗口的字体
	 LOGFONT   lf;
	 GetParent()->GetFont()->GetLogFont(&lf);
	 
	 CFont  font;
	 font.CreateFontIndirect(&lf);

	 CFont*  pOldFont = pDC->SelectObject(&font);

	 //下移2个像素
	 if( stateItem & ODS_SELECTED)
	     pRect->top +=2;

	 if( stateItem & ODS_DISABLED)
	 {
		 COLORREF OldColor = pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
		 pDC->DrawText(strText,*pRect+CPoint(1,1),DT_SINGLELINE | DT_LEFT | DT_VCENTER|DT_CENTER ); 
		 pDC->SetTextColor( ::GetSysColor( COLOR_3DSHADOW));
	     pDC->DrawText(strText,pRect,DT_SINGLELINE | DT_LEFT | DT_VCENTER|DT_CENTER );
		 pDC->SetTextColor( OldColor);
	 }
	 else
	 {
		 pDC->DrawText(strText,pRect,DT_SINGLELINE | DT_LEFT | DT_VCENTER|DT_CENTER );
	 }
	 
	 pDC->SelectObject(pOldFont);
	 pDC->SetBkMode(nOldMode);
	 
}

BOOL CMacrosButton::OnEraseBkgnd(CDC* pDC) 
{
	//
	return TRUE;
}

void CMacrosButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	/*
	if (!m_bHover || GetCapture()!=this)
	{
		SetCapture();
		m_bHover=TRUE;
		Invalidate();
	} 
	else
	{
		CRect rc;
		GetClientRect(&rc);
		if (!rc.PtInRect(point))
		{
			m_bHover=FALSE;
			ReleaseCapture();
			Invalidate();
		}
	}
	*/
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CMacrosButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return 0;
}

LRESULT CMacrosButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	
	m_bOver = TRUE;
	InvalidateRect(NULL);
	//HCURSOR   hCursor = LoadCursor(NULL,  (LPSTR)IDC_HAND);
	//SetCursor(hCursor);
	return 0;
}
