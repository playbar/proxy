// ViewDIBView.cpp : implementation of the CViewDIBView class
//

#include "stdafx.h"
#include "ViewDIB.h"
#include "MainFrm.h"
#include "Dib.h"

#include "ViewDIBDoc.h"
#include "ViewDIBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDIBView

IMPLEMENT_DYNCREATE(CViewDIBView, CScrollView)

BEGIN_MESSAGE_MAP(CViewDIBView, CScrollView)
	//{{AFX_MSG_MAP(CViewDIBView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDIBView construction/destruction

CViewDIBView::CViewDIBView()
{
	// TODO: add construction code here

}

CViewDIBView::~CViewDIBView()
{
}

BOOL CViewDIBView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CViewDIBView drawing

void CViewDIBView::OnDraw(CDC* pDC)
{
	CViewDIBDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 0, 0);
}

void CViewDIBView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CViewDIBDoc* pDoc = GetDocument();

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
// CViewDIBView printing

BOOL CViewDIBView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewDIBView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewDIBView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CViewDIBView diagnostics

#ifdef _DEBUG
void CViewDIBView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewDIBView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CViewDIBDoc* CViewDIBView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CViewDIBDoc)));
	return (CViewDIBDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDIBView message handlers

LRESULT CViewDIBView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CViewDIBDoc* pDoc = GetDocument();

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
