// DibView.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "DibView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDibView

IMPLEMENT_DYNCREATE(CDibView, CScrollView)

CDibView::CDibView()
{
	m_pDib = NULL;
	m_pFloatWnd = NULL;
	m_bDrawingRubber = FALSE;
	m_bDrawFreeline = FALSE;
	m_nDrawCurveStep = -1;
	m_bDrawCurve = FALSE;
	m_nDrawType = DT_SELECT;
	m_nPenStyle = PS_SOLID;
	m_nPenWidth = 1;
	m_crPenColor = RGB(0,0,0);
	m_crFillColor = RGB(0,0,0);
	m_nTextAlign = DT_LEFT;

	m_hCursorGeneralDraw = NULL;
	m_hCursorFreeline = NULL;
	m_hCursorFill = NULL;
	m_hCursorCurve = NULL;
}

CDibView::~CDibView()
{
}


BEGIN_MESSAGE_MAP(CDibView, CScrollView)
	//{{AFX_MSG_MAP(CDibView)
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibView drawing

BOOL CDibView::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_hCursorGeneralDraw = AfxGetApp()->LoadCursor(IDC_CURSORGENERALDRAW);
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS,
					m_hCursorGeneralDraw, 
					(HBRUSH)(COLOR_WINDOW-1),
					AfxGetApp()->LoadIcon(IDR_IMAGEBTYPE));
	
	return CScrollView::PreCreateWindow(cs);
}

void CDibView::Initialize(CDib* pDib)
{
	m_pDib = pDib;
}

void CDibView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDibView::OnDraw(CDC* pDC)
{
	if (::IsWindow(m_EditText.m_hWnd))
	{
		CRect rc;
		m_EditText.GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.InflateRect(2,2);

		CPen pen(PS_DASH,1,RGB(0,0,255));
		CPen *pOldPen = pDC->SelectObject(&pen);
		CBrush *pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)::GetStockObject(NULL_BRUSH)));
		pDC->Rectangle(&rc);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDibView diagnostics

#ifdef _DEBUG
void CDibView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDibView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDibView message handlers

void CDibView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	DeleteFloatWnd();
	delete m_pFont;
}

void CDibView::CreateFloatWnd(HDIB hDib, CPoint ptTopLeft)
{
	MergeFloatDib();

	// get DIB width and height
    LPBYTE lpDIB = (LPBYTE)GlobalLock(hDib); 
	if (! lpDIB)
	{
		GlobalUnlock(hDib);
		return;
	}
	int nWidth  = (int)DIBWidth(lpDIB);
	int nHeight = (int)DIBHeight(lpDIB);
	GlobalUnlock(hDib);

	// create float window
	CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
	m_pFloatWnd	= new CFloatDibWnd(hDib, rc, this);
}

void CDibView::DeleteFloatWnd()
{
	if (m_pFloatWnd)
	{
		delete m_pFloatWnd;
		m_pFloatWnd = NULL;
	}
}

void CDibView::ClientToDib(CPoint& point)
{
	point.x += GetScrollPos(SB_HORZ);
	point.y += GetScrollPos(SB_VERT);
}

void CDibView::ClientToDib(CRect& rect)
{
	rect.left += GetScrollPos(SB_HORZ);
	rect.top += GetScrollPos(SB_VERT);
	rect.right += GetScrollPos(SB_HORZ);
	rect.bottom += GetScrollPos(SB_VERT);
}

void CDibView::DibToClient(CPoint& point)
{
	point.x -= GetScrollPos(SB_HORZ);
	point.y -= GetScrollPos(SB_VERT);
}

void CDibView::DibToClient(CRect& rect)
{
	rect.left -= GetScrollPos(SB_HORZ);
	rect.top -= GetScrollPos(SB_VERT);
	rect.right -= GetScrollPos(SB_HORZ);
	rect.bottom -= GetScrollPos(SB_VERT);
}

BOOL CDibView::AdjustPointinDib(CPoint& point)
{
	int nWidth = m_pDib->GetWidth();
	int nHeight = m_pDib->GetHeight();
	BOOL bOut = FALSE;

	ClientToDib(point);
	if (point.x < 0)
	{
		point.x = 0;
		bOut = TRUE;
	}
	else if (point.x >= nWidth)
	{
		point.x = nWidth;
		bOut = TRUE;
	}
	if (point.y < 0)
	{
		point.y = 0;
		bOut = TRUE;
	}
	else if (point.y >= nHeight)
	{
		point.y = nHeight;
		bOut = TRUE;
	}
	DibToClient(point);

	return bOut;
}

