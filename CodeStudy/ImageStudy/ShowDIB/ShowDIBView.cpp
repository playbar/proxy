// ShowDIBView.cpp : implementation of the CShowDIBView class
//

#include "stdafx.h"
#include "dibapi.h" 
#include "ShowDIB.h"
#include "MainFrm.h"
#include "ShowDIBDoc.h"
#include "ShowDIBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowDIBView

IMPLEMENT_DYNCREATE(CShowDIBView, CScrollView)

BEGIN_MESSAGE_MAP(CShowDIBView, CScrollView)
	//{{AFX_MSG_MAP(CShowDIBView)
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
// CShowDIBView construction/destruction

CShowDIBView::CShowDIBView()
{
	// TODO: add construction code here

}

CShowDIBView::~CShowDIBView()
{
}

BOOL CShowDIBView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CShowDIBView drawing

void CShowDIBView::OnDraw(CDC* pDC)
{
	CShowDIBDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_hDIB == NULL)
		return;

	PaintDIB(pDC->GetSafeHdc(),
		     m_rcDIB, 
			 pDoc->m_hDIB, 
			 m_rcDIB, 
			 NULL, 
			 SRCCOPY);
}

void CShowDIBView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CShowDIBDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize sizeTotal;
	if (pDoc->m_hDIB != NULL)
	{
		LPBITMAPINFOHEADER lpDIB = (LPBITMAPINFOHEADER)GlobalLock(pDoc->m_hDIB);
		m_rcDIB.left = 0;
		m_rcDIB.top = 0;
		sizeTotal.cx = m_rcDIB.right = ((LPBITMAPINFOHEADER)lpDIB)->biWidth;
		sizeTotal.cy = m_rcDIB.bottom = ((LPBITMAPINFOHEADER)lpDIB)->biHeight;
		GlobalUnlock(pDoc->m_hDIB);
	}
	else
	{
		m_rcDIB.SetRectEmpty();
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CShowDIBView printing

BOOL CShowDIBView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShowDIBView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShowDIBView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CShowDIBView diagnostics

#ifdef _DEBUG
void CShowDIBView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CShowDIBView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CShowDIBDoc* CShowDIBView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShowDIBDoc)));
	return (CShowDIBDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowDIBView message handlers

LRESULT CShowDIBView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CShowDIBDoc* pDoc = GetDocument();

	if (pDoc->m_hDIB == NULL)
		return 0L;  // must be a new document

	LPBYTE lpbi = (LPBYTE)GlobalLock(pDoc->m_hDIB);
	CPalette* pPal = CPalette::FromHandle(CreateDIBPalette(lpbi));
	GlobalUnlock(pDoc->m_hDIB);
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
