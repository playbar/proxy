// TransformShowView.cpp : implementation of the CTransformShowView class
//

#include "stdafx.h"
#include "TransformShow.h"
#include "MainFrm.h"
#include "Dib.h"
#include "CanvasDlg.h"
#include "ScaleDlg.h"
#include "RotateAngle.h"
#include "TransformShowDoc.h"
#include "TransformShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransformShowView

IMPLEMENT_DYNCREATE(CTransformShowView, CScrollView)

BEGIN_MESSAGE_MAP(CTransformShowView, CScrollView)
	//{{AFX_MSG_MAP(CTransformShowView)
	ON_COMMAND(IDM_CANVASSIZE, OnCanvassize)
	ON_UPDATE_COMMAND_UI(IDM_CANVASSIZE, OnUpdateCanvassize)
	ON_COMMAND(IDM_CROP, OnCrop)
	ON_UPDATE_COMMAND_UI(IDM_CROP, OnUpdateCrop)
	ON_COMMAND(IDM_HMIRROR, OnHmirror)
	ON_UPDATE_COMMAND_UI(IDM_HMIRROR, OnUpdateHmirror)
	ON_COMMAND(IDM_ROTATE, OnRotate)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE, OnUpdateRotate)
	ON_COMMAND(IDM_ROTATE180, OnRotate180)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE180, OnUpdateRotate180)
	ON_COMMAND(IDM_ROTATE270, OnRotate270)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE270, OnUpdateRotate270)
	ON_COMMAND(IDM_SCALE, OnScale)
	ON_UPDATE_COMMAND_UI(IDM_SCALE, OnUpdateScale)
	ON_COMMAND(IDM_VMIRROR, OnVmirror)
	ON_UPDATE_COMMAND_UI(IDM_VMIRROR, OnUpdateVmirror)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(IDM_ROTATE_ANGLE, OnRotateAngle)
	ON_UPDATE_COMMAND_UI(IDM_ROTATE_ANGLE, OnUpdateRotateAngle)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransformShowView construction/destruction

CTransformShowView::CTransformShowView()
{
	// TODO: add construction code here

}

CTransformShowView::~CTransformShowView()
{
}

BOOL CTransformShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTransformShowView drawing

void CTransformShowView::OnDraw(CDC* pDC)
{
	CTransformShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 0, 0);
}

void CTransformShowView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CTransformShowDoc* pDoc = GetDocument();

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
// CTransformShowView printing

BOOL CTransformShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransformShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransformShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTransformShowView diagnostics

#ifdef _DEBUG
void CTransformShowView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTransformShowView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTransformShowDoc* CTransformShowView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransformShowDoc)));
	return (CTransformShowDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTransformShowView message handlers

LRESULT CTransformShowView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CTransformShowDoc* pDoc = GetDocument();

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

void CTransformShowView::OnCanvassize() 
{
	CTransformShowDoc* pDoc = GetDocument();
	CCanvasDlg canvasDlg((int)pDoc->m_pDib->GetWidth(), (int)pDoc->m_pDib->GetHeight(), this);

	if (canvasDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->ChangeCanvasSize(canvasDlg.m_nWidth, canvasDlg.m_nHeight, canvasDlg.m_nPosition);

		CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}	
}

void CTransformShowView::OnUpdateCanvassize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnCrop() 
{
	CTransformShowDoc* pDoc = GetDocument();
	int nWidth = pDoc->m_pDib->GetWidth();
	int nHeight = pDoc->m_pDib->GetHeight();
	pDoc->m_pDib->Crop(CRect(nWidth/4, nHeight/4, 3*nWidth/4, 3*nHeight/4));

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateCrop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnHmirror() 
{
	CTransformShowDoc* pDoc = GetDocument();
	pDoc->m_pDib->FlipHorz();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateHmirror(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnRotateAngle() 
{
	// TODO: Add your command handler code here
	CRotateAngle rotateDlg;

	if (rotateDlg.DoModal() == IDOK)
	{
		CTransformShowDoc* pDoc = GetDocument();
		pDoc->m_pDib->Rotate(rotateDlg.m_fAngle, RGB(0,0,0));

		CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}	
}

void CTransformShowView::OnUpdateRotateAngle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());

}

void CTransformShowView::OnRotate() 
{
	CTransformShowDoc* pDoc = GetDocument();
	pDoc->m_pDib->Rotate90();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnRotate180() 
{
	CTransformShowDoc* pDoc = GetDocument();
	pDoc->m_pDib->Rotate180();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateRotate180(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnRotate270() 
{
	CTransformShowDoc* pDoc = GetDocument();
	pDoc->m_pDib->Rotate270();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateRotate270(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnScale() 
{
	CTransformShowDoc* pDoc = GetDocument();
	CScaleDlg scaleDlg(this);

	if (scaleDlg.DoModal() == IDOK)
	{
		pDoc->m_pDib->Zoom(scaleDlg.m_fRatioX, scaleDlg.m_fRatioY);

		CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}	
}

void CTransformShowView::OnUpdateScale(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnVmirror() 
{
	CTransformShowDoc* pDoc = GetDocument();
	pDoc->m_pDib->FlipVert();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateVmirror(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CTransformShowView::OnEditCut() 
{
	CTransformShowDoc* pDoc = GetDocument();
	int nWidth = pDoc->m_pDib->GetWidth();
	int nHeight = pDoc->m_pDib->GetHeight();
	pDoc->m_pDib->CutRect(CRect(nWidth/4, nHeight/4, 3*nWidth/4, 3*nHeight/4));

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}

void CTransformShowView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