void CDibView::DoDrawRubber(CDC *pDC, CRect rc)
{
	if (rc.IsRectEmpty())
		return;

	// draw 
	switch(m_nDrawType)
	{
	case DT_SELECT:
	case DT_TEXT:
		pDC->Rectangle(&rc);
		break;
	case DT_LINE:
		{
			// use client coordinates
			CPoint ptStart(m_ptStart);
			DibToClient(ptStart);
			if (ptStart == rc.TopLeft() || 
				ptStart == rc.BottomRight())
			{
				pDC->MoveTo(rc.TopLeft());
				pDC->LineTo(rc.BottomRight());
			}
			else
			{
				pDC->MoveTo(rc.right, rc.top);
				pDC->LineTo(rc.left, rc.bottom);
			}
		}
		break;
	case DT_CURVE:
		{
			if (m_nDrawCurveStep == 0)
			{
				// use client coordinates
				CPoint ptStart(m_ptStart);
				DibToClient(ptStart);
				if (ptStart == rc.TopLeft() || 
					ptStart == rc.BottomRight())
				{
					pDC->MoveTo(rc.TopLeft());
					pDC->LineTo(rc.BottomRight());
				}
				else
				{
					pDC->MoveTo(rc.right, rc.top);
					pDC->LineTo(rc.left, rc.bottom);
				}
			}
		}
		break;
	case DT_RECT_H:
	case DT_RECT_F:
		pDC->Rectangle(&rc);
		break;
	case DT_ROUNDRECT_H:
	case DT_ROUNDRECT_F:
		pDC->RoundRect(&rc, CPoint((int)(rc.Width()/3), (int)(rc.Height()/3)));
		break;
	case DT_ELLIP_H:
	case DT_ELLIP_F:
		pDC->Ellipse(&rc);
		break;
	}
}

void CDibView::StartDrawRubber(CPoint point)
{
	// save current mouse position
	ClientToDib(point);
	m_ptStart = point;

	// empty current rectangle
	m_rcClip.SetRectEmpty();
	m_rcRubber.SetRectEmpty();

	// capture mouse
	SetCapture();
}

void CDibView::DrawRubber(CPoint point)
{
	// get DC and set its ROP
	CClientDC dc(this);

	// define used pen
	int nPenStyle;
	int nPenWidth;
	COLORREF color;
	int nOldRop = dc.SetROP2(R2_NOTXORPEN);
	if (m_nDrawType == DT_SELECT || m_nDrawType == DT_TEXT)
	{
		nPenStyle = PS_DOT;
		nPenWidth = 1;
		color = RGB(0,0,0);
	}
	else
	{
		nPenStyle = m_nPenStyle;
		nPenWidth = m_nPenWidth;
		color = m_crPenColor;
	}
	CPen pen(nPenStyle, nPenWidth, color);
	CPen* pOldPen = dc.SelectObject(&pen);

	// if there is rectangle drawn, clear it
	DoDrawRubber(&dc, m_rcRubber);

	// Adjust cooridnates for select
	if (m_nDrawType == DT_SELECT)
	{
		// get current scroll pos
		int nScrollX = GetScrollPos(SB_HORZ);
		int nScrollY = GetScrollPos(SB_VERT);
		// calculate new scroll pos, and set it
		CRect rcClient;
		GetClientRect(&rcClient);
		int nMinX, nMaxX, nMinY, nMaxY;
		GetScrollRange(SB_HORZ, &nMinX, &nMaxX);
		GetScrollRange(SB_VERT, &nMinY, &nMaxY);
		BOOL bNeedRedraw = FALSE;
		if ((rcClient.Width() < m_pDib->GetWidth()) &&
			(point.x < 0 || point.x > rcClient.right))
		{
			nScrollX += point.x;
			nScrollX = BOUND(nScrollX, nMinX, nMaxX);
			SetScrollPos(SB_HORZ, nScrollX);
			bNeedRedraw = TRUE;
		}
		if ((rcClient.Height() < m_pDib->GetHeight()) &&
			(point.y < 0 || point.y > rcClient.bottom))
		{
			nScrollY += point.y;
			nScrollY = BOUND(nScrollY, nMinY, nMaxY);
			SetScrollPos(SB_VERT, nScrollY);
			bNeedRedraw = TRUE;
		}
		if (bNeedRedraw)
		{
			// redraw
			Invalidate(FALSE);
			UpdateWindow();
		}
		// normalize point coordinate
		if (AdjustPointinDib(point))
		{
			ClientToScreen(&point);
			SetCursorPos(point.x, point.y);
			ScreenToClient(&point);
		}
	}

	// use client coordinates
	CPoint ptStart(m_ptStart);
	DibToClient(ptStart);
	// set new rectangle
	if (point.x < ptStart.x)
	{
		m_rcRubber.left = point.x;
		m_rcRubber.right = ptStart.x;
	}	
	else
	{
		m_rcRubber.left = ptStart.x;
		m_rcRubber.right = point.x;
	}
	if (point.y < ptStart.y)
	{
		m_rcRubber.top = point.y;
		m_rcRubber.bottom = ptStart.y;
	}
	else
	{
		m_rcRubber.top = ptStart.y;
		m_rcRubber.bottom = point.y;
	}
	m_rcRubber.NormalizeRect();

	// draw new rectangle
	DoDrawRubber(&dc, m_rcRubber);

	// restore
	dc.SelectObject(pOldPen);
	dc.SetROP2(nOldRop);
}

