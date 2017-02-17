// ColorProcessView.cpp : implementation of the CColorProcessView class
//

#include "stdafx.h"
#include "ColorProcess.h"
#include "MainFrm.h"
#include "GrayDlg.h"
#include "BCDlg.h"
#include "AdjustColorDlg.h"
#include "ColorBitsDlg.h"
#include "GammaCorrection.h"
#include "ExposureDlg.h"
#include "ColorProcessDoc.h"
#include "ColorProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorProcessView

IMPLEMENT_DYNCREATE(CColorProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CColorProcessView, CScrollView)
	//{{AFX_MSG_MAP(CColorProcessView)
	ON_COMMAND(IDM_ADJUSTCOLOR, OnAdjustcolor)
	ON_UPDATE_COMMAND_UI(IDM_ADJUSTCOLOR, OnUpdateAdjustcolor)
	ON_COMMAND(IDM_BC, OnBc)
	ON_UPDATE_COMMAND_UI(IDM_BC, OnUpdateBc)
	ON_COMMAND(IDM_COLORBITS, OnColorbits)
	ON_UPDATE_COMMAND_UI(IDM_COLORBITS, OnUpdateColorbits)
	ON_COMMAND(IDM_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(IDM_GRAY, OnUpdateGray)
	ON_COMMAND(IDM_BLUE, OnBlue)
	ON_UPDATE_COMMAND_UI(IDM_BLUE, OnUpdateBlue)
	ON_COMMAND(IDM_GETBLUE, OnGetblue)
	ON_UPDATE_COMMAND_UI(IDM_GETBLUE, OnUpdateGetblue)
	ON_COMMAND(IDM_GETGREEN, OnGetgreen)
	ON_UPDATE_COMMAND_UI(IDM_GETGREEN, OnUpdateGetgreen)
	ON_COMMAND(IDM_GETRED, OnGetred)
	ON_UPDATE_COMMAND_UI(IDM_GETRED, OnUpdateGetred)
	ON_COMMAND(IDM_GREEN, OnGreen)
	ON_UPDATE_COMMAND_UI(IDM_GREEN, OnUpdateGreen)
	ON_COMMAND(IDM_RED, OnRed)
	ON_UPDATE_COMMAND_UI(IDM_RED, OnUpdateRed)
	ON_COMMAND(IDM_CORRECTGAMMA, OnCorrectgamma)
	ON_UPDATE_COMMAND_UI(IDM_CORRECTGAMMA, OnUpdateCorrectgamma)
	ON_COMMAND(IDM_NEGATIVE, OnNegative)
	ON_UPDATE_COMMAND_UI(IDM_NEGATIVE, OnUpdateNegative)
	ON_COMMAND(IDM_EXPOSURE, OnExposure)
	ON_UPDATE_COMMAND_UI(IDM_EXPOSURE, OnUpdateExposure)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorProcessView construction/destruction

CColorProcessView::CColorProcessView()
{
	// TODO: add construction code here

}

CColorProcessView::~CColorProcessView()
{
}

BOOL CColorProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CColorProcessView drawing

void CColorProcessView::OnDraw(CDC* pDC)
{
	CColorProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 0, 0);
}

void CColorProcessView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CColorProcessDoc* pDoc = GetDocument();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	CRect rc;
	pAppFrame->GetClientRect(&rc);
	if (rc.Width() >= sizeTotal.cx && rc.Height() >= sizeTotal.cy &&
		(sizeTotal.cx>0 || sizeTotal.cy>0))
		ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CColorProcessView printing

BOOL CColorProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CColorProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CColorProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CColorProcessView diagnostics

#ifdef _DEBUG
void CColorProcessView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CColorProcessView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CColorProcessDoc* CColorProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorProcessDoc)));
	return (CColorProcessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColorProcessView message handlers

LRESULT CColorProcessView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CColorProcessDoc* pDoc = GetDocument();

	if (pDoc->m_pDib->IsEmpty())
		return 0L;  // must be a new document

	CPalette* pPal = pDoc->m_pDib->GetPalette();
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
			TRACE0("\tSelectPalette failed!\n");
		}
	}

	return 0L;
}

void CColorProcessView::OnAdjustcolor() 
{
	CColorProcessDoc* pDoc = GetDocument();
	CAdjustColorDlg colorDlg(this);

	if (colorDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->AdjustColor(colorDlg.m_nColorModel, 
								  colorDlg.m_nValue1, 
							      colorDlg.m_nValue2, 
							      colorDlg.m_nValue3); 
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CColorProcessView::OnUpdateAdjustcolor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnBc() 
{
	CColorProcessDoc* pDoc = GetDocument();
	CBCDlg bcDlg(this);

	if (bcDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->AdjustBrightness(bcDlg.m_nValue1);
		pDoc->m_pDib->AdjustContrast(bcDlg.m_nValue2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CColorProcessView::OnUpdateBc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnColorbits() 
{
	CColorProcessDoc* pDoc = GetDocument();
	CColorBitsDlg colorBitsDlg((int)pDoc->m_pDib->GetBitCount(), this);

	if (colorBitsDlg.DoModal() == IDOK)
	{
	 	pDoc->m_pDib->ColorQuantize(colorBitsDlg.m_nColorBits);

		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CColorProcessView::OnUpdateColorbits(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnGray() 
{
	CColorProcessDoc* pDoc = GetDocument();
	CGrayDlg grayDlg(this);

	if (grayDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->ChangeToGrayscale(grayDlg.m_nMethod, 
							  grayDlg.m_fWeightR, 
							  grayDlg.m_fWeightG, 
							  grayDlg.m_fWeightB);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CColorProcessView::OnUpdateGray(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnRed() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FilteRGB(FILTE_RED);

	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateRed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnGreen() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FilteRGB(FILTE_GREEN);

	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateGreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnBlue() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FilteRGB(FILTE_BLUE);

	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateBlue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnGetblue() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->SeparateRGB(SEPARATE_BLUE);

	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateGetblue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnGetgreen() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->SeparateRGB(SEPARATE_GREEN);

	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateGetgreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnGetred() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->SeparateRGB(SEPARATE_RED);

	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateGetred(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnCorrectgamma() 
{
	CColorProcessDoc* pDoc = GetDocument();
	CGammaCorrection gammaDlg(this);

	if (gammaDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->CorrectGamma(gammaDlg.m_fGamma);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CColorProcessView::OnUpdateCorrectgamma(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnNegative() 
{
	CColorProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Negative();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CColorProcessView::OnUpdateNegative(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CColorProcessView::OnExposure() 
{
	CColorProcessDoc* pDoc = GetDocument();
	CExposureDlg exposureDlg(this);

	if (exposureDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->Exposure(exposureDlg.m_nThreshold);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CColorProcessView::OnUpdateExposure(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}
