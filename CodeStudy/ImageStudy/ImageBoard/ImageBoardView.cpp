// ImageBoardView.cpp : implementation of the CImageBoardView class
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "PenWidth.h"
#include "SelectPasteDlg.h"
#include "Scale.h"
#include "CanvasSize.h"
#include "GrayDlg.h"
#include "BCDlg.h"
#include "AdjustColorDlg.h"
#include "ColorBitsDlg.h"
#include "ConvoluteDlg.h"
#include "RotateAngle.h"
#include "GammaCorrection.h"
#include "ExposureDlg.h"
#include "EmbossmentDlg.h"
#include "SpreadDlg.h"
#include "ImageBoardDoc.h"
#include "ImageBoardView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageBoardView

IMPLEMENT_DYNCREATE(CImageBoardView, CScrollView)

BEGIN_MESSAGE_MAP(CImageBoardView, CScrollView)
	//{{AFX_MSG_MAP(CImageBoardView)
	ON_COMMAND(IDM_EAST, OnEast)
	ON_UPDATE_COMMAND_UI(IDM_EAST, OnUpdateEast)
	ON_COMMAND(IDM_HF1, OnHf1)
	ON_UPDATE_COMMAND_UI(IDM_HF1, OnUpdateHf1)
	ON_COMMAND(IDM_HF2, OnHf2)
	ON_UPDATE_COMMAND_UI(IDM_HF2, OnUpdateHf2)
	ON_COMMAND(IDM_HF3, OnHf3)
	ON_UPDATE_COMMAND_UI(IDM_HF3, OnUpdateHf3)
	ON_COMMAND(IDM_HORZ, OnHorz)
	ON_UPDATE_COMMAND_UI(IDM_HORZ, OnUpdateHorz)
	ON_COMMAND(IDM_LAP1, OnLap1)
	ON_UPDATE_COMMAND_UI(IDM_LAP1, OnUpdateLap1)
	ON_COMMAND(IDM_LAP2, OnLap2)
	ON_UPDATE_COMMAND_UI(IDM_LAP2, OnUpdateLap2)
	ON_COMMAND(IDM_LAP3, OnLap3)
	ON_UPDATE_COMMAND_UI(IDM_LAP3, OnUpdateLap3)
	ON_COMMAND(IDM_LAP4, OnLap4)
	ON_UPDATE_COMMAND_UI(IDM_LAP4, OnUpdateLap4)
	ON_COMMAND(IDM_LF1, OnLf1)
	ON_UPDATE_COMMAND_UI(IDM_LF1, OnUpdateLf1)
	ON_COMMAND(IDM_LF2, OnLf2)
	ON_UPDATE_COMMAND_UI(IDM_LF2, OnUpdateLf2)
	ON_COMMAND(IDM_LF3, OnLf3)
	ON_UPDATE_COMMAND_UI(IDM_LF3, OnUpdateLf3)
	ON_COMMAND(IDM_NORTH, OnNorth)
	ON_UPDATE_COMMAND_UI(IDM_NORTH, OnUpdateNorth)
	ON_COMMAND(IDM_NORTHEAST, OnNortheast)
	ON_UPDATE_COMMAND_UI(IDM_NORTHEAST, OnUpdateNortheast)
	ON_COMMAND(IDM_NORTHWEST, OnNorthwest)
	ON_UPDATE_COMMAND_UI(IDM_NORTHWEST, OnUpdateNorthwest)
	ON_COMMAND(IDM_SOUTH, OnSouth)
	ON_UPDATE_COMMAND_UI(IDM_SOUTH, OnUpdateSouth)
	ON_COMMAND(IDM_SOUTHEAST, OnSoutheast)
	ON_UPDATE_COMMAND_UI(IDM_SOUTHEAST, OnUpdateSoutheast)
	ON_COMMAND(IDM_SOUTHWEST, OnSouthwest)
	ON_UPDATE_COMMAND_UI(IDM_SOUTHWEST, OnUpdateSouthwest)
	ON_COMMAND(IDM_VERT, OnVert)
	ON_UPDATE_COMMAND_UI(IDM_VERT, OnUpdateVert)
	ON_COMMAND(IDM_VERTHORZ, OnVerthorz)
	ON_UPDATE_COMMAND_UI(IDM_VERTHORZ, OnUpdateVerthorz)
	ON_COMMAND(IDM_WEST, OnWest)
	ON_UPDATE_COMMAND_UI(IDM_WEST, OnUpdateWest)
	ON_COMMAND(IDM_MEDIANFILTER, OnMedianfilter)
	ON_UPDATE_COMMAND_UI(IDM_MEDIANFILTER, OnUpdateMedianfilter)
	ON_COMMAND(IDM_INCH, OnInch)
	ON_COMMAND(IDM_MM, OnMm)
	ON_COMMAND(IDM_PIXEL, OnPixel)
	ON_COMMAND(IDM_SELECT, OnSelect)
	ON_COMMAND(IDM_CURVE, OnCurve)
	ON_COMMAND(IDM_LINE, OnLine)
	ON_COMMAND(IDM_FREELINE, OnFreeline)
	ON_COMMAND(IDM_RECT_H, OnRectH)
	ON_COMMAND(IDM_RECT_F, OnRectF)
	ON_COMMAND(IDM_ELLIPSE_H, OnEllipseH)
	ON_COMMAND(IDM_ELLIPSE_F, OnEllipseF)
	ON_COMMAND(IDM_TEXT, OnText)
	ON_COMMAND(IDM_FILL, OnFill)
	ON_COMMAND(IDM_PENCOLOR, OnPencolor)
	ON_COMMAND(IDM_SOLID, OnSolid)
	ON_COMMAND(IDM_DASH, OnDash)
	ON_COMMAND(IDM_DOT, OnDot)
	ON_COMMAND(IDM_FILLCOLOR, OnFillcolor)
	ON_UPDATE_COMMAND_UI(IDM_SELECT, OnUpdateSelect)
	ON_UPDATE_COMMAND_UI(IDM_CURVE, OnUpdateCurve)
	ON_UPDATE_COMMAND_UI(IDM_LINE, OnUpdateLine)
	ON_UPDATE_COMMAND_UI(IDM_FREELINE, OnUpdateFreeline)
	ON_UPDATE_COMMAND_UI(IDM_RECT_H, OnUpdateRectH)
	ON_UPDATE_COMMAND_UI(IDM_RECT_F, OnUpdateRectF)
	ON_UPDATE_COMMAND_UI(IDM_ELLIPSE_H, OnUpdateEllipseH)
	ON_UPDATE_COMMAND_UI(IDM_ELLIPSE_F, OnUpdateEllipseF)
	ON_UPDATE_COMMAND_UI(IDM_FILL, OnUpdateFill)
	ON_UPDATE_COMMAND_UI(IDM_TEXT, OnUpdateText)
	ON_UPDATE_COMMAND_UI(IDM_DASH, OnUpdateDash)
	ON_UPDATE_COMMAND_UI(IDM_DOT, OnUpdateDot)
	ON_UPDATE_COMMAND_UI(IDM_SOLID, OnUpdateSolid)
	ON_COMMAND(IDM_PW_1, OnPw1)
	ON_UPDATE_COMMAND_UI(IDM_PW_1, OnUpdatePw1)
	ON_COMMAND(IDM_PW_2, OnPw2)
	ON_UPDATE_COMMAND_UI(IDM_PW_2, OnUpdatePw2)
	ON_COMMAND(IDM_PW_3, OnPw3)
	ON_UPDATE_COMMAND_UI(IDM_PW_3, OnUpdatePw3)
	ON_COMMAND(IDM_PW_4, OnPw4)
	ON_UPDATE_COMMAND_UI(IDM_PW_4, OnUpdatePw4)
	ON_COMMAND(IDM_PW_5, OnPw5)
	ON_UPDATE_COMMAND_UI(IDM_PW_5, OnUpdatePw5)
	ON_COMMAND(IDM_PW_6, OnPw6)
	ON_UPDATE_COMMAND_UI(IDM_PW_6, OnUpdatePw6)
	ON_COMMAND(IDM_PW_7, OnPw7)
	ON_UPDATE_COMMAND_UI(IDM_PW_7, OnUpdatePw7)
	ON_COMMAND(IDM_PW_8, OnPw8)
	ON_UPDATE_COMMAND_UI(IDM_PW_8, OnUpdatePw8)
	ON_COMMAND(IDM_PW_OTHER, OnPwOther)
	ON_UPDATE_COMMAND_UI(IDM_PW_OTHER, OnUpdatePwOther)
	ON_COMMAND(IDM_ROUNDRECT_F, OnRoundrectF)
	ON_UPDATE_COMMAND_UI(IDM_ROUNDRECT_F, OnUpdateRoundrectF)
	ON_COMMAND(IDM_ROUNDRECT_H, OnRoundrectH)
	ON_UPDATE_COMMAND_UI(IDM_ROUNDRECT_H, OnUpdateRoundrectH)
	ON_COMMAND(IDM_FONT, OnFont)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(IDM_SELECTPASTE, OnSelectpaste)
	ON_UPDATE_COMMAND_UI(IDM_SELECTPASTE, OnUpdateSelectpaste)
	ON_COMMAND(IDM_CROP, OnCrop)
	ON_UPDATE_COMMAND_UI(IDM_CROP, OnUpdateCrop)
	ON_COMMAND(IDM_ROTATE, OnRotate)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE, OnUpdateRotate)
	ON_COMMAND(IDM_ROTATE180, OnRotate180)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE180, OnUpdateRotate180)
	ON_COMMAND(IDM_ROTATE270, OnRotate270)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE270, OnUpdateRotate270)
	ON_COMMAND(IDM_HMIRROR, OnHmirror)
	ON_UPDATE_COMMAND_UI(IDM_HMIRROR, OnUpdateHmirror)
	ON_COMMAND(IDM_VMIRROR, OnVmirror)
	ON_UPDATE_COMMAND_UI(IDM_VMIRROR, OnUpdateVmirror)
	ON_COMMAND(IDM_SCALE, OnScale)
	ON_UPDATE_COMMAND_UI(IDM_SCALE, OnUpdateScale)
	ON_COMMAND(IDM_CANVASSIZE, OnCanvassize)
	ON_UPDATE_COMMAND_UI(IDM_CANVASSIZE, OnUpdateCanvassize)
	ON_COMMAND(IDM_LEFT, OnLeft)
	ON_COMMAND(IDM_CENTER, OnCenter)
	ON_COMMAND(IDM_RIGHT, OnRight)
	ON_UPDATE_COMMAND_UI(IDM_CENTER, OnUpdateCenter)
	ON_UPDATE_COMMAND_UI(IDM_LEFT, OnUpdateLeft)
	ON_UPDATE_COMMAND_UI(IDM_RIGHT, OnUpdateRight)
	ON_COMMAND(IDM_ADJUSTCOLOR, OnAdjustcolor)
	ON_UPDATE_COMMAND_UI(IDM_ADJUSTCOLOR, OnUpdateAdjustcolor)
	ON_COMMAND(IDM_BC, OnBc)
	ON_UPDATE_COMMAND_UI(IDM_BC, OnUpdateBc)
	ON_COMMAND(IDM_COLORBITS, OnColorbits)
	ON_UPDATE_COMMAND_UI(IDM_COLORBITS, OnUpdateColorbits)
	ON_COMMAND(IDM_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(IDM_GRAY, OnUpdateGray)
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(IDM_RED, OnRed)
	ON_UPDATE_COMMAND_UI(IDM_RED, OnUpdateRed)
	ON_COMMAND(IDM_GREEN, OnGreen)
	ON_UPDATE_COMMAND_UI(IDM_GREEN, OnUpdateGreen)
	ON_COMMAND(IDM_BLUE, OnBlue)
	ON_UPDATE_COMMAND_UI(IDM_BLUE, OnUpdateBlue)
	ON_COMMAND(IDM_GETBLUE, OnGetblue)
	ON_UPDATE_COMMAND_UI(IDM_GETBLUE, OnUpdateGetblue)
	ON_COMMAND(IDM_GETGREEN, OnGetgreen)
	ON_UPDATE_COMMAND_UI(IDM_GETGREEN, OnUpdateGetgreen)
	ON_COMMAND(IDM_GETRED, OnGetred)
	ON_UPDATE_COMMAND_UI(IDM_GETRED, OnUpdateGetred)
	ON_COMMAND(IDM_COPYTO, OnCopyto)
	ON_UPDATE_COMMAND_UI(IDM_COPYTO, OnUpdateCopyto)
	ON_COMMAND(IDM_PASTEFROM, OnPastefrom)
	ON_UPDATE_COMMAND_UI(IDM_PASTEFROM, OnUpdatePastefrom)
	ON_COMMAND(IDM_ERASER, OnEraser)
	ON_UPDATE_COMMAND_UI(IDM_ERASER, OnUpdateEraser)
	ON_COMMAND(IDM_PICKER, OnPicker)
	ON_UPDATE_COMMAND_UI(IDM_PICKER, OnUpdatePicker)
	ON_COMMAND(IDM_CLOSE_H, OnCloseH)
	ON_UPDATE_COMMAND_UI(IDM_CLOSE_H, OnUpdateCloseH)
	ON_COMMAND(IDM_CLOSE_V, OnCloseV)
	ON_UPDATE_COMMAND_UI(IDM_CLOSE_V, OnUpdateCloseV)
	ON_COMMAND(IDM_DILATION_H, OnDilationH)
	ON_UPDATE_COMMAND_UI(IDM_DILATION_H, OnUpdateDilationH)
	ON_COMMAND(IDM_DILATION_V, OnDilationV)
	ON_UPDATE_COMMAND_UI(IDM_DILATION_V, OnUpdateDilationV)
	ON_COMMAND(IDM_EROSION_H, OnErosionH)
	ON_UPDATE_COMMAND_UI(IDM_EROSION_H, OnUpdateErosionH)
	ON_COMMAND(IDM_EROSION_V, OnErosionV)
	ON_UPDATE_COMMAND_UI(IDM_EROSION_V, OnUpdateErosionV)
	ON_COMMAND(IDM_OPEN_H, OnOpenH)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_H, OnUpdateOpenH)
	ON_COMMAND(IDM_OPEN_V, OnOpenV)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_V, OnUpdateOpenV)
	ON_COMMAND(IDM_THIN, OnThin)
	ON_UPDATE_COMMAND_UI(IDM_THIN, OnUpdateThin)
	ON_COMMAND(IDM_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(IDM_FFT, OnUpdateFft)
	ON_COMMAND(IDM_DCT, OnDct)
	ON_UPDATE_COMMAND_UI(IDM_DCT, OnUpdateDct)
	ON_COMMAND(IDM_WALH, OnWalh)
	ON_UPDATE_COMMAND_UI(IDM_WALH, OnUpdateWalh)
	ON_COMMAND(IDM_CONTOUR_H, OnContourH)
	ON_UPDATE_COMMAND_UI(IDM_CONTOUR_H, OnUpdateContourH)
	ON_COMMAND(IDM_CONTOUR_V, OnContourV)
	ON_UPDATE_COMMAND_UI(IDM_CONTOUR_V, OnUpdateContourV)
	ON_COMMAND(IDM_ROTATE_ANGLE, OnRotateAngle)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE_ANGLE, OnUpdateRotateAngle)
	ON_COMMAND(IDM_CORRECTGAMMA, OnCorrectgamma)
	ON_UPDATE_COMMAND_UI(IDM_CORRECTGAMMA, OnUpdateCorrectgamma)
	ON_COMMAND(IDM_DIFFERENCEEDGEDETECT, OnDifferenceedgedetect)
	ON_UPDATE_COMMAND_UI(IDM_DIFFERENCEEDGEDETECT, OnUpdateDifferenceedgedetect)
	ON_COMMAND(IDM_EMBOSSMENT, OnEmbossment)
	ON_UPDATE_COMMAND_UI(IDM_EMBOSSMENT, OnUpdateEmbossment)
	ON_COMMAND(IDM_EXPOSURE, OnExposure)
	ON_UPDATE_COMMAND_UI(IDM_EXPOSURE, OnUpdateExposure)
	ON_COMMAND(IDM_HOMOEDGEDETECT, OnHomoedgedetect)
	ON_UPDATE_COMMAND_UI(IDM_HOMOEDGEDETECT, OnUpdateHomoedgedetect)
	ON_COMMAND(IDM_NEGATIVE, OnNegative)
	ON_UPDATE_COMMAND_UI(IDM_NEGATIVE, OnUpdateNegative)
	ON_COMMAND(IDM_SOBELED, OnSobeled)
	ON_UPDATE_COMMAND_UI(IDM_SOBELED, OnUpdateSobeled)
	ON_COMMAND(IDM_SPREAD, OnSpread)
	ON_UPDATE_COMMAND_UI(IDM_SPREAD, OnUpdateSpread)
	ON_COMMAND(IDM_HOUGHED, OnHoughed)
	ON_UPDATE_COMMAND_UI(IDM_HOUGHED, OnUpdateHoughed)
	ON_COMMAND(IDM_CANNYED, OnCannyed)
	ON_UPDATE_COMMAND_UI(IDM_CANNYED, OnUpdateCannyed)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(IDC_PEN_COLOR, OnPencolor1)
	ON_COMMAND(IDC_FILL_COLOR, OnFillcolor1)
	ON_COMMAND_RANGE(IDC_COLORTABLE_BASE, IDC_COLORTABLE_BASE+256, OnPenColorGrid)
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageBoardView construction/destruction