void CDibView::StopDrawRubber()
{
	if (m_nDrawType == DT_SELECT || m_nDrawType == DT_TEXT)
	{
		m_rcClip = m_rcRubber;

		// if there is rectangle drawn, clear it
		if (! m_rcClip.IsRectEmpty())
		{
			// get DC and set its ROP
			CClientDC dc(this);
			int OldRop = dc.SetROP2(R2_NOTXORPEN);
			// define used pen
			CPen pen(PS_DOT, 1, RGB(0,0,0));
			CPen* pOldPen = dc.SelectObject(&pen);
			
			// draw to clear
			DoDrawRubber(&dc, m_rcClip);
		
			// restore
			dc.SetROP2(OldRop);
			dc.SelectObject(pOldPen);
		}
		
		if (m_nDrawType == DT_TEXT)
		{
			// empty clip area 
			m_rcClip.SetRectEmpty();

			CRect rc = m_rcRubber;

			CClientDC dc(this);
			CFont *pOldFont = dc.SelectObject(m_pFont);
			CRect rcLetter(0,0,1,1);
			int nHeight = dc.DrawText(_T("жа"),&rcLetter, DT_CALCRECT);
			int nWidth = 4*rcLetter.Width();
			dc.SelectObject(pOldFont);

			if (rc.Height() < nHeight)
				rc.bottom = rc.top + nHeight;
			if (rc.Width() < nWidth)
				rc.right = rc.left + nWidth;
			ClientToDib(rc);
			if (rc.bottom > m_pDib->GetHeight())
				rc.bottom = m_pDib->GetHeight();
			if (rc.right > m_pDib->GetWidth())
				rc.right = m_pDib->GetWidth();
			DibToClient(rc);

			DWORD style = ES_LEFT;
			if (m_nTextAlign == DT_LEFT)
				style = ES_LEFT;
			else if (m_nTextAlign == DT_CENTER)
				style = ES_CENTER;
			else if (m_nTextAlign == DT_RIGHT)
				style = ES_RIGHT;
			m_EditText.Create(style|WS_VISIBLE|WS_CHILD|ES_MULTILINE, 
						  rc, 
						  this, 
						  IDC_EDIT);
			rc.InflateRect(2,2);
			InvalidateRect(&rc);
			m_EditText.SetFont(m_pFont);
			m_EditText.SetFocus();
		}
	}
	else
	{
		// if there is rectangle drawn, clear it
		CClientDC dc(this);
		CPen pen(m_nPenStyle, m_nPenWidth, m_crPenColor);
		int nOldRop = dc.SetROP2(R2_NOTXORPEN);
		CPen* pOldPen = dc.SelectObject(&pen);
		DoDrawRubber(&dc, m_rcRubber);
		dc.SetROP2(nOldRop);
		dc.SelectObject(pOldPen);
		
		// draw on dib		
		CRect rc = m_rcRubber;
		ClientToDib(rc);
		CBrush brush(m_crFillColor);
		CBrush* pOldBrush;
		CDC * pDibDC = m_pDib->BeginPaint(&dc);
		pOldPen = pDibDC->SelectObject(&pen);
		if (m_nDrawType == DT_RECT_F || 
			m_nDrawType == DT_ROUNDRECT_F || 
			m_nDrawType == DT_ELLIP_F)
		{
			pOldBrush = pDibDC->SelectObject(&brush);
		}
		else
			pOldBrush = dc.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(HOLLOW_BRUSH)));

		switch(m_nDrawType)
		{
		case DT_LINE:
			{
				if (m_ptStart == rc.TopLeft() || 
					m_ptStart == rc.BottomRight())
				{
					pDibDC->MoveTo(rc.TopLeft());
					pDibDC->LineTo(rc.BottomRight());
				}
				else
				{
					pDibDC->MoveTo(rc.right, rc.top);
					pDibDC->LineTo(rc.left, rc.bottom);
				}
			}
			break;
		case DT_RECT_H:
		case DT_RECT_F:
			pDibDC->Rectangle(&rc);
			break;
		case DT_ROUNDRECT_H:
		case DT_ROUNDRECT_F:
			pDibDC->RoundRect(&rc, CPoint((int)(rc.Width()/3), (int)(rc.Height()/3)));
			break;
		case DT_ELLIP_H:
		case DT_ELLIP_F:
			pDibDC->Ellipse(&rc);
			break;
		}

		pDibDC->SelectObject(pOldPen);
		pDibDC->SelectObject(pOldBrush);

		m_pDib->EndPaint();

		Invalidate(FALSE);
	}

	// release capture mouse
	ReleaseCapture();
}

