// ColorProcessDoc.cpp : implementation of the CColorProcessDoc class
//

#include "stdafx.h"
#include "ColorProcess.h"

#include "ColorProcessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorProcessDoc

IMPLEMENT_DYNCREATE(CColorProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorProcessDoc, CDocument)
	//{{AFX_MSG_MAP(CColorProcessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorProcessDoc construction/destruction

CColorProcessDoc::CColorProcessDoc()
{
	m_pDib = new CDib;
}

CColorProcessDoc::~CColorProcessDoc()
{
	delete m_pDib;
}

BOOL CColorProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CColorProcessDoc serialization

void CColorProcessDoc::Serialize(CArchive& ar)
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
// CColorProcessDoc diagnostics

#ifdef _DEBUG
void CColorProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColorProcessDoc commands