CImageBoardView::CImageBoardView()
{
	m_nLengthUnit = MM_TEXT;
	m_pDib = NULL;
	m_pFloatWnd = NULL;
	m_bDrawingRubber = FALSE;
	m_bDrawFreeline = FALSE;
	m_bErasing = FALSE;
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
	m_hCursorEraser = NULL;
	m_hCursorPicker = NULL;
	m_hCursorFill = NULL;
	m_hCursorCurve = NULL;
}

CImageBoardView::~CImageBoardView()
{
}

BOOL CImageBoardView::PreCreateWindow(CREATESTRUCT& cs)
{
	m_hCursorGeneralDraw = AfxGetApp()->LoadCursor(IDC_CURSORGENERALDRAW);
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS,
					m_hCursorGeneralDraw, 
					(HBRUSH)(COLOR_WINDOW-1),
					AfxGetApp()->LoadIcon(IDR_IMAGEBTYPE));
	
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageBoardView drawing

void CImageBoardView::OnDraw(CDC* pDC)
{
	CImageBoardDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		m_pDib->Display(pDC, 0, 0);

	if (::IsWindow(m_EditText.m_hWnd))
	{
		CRect rc;
		m_EditText.GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.InflateRect(2,2);

		CPen pen1(PS_SOLID,1,RGB(255,255,255));
		CPen pen2(PS_DOT,1,RGB(0,0,0));
		CBrush *pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)::GetStockObject(NULL_BRUSH)));
		CPen *pOldPen = pDC->SelectObject(&pen1);
		pDC->Rectangle(&rc);
		pDC->SelectObject(&pen2);
		pDC->Rectangle(&rc);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}

