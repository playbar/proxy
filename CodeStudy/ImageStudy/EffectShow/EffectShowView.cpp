// EffectShowView.cpp : implementation of the CEffectShowView class
//

#include "stdafx.h"
#include "EffectShow.h"
#include "MainFrm.h"
#include "Dib.h"

#include "EffectShowDoc.h"
#include "EffectShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEffectShowView

IMPLEMENT_DYNCREATE(CEffectShowView, CScrollView)

BEGIN_MESSAGE_MAP(CEffectShowView, CScrollView)
	//{{AFX_MSG_MAP(CEffectShowView)
	ON_COMMAND(IDM_FADEIN, OnFadein)
	ON_UPDATE_COMMAND_UI(IDM_FADEIN, OnUpdateFadein)
	ON_COMMAND(IDM_HBLIND, OnHblind)
	ON_UPDATE_COMMAND_UI(IDM_HBLIND, OnUpdateHblind)
	ON_COMMAND(IDM_HCROSSMOVE, OnHcrossmove)
	ON_UPDATE_COMMAND_UI(IDM_HCROSSMOVE, OnUpdateHcrossmove)
	ON_COMMAND(IDM_HRASTER, OnHraster)
	ON_UPDATE_COMMAND_UI(IDM_HRASTER, OnUpdateHraster)
	ON_COMMAND(IDM_HSPLITMOVE, OnHsplitmove)
	ON_UPDATE_COMMAND_UI(IDM_HSPLITMOVE, OnUpdateHsplitmove)
	ON_COMMAND(IDM_HSPLITSCAN, OnHsplitscan)
	ON_UPDATE_COMMAND_UI(IDM_HSPLITSCAN, OnUpdateHsplitscan)
	ON_COMMAND(IDM_MOSAIC, OnMosaic)
	ON_UPDATE_COMMAND_UI(IDM_MOSAIC, OnUpdateMosaic)
	ON_COMMAND(IDM_MOVEDOWN, OnMovedown)
	ON_UPDATE_COMMAND_UI(IDM_MOVEDOWN, OnUpdateMovedown)
	ON_COMMAND(IDM_MOVELEFT, OnMoveleft)
	ON_UPDATE_COMMAND_UI(IDM_MOVELEFT, OnUpdateMoveleft)
	ON_COMMAND(IDM_MOVERIGHT, OnMoveright)
	ON_UPDATE_COMMAND_UI(IDM_MOVERIGHT, OnUpdateMoveright)
	ON_COMMAND(IDM_MOVEUP, OnMoveup)
	ON_UPDATE_COMMAND_UI(IDM_MOVEUP, OnUpdateMoveup)
	ON_COMMAND(IDM_SCANDOWN, OnScandown)
	ON_UPDATE_COMMAND_UI(IDM_SCANDOWN, OnUpdateScandown)
	ON_COMMAND(IDM_SCANLEFT, OnScanleft)
	ON_UPDATE_COMMAND_UI(IDM_SCANLEFT, OnUpdateScanleft)
	ON_COMMAND(IDM_SCANRIGHT, OnScanright)
	ON_UPDATE_COMMAND_UI(IDM_SCANRIGHT, OnUpdateScanright)
	ON_COMMAND(IDM_SCANUP, OnScanup)
	ON_UPDATE_COMMAND_UI(IDM_SCANUP, OnUpdateScanup)
	ON_COMMAND(IDM_VBLIND, OnVblind)
	ON_UPDATE_COMMAND_UI(IDM_VBLIND, OnUpdateVblind)
	ON_COMMAND(IDM_VCROSSMOVE, OnVcrossmove)
	ON_UPDATE_COMMAND_UI(IDM_VCROSSMOVE, OnUpdateVcrossmove)
	ON_COMMAND(IDM_VRASTER, OnVraster)
	ON_UPDATE_COMMAND_UI(IDM_VRASTER, OnUpdateVraster)
	ON_COMMAND(IDM_VSPLITMOVE, OnVsplitmove)
	ON_UPDATE_COMMAND_UI(IDM_VSPLITMOVE, OnUpdateVsplitmove)
	ON_COMMAND(IDM_VSPLITSCAN, OnVsplitscan)
	ON_UPDATE_COMMAND_UI(IDM_VSPLITSCAN, OnUpdateVsplitscan)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEffectShowView construction/destruction

CEffectShowView::CEffectShowView()
{
	m_nEffect = -1;
}

CEffectShowView::~CEffectShowView()
{
}

BOOL CEffectShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEffectShowView drawing