BOOL CDibView::PointInDib(CPoint point)
{
	if (m_pDib->IsEmpty())
		return FALSE;

	ClientToDib(point);
	CRect rcDib(0, 0, m_pDib->GetWidth(), m_pDib->GetWidth());
	return rcDib.PtInRect(point);
}

BOOL CDibView::MergeText()
{
	if (! ::IsWindow(m_EditText.m_hWnd))
		return FALSE;

	CString s;
	m_EditText.GetWindowText(s);

	CRect rc;
	m_EditText.GetWindowRect(&rc);
	ScreenToClient(&rc);
	CRect rcClear = rc;
	rcClear.InflateRect(3,3);
	m_EditText.DestroyWindow();
	InvalidateRect(&rcClear);

	// Merge Text into DIB
	if (! s.IsEmpty())
	{
		CClientDC dc(this);

		CDC * pDibDC = m_pDib->BeginPaint(&dc);
		int nOldBkMode = pDibDC->SetBkMode(TRANSPARENT);
		COLORREF crOldTextColor = pDibDC->SetTextColor(m_crPenColor);
		CFont *pOldFont = pDibDC->SelectObject(m_pFont);

		// needed rectangle
		ClientToDib(rc);
		if (rc.bottom > m_pDib->GetHeight())
			rc.bottom = m_pDib->GetHeight();
		if (rc.right > m_pDib->GetWidth())
			rc.right = m_pDib->GetWidth();

		pDibDC->DrawText(s, &rc, m_nTextAlign);
		pDibDC->SetBkMode(nOldBkMode);
		pDibDC->SetTextColor(crOldTextColor);
		pDibDC->SelectObject(pOldFont);
		m_pDib->EndPaint();
	}

	return TRUE;
}