void CImageBoardView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageBoardDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_pDib = pDoc->m_pDib;

	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CImageBoardView printing

BOOL CImageBoardView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageBoardView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageBoardView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageBoardView diagnostics

#ifdef _DEBUG
void CImageBoardView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageBoardView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageBoardDoc* CImageBoardView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageBoardDoc)));
	return (CImageBoardDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageBoardView message handlers

void CImageBoardView::SetStatusBarBitCount(WORD wBitcount)
{
	CString str;
	switch (wBitcount)
	{
	case 1:
		str.LoadString(IDS_BITCOUNT_1);
		break;
	case 4:
		str.LoadString(IDS_BITCOUNT_4);
		break;
	case 8:
		str.LoadString(IDS_BITCOUNT_8);
		break;
	case 24:
	default:
		str.LoadString(IDS_BITCOUNT_24);
		break;
	}
	CMainFrame* pMainFrame= (CMainFrame*)( AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CMainFrame, pMainFrame);
    pMainFrame->m_wndStatusBar.SetPaneText(PANE_BIT_COUNT, str);
}

void CImageBoardView::SetStatusBarLengthUnit(int nUnit)
{
	CString str;
	switch (m_nLengthUnit)
	{
	case MM_HIENGLISH:
		str.LoadString(IDS_LENGTH_UNIT_INCH);
		break;
	case MM_HIMETRIC:
		str.LoadString(IDS_LENGTH_UNIT_MM);
		break;
	case MM_TEXT:
	default:
		str.LoadString(IDS_LENGTH_UNIT_PIXEL);
		break;
	}
	CMainFrame* pMainFrame= (CMainFrame*)( AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CMainFrame, pMainFrame);
    pMainFrame->m_wndStatusBar.SetPaneText(PANE_LENGTH_UNIT, str);
}

void CImageBoardView::SetStatusBarCursorPosition(CPoint point)
{
	CWindowDC dc(this);
	dc.LPtoDP(&point);
	int nOldUnit = dc.SetMapMode(m_nLengthUnit);
	dc.DPtoLP(&point);
	dc.SetMapMode(nOldUnit);

	CString msg;
	double fCX = abs(point.x), fCY = abs(point.y);
	switch (m_nLengthUnit)
	{
	case MM_TEXT:
		msg.Format("%d,%d", (int)fCX, (int)fCY);
		break;
	case MM_HIENGLISH:
		fCX /= 1000.0;
		fCY /= 1000.0;
		msg.Format("%.2f,%.2f", fCX, fCY);
		break;
	case MM_HIMETRIC:
		fCX /= 100.0;
		fCY /= 100.0;
		msg.Format("%.1f,%.1f", fCX, fCY);
		break;
	}
	CMainFrame* pMainFrame= (CMainFrame*)( AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CMainFrame, pMainFrame);
    pMainFrame->m_wndStatusBar.SetPaneText(PANE_CURSOR_POSITION, msg);
}

void CImageBoardView::SetStatusBarImageSize(CSize size)
{
	CWindowDC dc(this);
	dc.LPtoDP(&size);
	int nOldUnit = dc.SetMapMode(m_nLengthUnit);
	dc.DPtoLP(&size);
	dc.SetMapMode(nOldUnit);

	CString msg;
	double fCX = abs(size.cx), fCY = abs(size.cy);
	switch (m_nLengthUnit)
	{
	case MM_TEXT:
		msg.Format("%d,%d", (int)fCX, (int)fCY);
		break;
	case MM_HIENGLISH:
		fCX /= 1000.0;
		fCY /= 1000.0;
		msg.Format("%.2f,%.2f", fCX, fCY);
		break;
	case MM_HIMETRIC:
		fCX /= 100.0;
		fCY /= 100.0;
		msg.Format("%.1f,%.1f", fCX, fCY);
		break;
	}
	CMainFrame* pMainFrame= (CMainFrame*)( AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CMainFrame, pMainFrame);
    pMainFrame->m_wndStatusBar.SetPaneText(PANE_IMAGE_SIZE, msg);
}

void CImageBoardView::OnInch() 
{
	SetLengthUnit(MM_HIENGLISH);
}

void CImageBoardView::OnMm() 
{
	SetLengthUnit(MM_HIMETRIC);
}

void CImageBoardView::OnPixel() 
{
	SetLengthUnit(MM_TEXT);
}

void CImageBoardView::SetLengthUnit(int nUnit) 
{
	m_nLengthUnit = nUnit;
	SetStatusBarLengthUnit(nUnit);

	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetStatusBarImageSize(sizeTotal);

	CMenu *pMenu =  AfxGetApp()->m_pMainWnd->GetMenu();
	switch (m_nLengthUnit)
	{
	case MM_HIENGLISH:
		pMenu->CheckMenuRadioItem(IDM_PIXEL, IDM_MM, IDM_INCH, MF_BYCOMMAND);
		break;
	case MM_HIMETRIC:
		pMenu->CheckMenuRadioItem(IDM_PIXEL, IDM_MM, IDM_MM, MF_BYCOMMAND);
		break;
	case MM_TEXT:
	default:
		pMenu->CheckMenuRadioItem(IDM_PIXEL, IDM_MM, IDM_PIXEL, MF_BYCOMMAND);
		break;
	}
}

void CImageBoardView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		SetLengthUnit(m_nLengthUnit);
		SetStatusBarBitCount(m_pDib->GetBitCount());
		ShowPenColor();
		ShowFillColor();
		ShowPenWidth();
		ShowPenStyle();
		ShowColorGrid();
	}
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CImageBoardView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CImageBoardDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_pDib = pDoc->m_pDib;

	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetStatusBarImageSize(sizeTotal);
	SetStatusBarBitCount(m_pDib->GetBitCount());
	ShowColorGrid();

	// re paint the entire client area
	Invalidate();
}