void CEffectShowView::OnDraw(CDC* pDC)
{
	CEffectShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 0, 0);
}

void CEffectShowView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CEffectShowDoc * pDoc = GetDocument();

	CSize sizeTotal(pDoc->m_pDib->GetWidth(), 
					pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	CMainFrame* pAppFrame = 
					(CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	CRect rc;
	pAppFrame->GetClientRect(&rc);
	if (rc.Width() >= sizeTotal.cx && rc.Height() >= sizeTotal.cy &&
		(sizeTotal.cx>0 || sizeTotal.cy>0))
		ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEffectShowView printing

BOOL CEffectShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEffectShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEffectShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEffectShowView diagnostics

#ifdef _DEBUG
void CEffectShowView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEffectShowView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEffectShowDoc* CEffectShowView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectShowDoc)));
	return (CEffectShowDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEffectShowView message handlers

LRESULT CEffectShowView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CEffectShowDoc* pDoc = GetDocument();

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

void CEffectShowView::OnHblind() 
{
	m_nEffect = EFFECT_HBLIND;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateHblind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnHcrossmove() 
{
	m_nEffect = EFFECT_HCROSSMOVE;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateHcrossmove(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnHraster() 
{
	m_nEffect = EFFECT_HRASTER;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateHraster(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnHsplitmove() 
{
	m_nEffect = EFFECT_HSPLITMOVE;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateHsplitmove(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnHsplitscan() 
{
	m_nEffect = EFFECT_HSPLITSCAN;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateHsplitscan(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnMosaic() 
{
	m_nEffect = EFFECT_MOSAIC;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateMosaic(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnMovedown() 
{
	m_nEffect = EFFECT_MOVEDOWN;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateMovedown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnMoveleft() 
{
	m_nEffect = EFFECT_MOVELEFT;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateMoveleft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnMoveright() 
{
	m_nEffect = EFFECT_MOVERIGHT;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateMoveright(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnMoveup() 
{
	m_nEffect = EFFECT_MOVEUP;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateMoveup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnScandown() 
{
	m_nEffect = EFFECT_SCANDOWN;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateScandown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnScanleft() 
{
	m_nEffect = EFFECT_SCANLEFT;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateScanleft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnScanright() 
{
	m_nEffect = EFFECT_SCANRIGHT;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateScanright(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnScanup() 
{
	m_nEffect = EFFECT_SCANUP;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateScanup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnVblind() 
{
	m_nEffect = EFFECT_VBLIND;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateVblind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnVcrossmove() 
{
	m_nEffect = EFFECT_VCROSSMOVE;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateVcrossmove(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnVraster() 
{
	m_nEffect = EFFECT_VRASTER;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateVraster(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnVsplitmove() 
{
	m_nEffect = EFFECT_VSPLITMOVE;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateVsplitmove(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnUpdateVsplitscan(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::OnVsplitscan() 
{
	m_nEffect = EFFECT_VSPLITSCAN;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnFadein() 
{
	m_nEffect = EFFECT_FADE;
	EffectDisplay(m_nEffect);
}

void CEffectShowView::OnUpdateFadein(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CEffectShowView::EffectDisplay(int nEffect) 
{
	CEffectShowDoc* pDoc = GetDocument();
	CClientDC dc(this);
	CPoint pt = GetScrollPosition();

	if (nEffect==EFFECT_FADE)
	{
		pDoc->m_pDib->Clear(nEffect, &dc, -pt.x, -pt.y, 16, 50);
		pDoc->m_pDib->Display(nEffect, &dc, -pt.x, -pt.y, 16, 50);
	}
	else if (nEffect==EFFECT_HBLIND || nEffect==EFFECT_VBLIND)
	{
		pDoc->m_pDib->Clear(nEffect, &dc, -pt.x, -pt.y, 8, 70);
		pDoc->m_pDib->Display(nEffect, &dc, -pt.x, -pt.y, 8, 70);
	}
	else if (nEffect==EFFECT_HRASTER || nEffect==EFFECT_VRASTER || nEffect==EFFECT_MOSAIC)
	{
		pDoc->m_pDib->Clear(nEffect, &dc, -pt.x, -pt.y, 32, 1);
		pDoc->m_pDib->Display(nEffect, &dc, -pt.x, -pt.y, 32, 1);
	}
	else
	{
		pDoc->m_pDib->Clear(nEffect, &dc, -pt.x, -pt.y, 1, 5);
		pDoc->m_pDib->Display(nEffect, &dc, -pt.x, -pt.y, 1, 5);
	}
}
