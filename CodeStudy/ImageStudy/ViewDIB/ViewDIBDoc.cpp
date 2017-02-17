// ViewDIBDoc.cpp : implementation of the CViewDIBDoc class
//

#include "stdafx.h"
#include "ViewDIB.h"

#include "ViewDIBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDIBDoc

IMPLEMENT_DYNCREATE(CViewDIBDoc, CDocument)

BEGIN_MESSAGE_MAP(CViewDIBDoc, CDocument)
	//{{AFX_MSG_MAP(CViewDIBDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDIBDoc construction/destruction

CViewDIBDoc::CViewDIBDoc()
{
	m_pDib = new CDib;
}

CViewDIBDoc::~CViewDIBDoc()
{
	delete m_pDib;
}

BOOL CViewDIBDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CViewDIBDoc serialization

void CViewDIBDoc::Serialize(CArchive& ar)
{
	m_pDib->Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CViewDIBDoc diagnostics

#ifdef _DEBUG
void CViewDIBDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CViewDIBDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDIBDoc commands