void CImageBoardView::OnUpdateSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_SELECT == m_nDrawType);
}

void CImageBoardView::OnUpdateCurve(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_CURVE == m_nDrawType);
}

void CImageBoardView::OnUpdateLine(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_LINE == m_nDrawType);
}

void CImageBoardView::OnUpdateFreeline(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_FREELINE == m_nDrawType);
}

void CImageBoardView::OnUpdateRectH(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_RECT_H == m_nDrawType);
}

void CImageBoardView::OnUpdateRectF(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_RECT_F == m_nDrawType);
}

void CImageBoardView::OnUpdateEllipseH(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_ELLIP_H == m_nDrawType);
}

void CImageBoardView::OnUpdateEllipseF(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_ELLIP_F == m_nDrawType);
}

void CImageBoardView::OnUpdateFill(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_FILL == m_nDrawType);
}

void CImageBoardView::OnUpdateText(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_TEXT == m_nDrawType);
}

void CImageBoardView::OnUpdateRoundrectF(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_ROUNDRECT_F == m_nDrawType);
}

void CImageBoardView::OnUpdateRoundrectH(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_ROUNDRECT_H == m_nDrawType);
}

void CImageBoardView::OnUpdateEraser(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_ERASER == m_nDrawType);
}

void CImageBoardView::OnUpdatePicker(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_PICKER == m_nDrawType);
}

void CImageBoardView::OnSelect() 
{
	SetDrawType(DT_SELECT);
}

void CImageBoardView::OnEraser() 
{
	SetDrawType(DT_ERASER);
}

void CImageBoardView::OnPicker() 
{
	SetDrawType(DT_PICKER);
}

void CImageBoardView::OnCurve() 
{
	SetDrawType(DT_CURVE);
}

void CImageBoardView::OnLine() 
{
	SetDrawType(DT_LINE);
}

void CImageBoardView::OnFreeline() 
{
	SetDrawType(DT_FREELINE);
}

void CImageBoardView::OnRectH() 
{
	SetDrawType(DT_RECT_H);
}

void CImageBoardView::OnRectF() 
{
	SetDrawType(DT_RECT_F);
}

void CImageBoardView::OnRoundrectF() 
{
	SetDrawType(DT_ROUNDRECT_F);
}

void CImageBoardView::OnRoundrectH() 
{
	SetDrawType(DT_ROUNDRECT_H);
}

void CImageBoardView::OnEllipseH() 
{
	SetDrawType(DT_ELLIP_H);
}

void CImageBoardView::OnEllipseF() 
{
	SetDrawType(DT_ELLIP_F);
}

void CImageBoardView::OnText() 
{
	SetDrawType(DT_TEXT);
}

void CImageBoardView::OnFill() 
{
	SetDrawType(DT_FILL);
}

void CImageBoardView::OnPencolor() 
{
	CColorDialog dlgColor(m_crPenColor);
	if (dlgColor.DoModal() == IDOK)
	{
		m_crPenColor = dlgColor.GetColor();
		ShowPenColor();
	}
}

void CImageBoardView::OnSolid() 
{
	SetPenStyle(PS_SOLID);
	ShowPenStyle();
}

void CImageBoardView::OnDash() 
{
	SetPenStyle(PS_DASH);
	ShowPenStyle();
}

void CImageBoardView::OnDot() 
{
	SetPenStyle(PS_DOT);
	ShowPenStyle();
}

void CImageBoardView::OnFillcolor() 
{
	CColorDialog dlgColor(m_crFillColor);
	if (dlgColor.DoModal() == IDOK)
	{
		m_crFillColor = dlgColor.GetColor();
		ShowFillColor();
	}
}

void CImageBoardView::OnUpdateDash(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(PS_DASH == m_nPenStyle);
}

void CImageBoardView::OnUpdateDot(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(PS_DOT == m_nPenStyle);
}

void CImageBoardView::OnUpdateSolid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(PS_SOLID == m_nPenStyle);
}

void CImageBoardView::OnPw1() 
{
	SetPenWidth(1);
	ShowPenWidth();
}

void CImageBoardView::OnPw2() 
{
	SetPenWidth(2);
	ShowPenWidth();
}
void CImageBoardView::OnPw3() 
{
	SetPenWidth(3);
	ShowPenWidth();
}
void CImageBoardView::OnPw4() 
{
	SetPenWidth(4);
	ShowPenWidth();
}
void CImageBoardView::OnPw5() 
{
	SetPenWidth(5);
	ShowPenWidth();
}
void CImageBoardView::OnPw6() 
{
	SetPenWidth(6);
	ShowPenWidth();
}
void CImageBoardView::OnPw7() 
{
	SetPenWidth(7);
	ShowPenWidth();
}
void CImageBoardView::OnPw8() 
{
	SetPenWidth(8);
	ShowPenWidth();
}

void CImageBoardView::OnUpdatePw1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(1 == m_nPenWidth);
}

void CImageBoardView::OnUpdatePw2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(2 == m_nPenWidth);
}
void CImageBoardView::OnUpdatePw3(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(3 == m_nPenWidth);
}
void CImageBoardView::OnUpdatePw4(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(4 == m_nPenWidth);
}
void CImageBoardView::OnUpdatePw5(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(5 == m_nPenWidth);
}
void CImageBoardView::OnUpdatePw6(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(6 == m_nPenWidth);
}
void CImageBoardView::OnUpdatePw7(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(7 == m_nPenWidth);
}
void CImageBoardView::OnUpdatePw8(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(8 == m_nPenWidth);
}