void CDibView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (PointInDib(point))
	{
		// Merge and delete old float DIB (if exist)
		MergeFloatDib();

		// if ther is text, just merge it into DIB
		if (! MergeText())	// else, do paint
		{
			if (m_nDrawType == DT_FREELINE)
			{
				m_bDrawFreeline = TRUE;
				m_ptFreelineStart = point;
				SetCapture();

				// set a pixel anyway
				CClientDC dc(this);
				dc.SetPixel(point, m_crPenColor);

				ClientToDib(point);
				CDC * pDibDC = m_pDib->BeginPaint(&dc);
				pDibDC->SetPixel(point, m_crPenColor);
				m_pDib->EndPaint();
			}
			else if (m_nDrawType == DT_FILL)
			{
				CBrush brush(m_crFillColor);

				CClientDC dc(this);
				COLORREF crColor = dc.GetPixel(point);

				CBrush* pOldBrush = dc.SelectObject(&brush);
				dc.ExtFloodFill(point.x,			// x-coordinate where filling begins
								point.y,			// y-coordinate where filling begins
								crColor,			// fill color  
								FLOODFILLSURFACE);	// fill type
				dc.SelectObject(pOldBrush);

				ClientToDib(point);
				CDC * pDibDC = m_pDib->BeginPaint(&dc);
				pOldBrush = pDibDC->SelectObject(&brush);
				pDibDC->ExtFloodFill(point.x,				// x-coordinate where filling begins
									 point.y,				// y-coordinate where filling begins
									 crColor,				// fill color  
									 FLOODFILLSURFACE);		// fill type
				pDibDC->SelectObject(pOldBrush);
				m_pDib->EndPaint();
			}
			else if (m_nDrawType == DT_CURVE)
			{
				if (! m_bDrawCurve)
				{
					m_bDrawCurve = TRUE;
					m_nDrawCurveStep = 1;
					m_ptCurve[0] = point;
					m_ptCurve[1] = point;
					m_ptCurve[2] = point;
					m_ptCurve[3] = point;
					// capture mouse
					::SetCursor(m_hCursorCurve);
					SetCapture();
				}
				else
				{
					CRect rc;
					GetClientRect(&rc);
					if (! rc.PtInRect(point))
					{
						DrawTmpCurve();
						m_bDrawCurve = FALSE;
						ReleaseCapture();
						::SetCursor(m_hCursorGeneralDraw);
					}
					else
					{
						DrawTmpCurve();
						if (m_nDrawCurveStep == 2)
							m_ptCurve[2] = point;
						else if (m_nDrawCurveStep == 3)
							m_ptCurve[1] = point;
						DrawTmpCurve();
					}
				}
			}
			else
			{
				if (m_nDrawType == DT_LINE)
				{
					// set a pixel anyway
					CClientDC dc(this);
					dc.SetPixel(point, m_crPenColor);

					ClientToDib(point);
					CDC * pDibDC = m_pDib->BeginPaint(&dc);
					pDibDC->SetPixel(point, m_crPenColor);
					m_pDib->EndPaint();
				}
				// start draw rectangle
				StartDrawRubber(point);
				m_bDrawingRubber = TRUE;
			}
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CDibView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// change rectangle
	if (m_bDrawingRubber)
		DrawRubber(point);

	if (m_bDrawCurve && nFlags == MK_LBUTTON)
	{
		DrawTmpCurve();
		if (m_nDrawCurveStep == 1)
			m_ptCurve[3] = point;
		else if (m_nDrawCurveStep == 2)
			m_ptCurve[2] = point;
		else if (m_nDrawCurveStep == 3)
			m_ptCurve[1] = point;
		DrawTmpCurve();
	}

	// draw FREELINE
	if (m_bDrawFreeline) //m_nDrawType == DT_FREELINE && nFlags == MK_LBUTTON)
	{
		CPen pen(m_nPenStyle, m_nPenWidth, m_crPenColor);

		CClientDC dc(this);
		CPen* pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(m_ptFreelineStart);
		dc.LineTo(point);
		dc.SelectObject(pOldPen);

		CPoint pt = point;

		ClientToDib(m_ptFreelineStart);
		ClientToDib(pt);
		CDC * pDibDC = m_pDib->BeginPaint(&dc);
		pOldPen = pDibDC->SelectObject(&pen);
		pDibDC->MoveTo(m_ptFreelineStart);
		pDibDC->LineTo(pt);
		pDibDC->SelectObject(pOldPen);
		m_pDib->EndPaint();

		// new start point
		m_ptFreelineStart = point;
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CDibView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDrawFreeline)
	{
		m_bDrawFreeline = FALSE;
		ReleaseCapture();
	}
	else if (m_bDrawCurve)
	{
		m_nDrawCurveStep++;
		// finish draw curve
		if (m_nDrawCurveStep > 3)
		{
			ReleaseCapture();
			::SetCursor(m_hCursorGeneralDraw);
			m_bDrawCurve = FALSE;
			DrawCurve();
		}
	}
	else if (m_bDrawingRubber)
	{
		StopDrawRubber();
		m_bDrawingRubber = FALSE;

		if (! m_rcClip.IsRectEmpty())
		{
			// adjust position with scroll position
			CRect rcInDib(m_rcClip);
			ClientToDib(rcInDib);

			// create float DIB
			HDIB hDib = m_pDib->CopyRect(rcInDib);

			// create new float DIB window
			CreateFloatWnd(hDib, m_rcClip.TopLeft());
		}
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CDibView::DrawTmpCurve()
{
	CPen penCurve(PS_SOLID, m_nPenWidth, m_crPenColor);

	CClientDC dc(this);
	CPen* pOldPen = dc.SelectObject(&penCurve);
	int nOldRop = dc.SetROP2(R2_NOTXORPEN);
	dc.PolyBezier(m_ptCurve, 4);
	dc.SelectObject(pOldPen);
	dc.SetROP2(nOldRop);
}

void CDibView::DrawCurve()
{
	DrawTmpCurve();

	CPen penCurve(PS_SOLID, m_nPenWidth, m_crPenColor);

	CClientDC dc(this);
	CDC * pDibDC = m_pDib->BeginPaint(&dc);

	CPen* pOldPen = pDibDC->SelectObject(&penCurve);
	pDibDC->PolyBezier(m_ptCurve, 4);
	pDibDC->SelectObject(pOldPen);
	m_pDib->EndPaint();

	Invalidate(FALSE);
}

void CDibView::CutSelectedRect()
{
	if (! m_rcClip.IsRectEmpty())
	{
		// adjust position with scroll position
		CRect rcInDib(m_rcClip);
		ClientToDib(rcInDib);

		// cut select rectangle in m_pDib
		m_pDib->CutRect(rcInDib);

		// empty current rectangle
		m_rcClip.SetRectEmpty();

		// document changed
		CDocument* pDoc = GetDocument();
		pDoc->SetModifiedFlag(TRUE);
	}
}

void CDibView::MergeFloatDib()
{
	if (m_pFloatWnd)
	{
		CRect rc;
		m_pFloatWnd->GetWindowRect(&rc);
		CPoint point = rc.TopLeft();
		ScreenToClient(&point);
		ClientToDib(point);
		m_pDib->MergeDib(m_pFloatWnd->m_hDibFloat, point);
	
		// document changed
		CDocument* pDoc = GetDocument();
		pDoc->SetModifiedFlag(TRUE);
	}
	DeleteFloatWnd();
}

void CDibView::DeleteFloatDib()
{
	// if selected rect is exist, cut it
	CutSelectedRect();
	// if float DIB window exist, delete it
	DeleteFloatWnd();
}

void CDibView::CutFloatDib()
{
	// copy to clipboard first
	CopyToClipboard();
	// then delete
	DeleteFloatDib();
}

void CDibView::CopyToClipboard()
{
	// Clean clipboard of contents, and copy the DIB/DDB/PAL.
	// Actual copy will occured in WM_RENDERALLFORMATS/WM_RENDERFORMAT
	if (OpenClipboard())
	{
		EmptyClipboard();

		HDIB hDib;
		HBITMAP hBitmap;
		HPALETTE hPalette;
		// if there is float dib, copy it
		if (m_pFloatWnd)
		{
			hDib = CopyHandle(m_pFloatWnd->m_hDibFloat);
			hBitmap = DIBToBitmap(m_pFloatWnd->m_hDibFloat, (HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
			hPalette = CopyPalette((HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
		}
		else	// otherwise, copy the entire DIB
		{
			hDib = CopyHandle(m_pDib->m_hDib);
			hBitmap = DIBToBitmap(m_pDib->m_hDib, (HPALETTE)m_pDib->m_pPalette->GetSafeHandle ());
			hPalette = CopyPalette((HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
		}

		// set them to clipboard
		SetClipboardData(CF_DIB, hDib);
		SetClipboardData(CF_BITMAP, hBitmap);
		SetClipboardData(CF_PALETTE, hPalette);

		CloseClipboard();
	}
}

void CDibView::Cut()
{
	// copy to clipboard first
	CopyToClipboard();
	// then delete
	Delete();
}

void CDibView::Delete()
{
	if (m_pFloatWnd) // clip area
		DeleteFloatDib();
	else			// entire DIB
	{
		m_pDib->Destroy();
		// modify document flags
		CSize sizeTotal(0, 0);
		SetScrollSizes(MM_TEXT, sizeTotal);
		CDocument* pDoc = GetDocument();
		pDoc->UpdateAllViews(NULL);
		pDoc->SetModifiedFlag(TRUE);
	}
}

BOOL CDibView::PasteDibFromClipboard()
{
	// get DIB handle from clipboard
	HDIB hNewDIB = NULL;
	if (OpenClipboard())
	{
		if (IsClipboardFormatAvailable(CF_DIB))
			hNewDIB = CopyHandle(GetClipboardData(CF_DIB));

		CloseClipboard();
	}
	if (hNewDIB == NULL)
		return FALSE;

	// if current DIB is empty, use the pasted one as current
	if (m_pDib->IsEmpty())
	{
		// create current DIB
		LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDIB);
		m_pDib->Create(lpDIB);
		GlobalUnlock(hNewDIB);
		GlobalFree(hNewDIB);

		// modify document flags
		CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		CDocument* pDoc = GetDocument();
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
		pDoc->SetModifiedFlag(TRUE);
	}
	else // put it in float DIB window
	{
		// empty current rectangle
		m_rcClip.SetRectEmpty();
		CreateFloatWnd(hNewDIB, CPoint(0, 0));
	}

	return TRUE;
}

BOOL CDibView::PasteBitmapFromClipboard()
{
	// get DIB handle from clipboard
	HBITMAP hNewDDB = NULL;
	HPALETTE hPal = NULL;
	if (OpenClipboard())
	{
		if (IsClipboardFormatAvailable(CF_BITMAP))
			hNewDDB = (HBITMAP)GetClipboardData(CF_BITMAP);
		if (IsClipboardFormatAvailable(CF_PALETTE))
			hPal = (HPALETTE)GetClipboardData(CF_PALETTE);

		CloseClipboard();
	}
	if (hNewDDB == NULL)
		return FALSE;

	// if current DIB is empty, use the pasted one as current
	if (m_pDib->IsEmpty())
	{
		// create current DIB
		m_pDib->Create(hNewDDB, hPal);

		// modify document flags
		CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		CDocument* pDoc = GetDocument();
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
		pDoc->SetModifiedFlag(TRUE);
	}
	else // put it in float DIB window
	{
		// empty current rectangle
		m_rcClip.SetRectEmpty();
		HDIB hDib = BitmapToDIB(hNewDDB, (HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
		HDIB hNewDib = ConvertDIBFormat(hDib, m_pDib->GetBitCount(), (HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
		DestroyDIB(hDib);
		CreateFloatWnd(hNewDib, CPoint(0, 0));
	}

	return TRUE;
}

BOOL CDibView::PastePalFromClipboard()
{
	// egt DIB handle from clipboard
	HPALETTE hPal = NULL;
	if (OpenClipboard())
	{
		if (IsClipboardFormatAvailable(CF_PALETTE))
			hPal = (HPALETTE)GetClipboardData(CF_PALETTE);

		CloseClipboard();
	}
	if (hPal == NULL)
		return FALSE;

	// if current DIB is empty, do nothing
	if (m_pDib->IsEmpty())
		return FALSE;

	// get color bits
	int nEntries;
	WORD wBpp;
	GetObject(hPal, sizeof(int), &nEntries);
	if (nEntries > 16)
		wBpp = 8;
	else if (nEntries > 2)
		wBpp = 4;
	else
		wBpp = 1;

	// convert to new DIB
	HDIB hNewDib = ConvertDIBFormat(m_pDib->m_hDib, wBpp, hPal);
	if (! hNewDib)
		return FALSE;

	m_pDib->Destroy();
	// create current DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	m_pDib->Create(lpDIB);
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
	// modify document flags
	CDocument* pDoc = GetDocument();
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	pDoc->SetModifiedFlag(TRUE);

	return TRUE;
}

void CDibView::SelectPaste(int nCF)
{
	switch(nCF)
	{
	case CF_DIB:
		PasteDibFromClipboard();
		return;
	case CF_BITMAP:
		PasteBitmapFromClipboard();
		return;
	case CF_PALETTE:
		PastePalFromClipboard();
		return;
	}
}

void CDibView::Paste()
{
	if (! PasteDibFromClipboard())
		if (! PasteBitmapFromClipboard())
			PastePalFromClipboard();
}

void CDibView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);

	if (m_pDib->IsEmpty())
		return;  // must be a new document

	CPalette* pPal = m_pDib->GetPalette();
	if (pPal != NULL)
	{
		CWnd* pAppFrame = AfxGetApp()->m_pMainWnd;

		CClientDC appDC(pAppFrame);
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);

		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				GetDocument()->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tSelectPalette failed in CDibView::OnPaletteChanged\n");
		}
	}
}

void CDibView::Rotate(int nAngle)
{
	if (m_pFloatWnd)
	{
		LPBYTE lpDIB = (LPBYTE)GlobalLock(m_pFloatWnd->m_hDibFloat);
		CDib dib;
		if (! dib.Create(lpDIB))
			return;
		GlobalUnlock(m_pFloatWnd->m_hDibFloat);
		switch (nAngle)
		{
		case 90:
			dib.Rotate90();
			break;
		case 180:
			dib.Rotate180();
			break;
		case 270:
			dib.Rotate270();
			break;
		}

		// get float window position
		CRect rcFloat;
		m_pFloatWnd->GetWindowRect(&rcFloat);
		CPoint ptTopLeft = rcFloat.TopLeft();
		ScreenToClient(&ptTopLeft);

		// delete old float window
		DeleteFloatWnd();
		CutSelectedRect();

		// create float window
			// get DIB width and height
		int nWidth  = (int)dib.GetWidth();
		int nHeight = (int)dib.GetHeight();
		CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
			// create it
		HDIB hDib = CopyHandle(dib.GetHandle());
		m_pFloatWnd	= new CFloatDibWnd(hDib, rc, this);
	}
	else	// otherwise, copy the entire DIB
	{
		switch (nAngle)
		{
		case 90:
			m_pDib->Rotate90();
			break;
		case 180:
			m_pDib->Rotate180();
			break;
		case 270:
			m_pDib->Rotate270();
			break;
		}
	}
}

void CDibView::Flip(int nDirection)
{
	if (m_pFloatWnd)
	{
		LPBYTE lpDIB = (LPBYTE)GlobalLock(m_pFloatWnd->m_hDibFloat);
		CDib dib;
		if (! dib.Create(lpDIB))
			return;
		GlobalUnlock(m_pFloatWnd->m_hDibFloat);
		switch (nDirection)
		{
		case MD_HORZ:
			dib.FlipHorz();
			break;
		case MD_VERT:
			dib.FlipVert();
			break;
		}

		// get float window position
		CRect rcFloat;
		m_pFloatWnd->GetWindowRect(&rcFloat);
		CPoint ptTopLeft = rcFloat.TopLeft();
		ScreenToClient(&ptTopLeft);

		// delete old float window
		DeleteFloatWnd();
		CutSelectedRect();

		// create float window
			// get DIB width and height
		int nWidth  = (int)dib.GetWidth();
		int nHeight = (int)dib.GetHeight();
		CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
			// create it
		HDIB hDib = CopyHandle(dib.GetHandle());
		m_pFloatWnd	= new CFloatDibWnd(hDib, rc, this);
	}
	else	// otherwise, copy the entire DIB
	{
		switch (nDirection)
		{
		case MD_HORZ:
			m_pDib->FlipHorz();
			break;
		case MD_VERT:
			m_pDib->FlipVert();
			break;
		}
	}
}

void CDibView::Zoom(double fRatioX, double fRatioY)
{
	if (m_pFloatWnd)
	{
		LPBYTE lpDIB = (LPBYTE)GlobalLock(m_pFloatWnd->m_hDibFloat);
		CDib dib;
		if (! dib.Create(lpDIB))
			return;
		GlobalUnlock(m_pFloatWnd->m_hDibFloat);
		dib.Zoom(fRatioX, fRatioY);

		// get float window position
		CRect rcFloat;
		m_pFloatWnd->GetWindowRect(&rcFloat);
		CPoint ptTopLeft = rcFloat.TopLeft();
		ScreenToClient(&ptTopLeft);

		// delete old float window
		DeleteFloatWnd();
		CutSelectedRect();

		// create float window
			// get DIB width and height
		int nWidth  = (int)dib.GetWidth();
		int nHeight = (int)dib.GetHeight();
		CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
			// create it
		HDIB hDib = CopyHandle(dib.GetHandle());
		m_pFloatWnd	= new CFloatDibWnd(hDib, rc, this);
	}
	else	// otherwise, copy the entire DIB
	{
		m_pDib->Zoom(fRatioX, fRatioY);
	}
}

void CDibView::ChangeImageSize(int nWidth, int nHeight)
{
	if (m_pFloatWnd)
	{
		LPBYTE lpDIB = (LPBYTE)GlobalLock(m_pFloatWnd->m_hDibFloat);
		CDib dib;
		if (! dib.Create(lpDIB))
			return;
		GlobalUnlock(m_pFloatWnd->m_hDibFloat);
		dib.ChangeImageSize(nWidth, nHeight);

		// get float window position
		CRect rcFloat;
		m_pFloatWnd->GetWindowRect(&rcFloat);
		CPoint ptTopLeft = rcFloat.TopLeft();
		ScreenToClient(&ptTopLeft);

		// delete old float window
		DeleteFloatWnd();
		CutSelectedRect();

		// create float window
			// get DIB width and height
		int nWidth  = (int)dib.GetWidth();
		int nHeight = (int)dib.GetHeight();
		CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
			// create it
		HDIB hDib = CopyHandle(dib.GetHandle());
		m_pFloatWnd	= new CFloatDibWnd(hDib, rc, this);
	}
	else	// otherwise, copy the entire DIB
	{
		m_pDib->ChangeImageSize(nWidth, nHeight);
	}
}

void CDibView::ChangeCanvasSize(int nWidth, int nHeight, int nPosition)
{
	if (m_pFloatWnd)
	{
		LPBYTE lpDIB = (LPBYTE)GlobalLock(m_pFloatWnd->m_hDibFloat);
		CDib dib;
		if (! dib.Create(lpDIB))
			return;
		GlobalUnlock(m_pFloatWnd->m_hDibFloat);
		dib.ChangeCanvasSize(nWidth, nHeight, nPosition);

		// get float window position
		CRect rcFloat;
		m_pFloatWnd->GetWindowRect(&rcFloat);
		CPoint ptTopLeft = rcFloat.TopLeft();
		ScreenToClient(&ptTopLeft);

		// delete old float window
		DeleteFloatWnd();
		CutSelectedRect();

		// create float window
			// get DIB width and height
		int nWidth  = (int)dib.GetWidth();
		int nHeight = (int)dib.GetHeight();
		CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
			// create it
		HDIB hDib = CopyHandle(dib.GetHandle());
		m_pFloatWnd	= new CFloatDibWnd(hDib, rc, this);
	}
	else	// otherwise, copy the entire DIB
	{
		m_pDib->ChangeCanvasSize(nWidth, nHeight, nPosition);
	}
}

void CDibView::Crop()
{
	if (! m_rcClip.IsRectEmpty())
	{
		m_pDib->Crop(m_rcClip);
		DeleteFloatWnd();
		m_rcClip.SetRectEmpty();
	}
}


HBRUSH CDibView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->m_hWnd == m_EditText.m_hWnd)
		pDC->SetTextColor(m_crPenColor);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

int CDibView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_hCursorFreeline = AfxGetApp()->LoadCursor(IDC_CURSORFREELINE);
	m_hCursorFill = AfxGetApp()->LoadCursor(IDC_CURSORFILL);
	m_hCursorCurve = AfxGetApp()->LoadCursor(IDC_CURSORCURVE);
	
	LOGFONT lf;
	HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(&lf);
	
	return 0;
}

BOOL CDibView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_nDrawType == DT_FREELINE)
	{
		::SetCursor(m_hCursorFreeline);
		return TRUE;
	}
	else if (m_nDrawType == DT_FILL)
	{
		::SetCursor(m_hCursorFill);
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

