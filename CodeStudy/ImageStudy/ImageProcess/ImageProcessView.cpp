// ImageProcessView.cpp : implementation of the CImageProcessView class
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "MainFrm.h"

#include "ConvoluteDlg.h"
#include "EmbossmentDlg.h"
#include "SpreadDlg.h"
#include "ImageProcessDoc.h"
#include "ImageProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessView)
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
	ON_COMMAND(IDM_SOBEL, OnSobel)
	ON_UPDATE_COMMAND_UI(IDM_SOBEL, OnUpdateSobel)
	ON_COMMAND(IDM_HOUGH, OnHough)
	ON_UPDATE_COMMAND_UI(IDM_HOUGH, OnUpdateHough)
	ON_COMMAND(IDM_MEDIANFILTER, OnMedianfilter)
	ON_UPDATE_COMMAND_UI(IDM_MEDIANFILTER, OnUpdateMedianfilter)
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
	ON_UPDATE_COMMAND_UI(IDM_REVERSE, OnUpdateReverse)
	ON_COMMAND(IDM_WALH, OnWalh)
	ON_UPDATE_COMMAND_UI(IDM_WALH, OnUpdateWalh)
	ON_COMMAND(IDM_CONTOUR_H, OnContourH)
	ON_UPDATE_COMMAND_UI(IDM_CONTOUR_H, OnUpdateContourH)
	ON_COMMAND(IDM_CONTOUR_V, OnContourV)
	ON_UPDATE_COMMAND_UI(IDM_CONTOUR_V, OnUpdateContourV)
	ON_COMMAND(IDM_EMBOSSMENT, OnEmbossment)
	ON_UPDATE_COMMAND_UI(IDM_EMBOSSMENT, OnUpdateEmbossment)
	ON_COMMAND(IDM_SPREAD, OnSpread)
	ON_UPDATE_COMMAND_UI(IDM_SPREAD, OnUpdateSpread)
	ON_COMMAND(IDM_HOMOEDGEDETECT, OnHomoedgedetect)
	ON_UPDATE_COMMAND_UI(IDM_HOMOEDGEDETECT, OnUpdateHomoedgedetect)
	ON_COMMAND(IDM_DIFFERENTEDGEDETECT, OnDifferentedgedetect)
	ON_UPDATE_COMMAND_UI(IDM_DIFFERENTEDGEDETECT, OnUpdateDifferentedgedetect)
	ON_COMMAND(IDM_CANNYED, OnCannyed)
	ON_UPDATE_COMMAND_UI(IDM_CANNYED, OnUpdateCannyed)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView construction/destruction

CImageProcessView::CImageProcessView()
{
	// TODO: add construction code here

}

CImageProcessView::~CImageProcessView()
{
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView drawing

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 0, 0);
}

void CImageProcessView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CImageProcessDoc* pDoc = GetDocument();

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
// CImageProcessView printing

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView diagnostics

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView message handlers

LRESULT CImageProcessView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CImageProcessDoc* pDoc = GetDocument();

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


void CImageProcessView::OnHorz() 
{
	EdgeEnhance(HORZ);
}

void CImageProcessView::OnUpdateHorz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap1() 
{
	EdgeEnhance(LAP1);
}

void CImageProcessView::OnUpdateLap1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap2() 
{
	EdgeEnhance(LAP2);
}

void CImageProcessView::OnUpdateLap2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap3() 
{
	EdgeEnhance(LAP3);
}

void CImageProcessView::OnUpdateLap3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap4() 
{
	EdgeEnhance(LAP4);
}

void CImageProcessView::OnUpdateLap4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnVert() 
{
	EdgeEnhance(VERT);
}

void CImageProcessView::OnUpdateVert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnVerthorz() 
{
	EdgeEnhance(VERTHORZ);
}

void CImageProcessView::OnUpdateVerthorz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnHf1() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->HighPass(FD.m_nStreng, FILTER1);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateHf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHf2() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->HighPass(FD.m_nStreng, FILTER2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateHf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHf3() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->HighPass(FD.m_nStreng, FILTER3);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateHf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLf1() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER1);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateLf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLf2() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateLf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLf3() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER3);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateLf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnEast() 
{
	EdgeEnhance(EAST);
}

void CImageProcessView::OnUpdateEast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnNorth() 
{
	EdgeEnhance(NORTH);
}

void CImageProcessView::OnUpdateNorth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnNortheast() 
{
	EdgeEnhance(NORTHEAST);
}

void CImageProcessView::OnUpdateNortheast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnNorthwest() 
{
	EdgeEnhance(NORTHWEST);
}

void CImageProcessView::OnUpdateNorthwest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSouth() 
{
	EdgeEnhance(SOUTH);
}

void CImageProcessView::OnUpdateSouth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSoutheast() 
{
	EdgeEnhance(SOUTHEAST);
}

void CImageProcessView::OnUpdateSoutheast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSouthwest() 
{
	EdgeEnhance(SOUTHWEST);
}

void CImageProcessView::OnUpdateSouthwest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnWest() 
{
	EdgeEnhance(WEST);
}

void CImageProcessView::OnUpdateWest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSobel() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->EdegeDetect(SOBEL);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateSobel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHough() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->EdegeDetect(HOUGH);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateHough(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::EdgeEnhance(int nAlgorithm) 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->EdgeEnhance(FD.m_nStreng, nAlgorithm);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}


void CImageProcessView::OnMedianfilter() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MedianFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateMedianfilter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}



void CImageProcessView::OnErosionH() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Erosion(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnErosionV() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Erosion(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateErosionH(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnUpdateErosionV(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnDilationH() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Erosion(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateDilationH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnDilationV() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Dilation(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateDilationV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnCloseH() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphClose(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateCloseH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnCloseV() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphClose(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateCloseV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnOpenH() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphOpen(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateOpenH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnOpenV() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphOpen(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateOpenV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnThin() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Thinning();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateThin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnUpdateReverse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnFft() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FFT();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnDct() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->DCT();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateDct(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnWalh() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->WALh();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateWalh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnContourH() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Contour(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateContourH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnContourV() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Contour(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateContourV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnEmbossment() 
{
	CEmbossmentDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->Embossment(dlg.m_nValue);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateEmbossment(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSpread() 
{
	CSpreadDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->Spread(dlg.m_nValue);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateSpread(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHomoedgedetect() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->HomogenityEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateHomoedgedetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnDifferentedgedetect() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->DifferenceEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateDifferentedgedetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnCannyed() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->CannyEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateCannyed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}