void CImageBoardView::OnPwOther() 
{
	CPenWidth dlg(this);

	dlg.m_nPenWidth = m_nPenWidth;
	if (dlg.DoModal() == IDOK)
		SetPenWidth(dlg.m_nPenWidth);
}

void CImageBoardView::OnUpdatePwOther(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(8 < m_nPenWidth);
}

void CImageBoardView::ShowPenColor()
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	pAppFrame->m_wndPaintParamBar.ShowPenColor(m_crPenColor);
}

void CImageBoardView::ShowFillColor()
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	pAppFrame->m_wndPaintParamBar.ShowFillColor(m_crFillColor);
}

void CImageBoardView::ShowPenWidth()
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	pAppFrame->m_wndPaintParamBar.ShowPenWidth(m_nPenWidth);
}

void CImageBoardView::ShowPenStyle()
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	pAppFrame->m_wndPaintParamBar.ShowPenStyle(m_nPenStyle);
}

void CImageBoardView::ShowColorGrid()
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);

	pAppFrame->m_wndPaintParamBar.ShowColorGrid((HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
}

void CImageBoardView::OnPenColorGrid(UINT nID)
{
	HPALETTE hPalette = (HPALETTE)m_pDib->m_pPalette->GetSafeHandle();
	if (hPalette != NULL)
	{
		PALETTEENTRY pe;
		GetPaletteEntries(hPalette, nID-IDC_COLORTABLE_BASE, 1, &pe);

		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		if (pAppFrame->m_wndPaintParamBar.m_nSelectColorMode == PP_FILL_COLOR)
		{
			m_crFillColor = RGB(pe.peRed, pe.peGreen, pe.peBlue);
			ShowFillColor();
		}
		else if (pAppFrame->m_wndPaintParamBar.m_nSelectColorMode == PP_PEN_COLOR)
		{
			m_crPenColor = RGB(pe.peRed, pe.peGreen, pe.peBlue);
			ShowPenColor();
		}
	}
}

void CImageBoardView::OnFillcolor1() 
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	if (pAppFrame->m_wndPaintParamBar.m_nSelectColorMode == PP_FILL_COLOR)
		OnFillcolor();
	else
		pAppFrame->m_wndPaintParamBar.SetSelectColorMode(PP_FILL_COLOR);
}

void CImageBoardView::OnPencolor1() 
{
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	if (pAppFrame->m_wndPaintParamBar.m_nSelectColorMode == PP_PEN_COLOR)
		OnPencolor();
	else
		pAppFrame->m_wndPaintParamBar.SetSelectColorMode(PP_PEN_COLOR);
}

void CImageBoardView::OnFont() 
{
	LOGFONT lf;
	m_pFont->GetLogFont(&lf);
	CFontDialog dlg(&lf, CF_SCREENFONTS);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&lf);
		m_pFont->DeleteObject();
		m_pFont->CreateFontIndirect(&lf);

		if (::IsWindow(m_EditText.m_hWnd))
		{
			CString s;
			m_EditText.GetWindowText(s);

			CClientDC dc(this);
			CFont *pOldFont = dc.SelectObject(m_pFont);
			// needed rectangle
			CRect rect(0,0,1,1);
			dc.DrawText(s, &rect, DT_CALCRECT);
			dc.SelectObject(pOldFont);

			CRect rc;
			m_EditText.GetWindowRect(&rc);
			ScreenToClient(&rc);
			if (rc.Height() < rect.Height())
				rc.bottom = rc.top + rect.Height();
			if (rc.Width() < rect.Width())
				rc.right = rc.left + rect.Width();
			ClientToDib(rc);
			if (rc.bottom > m_pDib->GetHeight())
				rc.bottom = m_pDib->GetHeight();
			if (rc.right > m_pDib->GetWidth())
				rc.right = m_pDib->GetWidth();
			DibToClient(rc);

			// new edit
			m_EditText.SetWindowPos(NULL,0,0,rc.Width(),rc.Height(),SWP_NOMOVE|SWP_NOZORDER);
			m_EditText.SetFont(m_pFont);
			m_EditText.Invalidate();

			// new frame
			m_EditText.GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.InflateRect(3,3);
			InvalidateRect(&rc);
		}
	}
}

void CImageBoardView::OnLeft() 
{
	m_nTextAlign = DT_LEFT;
	if (::IsWindow(m_EditText.m_hWnd))
	{
		CRect rc;
		m_EditText.GetWindowRect(&rc);
		ScreenToClient(&rc);
		CString s;
		m_EditText.GetWindowText(s);

		m_EditText.DestroyWindow();
		m_EditText.Create(WS_VISIBLE|WS_CHILD|ES_LEFT|ES_MULTILINE, 
						  rc, 
						  this, 
						  IDC_EDIT);

		m_EditText.SetWindowText(s);
	}
}

void CImageBoardView::OnCenter() 
{
	m_nTextAlign = DT_CENTER;
	if (::IsWindow(m_EditText.m_hWnd))
	{
		CRect rc;
		m_EditText.GetWindowRect(&rc);
		ScreenToClient(&rc);
		CString s;
		m_EditText.GetWindowText(s);

		m_EditText.DestroyWindow();
		m_EditText.Create(WS_VISIBLE|WS_CHILD|ES_CENTER|ES_MULTILINE, 
						  rc, 
						  this, 
						  IDC_EDIT);

		m_EditText.SetWindowText(s);
	}
}

void CImageBoardView::OnRight() 
{
	m_nTextAlign = DT_RIGHT;
	if (::IsWindow(m_EditText.m_hWnd))
	{
		CRect rc;
		m_EditText.GetWindowRect(&rc);
		ScreenToClient(&rc);
		CString s;
		m_EditText.GetWindowText(s);

		m_EditText.DestroyWindow();
		m_EditText.Create(WS_VISIBLE|WS_CHILD|ES_RIGHT|ES_MULTILINE, 
						  rc, 
						  this, 
						  IDC_EDIT);

		m_EditText.SetWindowText(s);
	}
}

void CImageBoardView::OnUpdateCenter(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_CENTER == m_nTextAlign);
}

void CImageBoardView::OnUpdateLeft(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_LEFT == m_nTextAlign);
}

void CImageBoardView::OnUpdateRight(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(DT_RIGHT == m_nTextAlign);
}

void CImageBoardView::OnEditCopy() 
{
	CopyToClipboard();
}

void CImageBoardView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnEditCut() 
{
	Cut();
}

void CImageBoardView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnEditPaste() 
{
	Paste();
}

void CImageBoardView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB) ||
				   ::IsClipboardFormatAvailable(CF_BITMAP) ||
				   ::IsClipboardFormatAvailable(CF_PALETTE));
}

void CImageBoardView::OnSelectpaste() 
{
	CSelectPasteDlg spDlg(this);

	if (spDlg.DoModal() == IDOK)
	{
		SelectPaste(spDlg.m_nCF);
	}
}

void CImageBoardView::OnUpdateSelectpaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB) ||
				   ::IsClipboardFormatAvailable(CF_BITMAP) ||
				   ::IsClipboardFormatAvailable(CF_PALETTE));
}

void CImageBoardView::OnCrop() 
{
	Crop();

	CImageBoardDoc* pDoc = GetDocument();
	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateCrop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty() && ! m_rcClip.IsRectEmpty());
}


void CImageBoardView::OnRotateAngle() 
{
	// TODO: Add your command handler code here
	CRotateAngle rotateDlg;

	if (rotateDlg.DoModal() == IDOK)
	{
		Rotate(rotateDlg.m_fAngle);

		CImageBoardDoc* pDoc = GetDocument();
		CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}	
}

void CImageBoardView::OnUpdateRotateAngle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnRotate() 
{
	Rotate(90);

	CImageBoardDoc* pDoc = GetDocument();
	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnRotate180() 
{
	Rotate(180);

	CImageBoardDoc* pDoc = GetDocument();
	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateRotate180(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnRotate270() 
{
	Rotate(270);

	CImageBoardDoc* pDoc = GetDocument();
	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateRotate270(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnHmirror() 
{
	Flip(MD_HORZ);

	CImageBoardDoc* pDoc = GetDocument();
	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateHmirror(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnVmirror() 
{
	Flip(MD_VERT);

	CImageBoardDoc* pDoc = GetDocument();
	CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateVmirror(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnScale() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CScale scaleDlg(m_pDib->GetWidth(), m_pDib->GetHeight(), this);

	if (scaleDlg.DoModal() == IDOK)
	{
		ChangeImageSize(scaleDlg.m_nWidth, scaleDlg.m_nHeight);

		CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}	
}

void CImageBoardView::OnUpdateScale(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnCanvassize() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CCanvasSize canvasDlg((int)m_pDib->GetWidth(), (int)m_pDib->GetHeight(), this);

	if (canvasDlg.DoModal() == IDOK)
	{
		ChangeCanvasSize(canvasDlg.m_nWidth, canvasDlg.m_nHeight, canvasDlg.m_nPosition);

		CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}	
}

void CImageBoardView::OnUpdateCanvassize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}


void CImageBoardView::OnAdjustcolor() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CAdjustColorDlg colorDlg(this);

	if (colorDlg.DoModal() == IDOK)
	{
		m_pDib->AdjustColor(colorDlg.m_nColorModel, 
								  colorDlg.m_nValue1, 
							      colorDlg.m_nValue2, 
							      colorDlg.m_nValue3); 
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateAdjustcolor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnBc() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CBCDlg bcDlg(this);

	if (bcDlg.DoModal() == IDOK)
	{
		m_pDib->AdjustBrightness(bcDlg.m_nValue1);
		m_pDib->AdjustContrast(bcDlg.m_nValue2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateBc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnColorbits() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CColorBitsDlg colorBitsDlg((int)m_pDib->GetBitCount(), this);

	if (colorBitsDlg.DoModal() == IDOK)
	{
	 	m_pDib->ColorQuantize(colorBitsDlg.m_nColorBits);

		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateColorbits(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnGray() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CGrayDlg grayDlg(this);

	if (grayDlg.DoModal() == IDOK)
	{
		m_pDib->ChangeToGrayscale(grayDlg.m_nMethod, 
							  grayDlg.m_fWeightR, 
							  grayDlg.m_fWeightG, 
							  grayDlg.m_fWeightB);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateGray(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	DeleteFloatWnd();
	delete m_pFont;
}

void CImageBoardView::CreateFloatWnd(HDIB hDib, CPoint ptTopLeft)
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

void CImageBoardView::DeleteFloatWnd()
{
	if (m_pFloatWnd)
	{
		delete m_pFloatWnd;
		m_pFloatWnd = NULL;
	}
}

void CImageBoardView::ClientToDib(CPoint& point)
{
	point.x += GetScrollPos(SB_HORZ);
	point.y += GetScrollPos(SB_VERT);
}

void CImageBoardView::ClientToDib(CRect& rect)
{
	rect.left += GetScrollPos(SB_HORZ);
	rect.top += GetScrollPos(SB_VERT);
	rect.right += GetScrollPos(SB_HORZ);
	rect.bottom += GetScrollPos(SB_VERT);
}

void CImageBoardView::DibToClient(CPoint& point)
{
	point.x -= GetScrollPos(SB_HORZ);
	point.y -= GetScrollPos(SB_VERT);
}

void CImageBoardView::DibToClient(CRect& rect)
{
	rect.left -= GetScrollPos(SB_HORZ);
	rect.top -= GetScrollPos(SB_VERT);
	rect.right -= GetScrollPos(SB_HORZ);
	rect.bottom -= GetScrollPos(SB_VERT);
}

BOOL CImageBoardView::AdjustPointinDib(CPoint& point)
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

void CImageBoardView::DoDrawRubber(CDC *pDC, CRect rc)
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

void CImageBoardView::StartDrawRubber(CPoint point)
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

void CImageBoardView::DrawRubber(CPoint point)
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

void CImageBoardView::StopDrawRubber()
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

BOOL CImageBoardView::PointInDib(CPoint point)
{
	if (m_pDib->IsEmpty())
		return FALSE;

	ClientToDib(point);
	CRect rcDib(0, 0, m_pDib->GetWidth(), m_pDib->GetWidth());
	return rcDib.PtInRect(point);
}

BOOL CImageBoardView::MergeText()
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

void CImageBoardView::OnLButtonDown(UINT nFlags, CPoint point) 
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
				ClientToDib(point);
				CDC * pDibDC = m_pDib->BeginPaint(&dc);
				pDibDC->SetPixel(point, m_crPenColor);
				m_pDib->EndPaint();
				Invalidate(FALSE);
			}
			else if (m_nDrawType == DT_ERASER)
			{
				m_bErasing = TRUE;
				SetCapture();

				// set a pixel anyway
				CClientDC dc(this);
				ClientToDib(point);
				CRect rc(point.x-4, point.y-4, point.x+4, point.y+4);
				CDC * pDibDC = m_pDib->BeginPaint(&dc);
				CBrush brush(m_crFillColor);
				CBrush *pOldBrush = pDibDC->SelectObject(&brush);
				CPen Pen(PS_SOLID, 1, m_crFillColor);
				CPen *pOldPen = pDibDC->SelectObject(&Pen);
				pDibDC->Ellipse(&rc);
				pDibDC->SelectObject(pOldPen);
				pDibDC->SelectObject(pOldBrush);
				m_pDib->EndPaint();
				Invalidate(FALSE);
			}
			else if (m_nDrawType == DT_PICKER)
			{
				CClientDC dc(this);
				COLORREF crColor = dc.GetPixel(point);
				CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
				ASSERT_KINDOF(CMainFrame, pAppFrame);
				if (pAppFrame->m_wndPaintParamBar.m_nSelectColorMode == PP_FILL_COLOR)
				{
					m_crFillColor = crColor;
					ShowFillColor();
				}
				else if (pAppFrame->m_wndPaintParamBar.m_nSelectColorMode == PP_PEN_COLOR)
				{
					m_crPenColor = crColor;
					ShowPenColor();
				}
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

void CImageBoardView::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetStatusBarCursorPosition(point);	

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
	if (m_bDrawFreeline)
	{
		CPen pen(m_nPenStyle, m_nPenWidth, m_crPenColor);

		CClientDC dc(this);

		CPoint pt = point;
		ClientToDib(m_ptFreelineStart);
		ClientToDib(pt);
		CDC * pDibDC = m_pDib->BeginPaint(&dc);
		CPen* pOldPen = pDibDC->SelectObject(&pen);
		pDibDC->MoveTo(m_ptFreelineStart);
		pDibDC->LineTo(pt);
		pDibDC->SelectObject(pOldPen);
		m_pDib->EndPaint();
		Invalidate(FALSE);

		// new start point
		m_ptFreelineStart = point;
	}
	else if (m_bErasing)
	{
		// set a pixel anyway
		CClientDC dc(this);
		ClientToDib(point);
		CRect rc(point.x-4, point.y-4, point.x+4, point.y+4);
		CDC * pDibDC = m_pDib->BeginPaint(&dc);
		CBrush brush(m_crFillColor);
		CBrush *pOldBrush = pDibDC->SelectObject(&brush);
		CPen Pen(PS_SOLID, 1, m_crFillColor);
		CPen *pOldPen = pDibDC->SelectObject(&Pen);
		pDibDC->Ellipse(&rc);
		pDibDC->SelectObject(pOldPen);
		pDibDC->SelectObject(pOldBrush);
		m_pDib->EndPaint();
		Invalidate(FALSE);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CImageBoardView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDrawFreeline)
	{
		m_bDrawFreeline = FALSE;
		ReleaseCapture();
	}
	else if (m_bErasing)
	{
		m_bErasing = FALSE;
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

void CImageBoardView::DrawTmpCurve()
{
	CPen penCurve(PS_SOLID, m_nPenWidth, m_crPenColor);

	CClientDC dc(this);
	CPen* pOldPen = dc.SelectObject(&penCurve);
	int nOldRop = dc.SetROP2(R2_NOTXORPEN);
	dc.PolyBezier(m_ptCurve, 4);
	dc.SelectObject(pOldPen);
	dc.SetROP2(nOldRop);
}

void CImageBoardView::DrawCurve()
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

void CImageBoardView::CutSelectedRect()
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

void CImageBoardView::MergeFloatDib()
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

void CImageBoardView::DeleteFloatDib()
{
	// if selected rect is exist, cut it
	CutSelectedRect();
	// if float DIB window exist, delete it
	DeleteFloatWnd();
}

void CImageBoardView::CutFloatDib()
{
	// copy to clipboard first
	CopyToClipboard();
	// then delete
	DeleteFloatDib();
}

void CImageBoardView::CopyToClipboard()
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

void CImageBoardView::Cut()
{
	// copy to clipboard first
	CopyToClipboard();
	// then delete
	Delete();
}

void CImageBoardView::Delete()
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

BOOL CImageBoardView::PasteDibFromClipboard()
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
		if (AdjustSize(hNewDIB))
		{
			// empty current rectangle
			m_rcClip.SetRectEmpty();
			CreateFloatWnd(hNewDIB, CPoint(0, 0));
		}
	}

	return TRUE;
}

BOOL CImageBoardView::PasteBitmapFromClipboard()
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
		if (AdjustSize(hDib))
		{
			HDIB hNewDib = ConvertDIBFormat(hDib, m_pDib->GetBitCount(), (HPALETTE)m_pDib->m_pPalette->GetSafeHandle());
			DestroyDIB(hDib);
			CreateFloatWnd(hNewDib, CPoint(0, 0));
		}
	}

	return TRUE;
}

BOOL CImageBoardView::PastePalFromClipboard()
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

void CImageBoardView::SelectPaste(int nCF)
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

void CImageBoardView::Paste()
{
	if (! PasteDibFromClipboard())
		if (! PasteBitmapFromClipboard())
			PastePalFromClipboard();
}

LRESULT CImageBoardView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);

	if (m_pDib->IsEmpty())
		return 0L;  // must be a new document

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
			TRACE0("\tSelectPalette failed in CImageBoardView::OnPaletteChanged\n");
		}
	}

	return 0L;
}

void CImageBoardView::Rotate(double fAngle)
{
	if (m_pFloatWnd)
	{
		LPBYTE lpDIB = (LPBYTE)GlobalLock(m_pFloatWnd->m_hDibFloat);
		CDib dib;
		if (! dib.Create(lpDIB))
			return;
		GlobalUnlock(m_pFloatWnd->m_hDibFloat);
		if (fAngle == 90.0)
			dib.Rotate90();
		else if (fAngle == 180.0)
			dib.Rotate180();
		else if (fAngle == 270.0)
			dib.Rotate270();
		else
			dib.Rotate(fAngle, RGB(0,0,0));

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
		if (fAngle == 90.0)
			m_pDib->Rotate90();
		else if (fAngle == 180.0)
			m_pDib->Rotate180();
		else if (fAngle == 270.0)
			m_pDib->Rotate270();
		else
			m_pDib->Rotate(fAngle, RGB(0,0,0));
	}
}

void CImageBoardView::Flip(int nDirection)
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

void CImageBoardView::Zoom(double fRatioX, double fRatioY)
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

void CImageBoardView::ChangeImageSize(int nWidth, int nHeight)
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

void CImageBoardView::ChangeCanvasSize(int nWidth, int nHeight, int nPosition)
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

void CImageBoardView::Crop()
{
	if (! m_rcClip.IsRectEmpty())
	{
		m_pDib->Crop(m_rcClip);
		DeleteFloatWnd();
		m_rcClip.SetRectEmpty();
	}
}


HBRUSH CImageBoardView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->m_hWnd == m_EditText.m_hWnd)
		pDC->SetTextColor(m_crPenColor);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

int CImageBoardView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_hCursorFreeline = AfxGetApp()->LoadCursor(IDC_CURSORFREELINE);
	m_hCursorEraser = AfxGetApp()->LoadCursor(IDC_CURSORERASER);
	m_hCursorPicker = AfxGetApp()->LoadCursor(IDC_CURSORPICKER);
	m_hCursorFill = AfxGetApp()->LoadCursor(IDC_CURSORFILL);
	m_hCursorCurve = AfxGetApp()->LoadCursor(IDC_CURSORCURVE);
	
	LOGFONT lf;
	HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	m_pFont = new CFont;
	m_pFont->CreateFontIndirect(&lf);
	
	return 0;
}

BOOL CImageBoardView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_nDrawType == DT_FREELINE)
	{
		::SetCursor(m_hCursorFreeline);
		return TRUE;
	}
	else if (m_nDrawType == DT_ERASER)
	{
		::SetCursor(m_hCursorEraser);
		return TRUE;
	}
	else if (m_nDrawType == DT_PICKER)
	{
		::SetCursor(m_hCursorPicker);
		return TRUE;
	}
	else if (m_nDrawType == DT_FILL)
	{
		::SetCursor(m_hCursorFill);
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CImageBoardView::OnHorz() 
{
	EdgeEnhance(HORZ);
}

void CImageBoardView::OnUpdateHorz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLap1() 
{
	EdgeEnhance(LAP1);
}

void CImageBoardView::OnUpdateLap1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLap2() 
{
	EdgeEnhance(LAP2);
}

void CImageBoardView::OnUpdateLap2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLap3() 
{
	EdgeEnhance(LAP3);
}

void CImageBoardView::OnUpdateLap3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLap4() 
{
	EdgeEnhance(LAP4);
}

void CImageBoardView::OnUpdateLap4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}


void CImageBoardView::OnVert() 
{
	EdgeEnhance(VERT);
}

void CImageBoardView::OnUpdateVert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnVerthorz() 
{
	EdgeEnhance(VERTHORZ);
}

void CImageBoardView::OnUpdateVerthorz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}


void CImageBoardView::OnHf1() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->HighPass(FD.m_nStreng, FILTER1);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateHf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnHf2() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->HighPass(FD.m_nStreng, FILTER2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateHf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnHf3() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->HighPass(FD.m_nStreng, FILTER3);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateHf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLf1() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->LowPass(FD.m_nStreng, FILTER1);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateLf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLf2() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->LowPass(FD.m_nStreng, FILTER2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateLf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnLf3() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->LowPass(FD.m_nStreng, FILTER3);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateLf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnEast() 
{
	EdgeEnhance(EAST);
}

void CImageBoardView::OnUpdateEast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnNorth() 
{
	EdgeEnhance(NORTH);
}

void CImageBoardView::OnUpdateNorth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnNortheast() 
{
	EdgeEnhance(NORTHEAST);
}

void CImageBoardView::OnUpdateNortheast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnNorthwest() 
{
	EdgeEnhance(NORTHWEST);
}

void CImageBoardView::OnUpdateNorthwest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnSouth() 
{
	EdgeEnhance(SOUTH);
}

void CImageBoardView::OnUpdateSouth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnSoutheast() 
{
	EdgeEnhance(SOUTHEAST);
}

void CImageBoardView::OnUpdateSoutheast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnSouthwest() 
{
	EdgeEnhance(SOUTHWEST);
}

void CImageBoardView::OnUpdateSouthwest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnWest() 
{
	EdgeEnhance(WEST);
}

void CImageBoardView::OnUpdateWest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::EdgeEnhance(int nAlgorithm) 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->EdgeEnhance(FD.m_nStreng, nAlgorithm);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}


void CImageBoardView::OnMedianfilter() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->MedianFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateMedianfilter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}


void CImageBoardView::OnEditClear() 
{
	Delete();
}

void CImageBoardView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnEditSelectAll() 
{
	// adjust position with scroll position
	m_rcClip = CRect(0, 0, m_pDib->GetWidth(), m_pDib->GetHeight());

	// create float DIB
	HDIB hDib = m_pDib->CopyRect(m_rcClip);

	// create new float DIB window
	CreateFloatWnd(hDib, m_rcClip.TopLeft());
}

void CImageBoardView::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	
}

void CImageBoardView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CImageBoardView::OnEditRedo() 
{
	// TODO: Add your command handler code here
	
}

void CImageBoardView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CImageBoardView::OnRed() 
{
	m_pDib->FilteRGB(FILTE_RED);

	CImageBoardDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateRed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnGreen() 
{
	m_pDib->FilteRGB(FILTE_GREEN);

	CImageBoardDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateGreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnBlue() 
{
	m_pDib->FilteRGB(FILTE_BLUE);

	CImageBoardDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateBlue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnGetblue() 
{
	m_pDib->SeparateRGB(SEPARATE_BLUE);

	CImageBoardDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateGetblue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnGetgreen() 
{
	m_pDib->SeparateRGB(SEPARATE_GREEN);

	CImageBoardDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateGetgreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnGetred() 
{
	m_pDib->SeparateRGB(SEPARATE_RED);

	CImageBoardDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateGetred(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnCopyto() 
{
	HDIB hDib;
	// if there is float dib, copy it
	if (m_pFloatWnd)
		hDib = CopyHandle(m_pFloatWnd->m_hDibFloat);
	else	// otherwise, copy the entire DIB
		hDib = CopyHandle(m_pDib->m_hDib);

	CDib dib;
	if (! dib.Attach(hDib))
		return;

	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szSaveFilter);

	switch (dib.GetBitCount())
	{
	case 1:
		FileDlg.m_ofn.nFilterIndex = 1;
		break;
	case 4:
		FileDlg.m_ofn.nFilterIndex = 2;
		break;
	case 8:
		FileDlg.m_ofn.nFilterIndex = 3;
		break;
	case 24:
	default:
		FileDlg.m_ofn.nFilterIndex = 4;
		break;
	}

	if (FileDlg.DoModal() == IDOK)
	{
		switch (FileDlg.m_ofn.nFilterIndex)
		{
		case 1:
			dib.ConvertFormat(1);
			break;
		case 2:
			dib.ConvertFormat(4);
			break;
		case 3:
			dib.ConvertFormat(8);
			break;
		case 4:
		default:
			dib.ConvertFormat(24);
			break;
		}
		dib.Save(FileDlg.m_ofn.lpstrFile);
	}
}

void CImageBoardView::OnUpdateCopyto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnPastefrom() 
{
	CFileDialog FileDlg(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		CDib dib;
		if (dib.Load(FileDlg.m_ofn.lpstrFile))
		{
			// adjust position with scroll position
			CRect rc(0, 0, dib.GetWidth(), dib.GetHeight());

			// create float DIB
			HDIB hDib = CopyHandle(dib.GetHandle());

			if (AdjustSize(hDib))
				// create new float DIB window
				CreateFloatWnd(hDib, rc.TopLeft());
		}
	}
}

void CImageBoardView::OnUpdatePastefrom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

BOOL CImageBoardView::AdjustSize(HDIB hDib)
{
	int nWidth = max((int)DIBWidth(hDib), (int)m_pDib->GetWidth());
	int nHeight = max((int)DIBHeight(hDib), (int)m_pDib->GetHeight());
	if (nWidth > (int)m_pDib->GetWidth() ||
		nHeight > (int)m_pDib->GetHeight())
	{
		CImageBoardDoc* pDoc = GetDocument();
		switch (AfxMessageBox(IDS_ADJUSTSIZE, MB_YESNOCANCEL))
		{
		case IDYES:
			{
				m_pDib->ChangeCanvasSize(nWidth, nHeight, LEFT_UP);
				CSize sizeTotal(m_pDib->GetWidth(), m_pDib->GetHeight());
				SetScrollSizes(MM_TEXT, sizeTotal);
				pDoc->SetModifiedFlag(TRUE);
				pDoc->UpdateAllViews(NULL);
				return TRUE;
			}
		case IDNO:
			return TRUE;
		case IDCANCEL:
			return FALSE;
		}
	}

	return TRUE;
}



void CImageBoardView::OnErosionH() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Erosion(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnErosionV() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Erosion(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateErosionH(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnUpdateErosionV(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnDilationH() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Erosion(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateDilationH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnDilationV() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Dilation(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateDilationV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnCloseH() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->MorphClose(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateCloseH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnCloseV() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->MorphClose(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateCloseV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnOpenH() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->MorphOpen(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateOpenH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnOpenV() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->MorphOpen(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateOpenV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnContourH() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Contour(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateContourH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnContourV() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Contour(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateContourV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnThin() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->Thinning();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateThin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnFft() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->FFT();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnDct() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->DCT();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateDct(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnWalh() 
{
	// TODO: Add your command handler code here
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->WALh();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageBoardView::OnUpdateWalh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
	
}

void CImageBoardView::OnCorrectgamma() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CGammaCorrection gammaDlg(this);

	if (gammaDlg.DoModal() == IDOK)
	{
		m_pDib->CorrectGamma(gammaDlg.m_fGamma);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateCorrectgamma(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnDifferenceedgedetect() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->DifferenceEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateDifferenceedgedetect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnEmbossment() 
{
	CEmbossmentDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->Embossment(dlg.m_nValue);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateEmbossment(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnExposure() 
{
	CImageBoardDoc* pDoc = GetDocument();
	CExposureDlg exposureDlg(this);

	if (exposureDlg.DoModal() == IDOK)
	{
		m_pDib->Exposure(exposureDlg.m_nThreshold);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateExposure(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnHomoedgedetect() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->HomogenityEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateHomoedgedetect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnNegative() 
{
	CImageBoardDoc* pDoc = GetDocument();
	pDoc->m_pDib->Negative();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateNegative(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnSobeled() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->EdegeDetect(SOBEL);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateSobeled(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnSpread() 
{
	CSpreadDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CImageBoardDoc* pDoc = GetDocument();
		m_pDib->Spread(dlg.m_nValue);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageBoardView::OnUpdateSpread(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnHoughed() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->EdegeDetect(HOUGH);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateHoughed(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! m_pDib->IsEmpty());
}

void CImageBoardView::OnCannyed() 
{
	CImageBoardDoc* pDoc = GetDocument();
	m_pDib->CannyEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageBoardView::OnUpdateCannyed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! m_pDib->IsEmpty());
}
