// TransformShowDoc.cpp : implementation of the CTransformShowDoc class
//

#include "stdafx.h"
#include "TransformShow.h"

#include "TransformShowDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransformShowDoc

IMPLEMENT_DYNCREATE(CTransformShowDoc, CDocument)

BEGIN_MESSAGE_MAP(CTransformShowDoc, CDocument)
	//{{AFX_MSG_MAP(CTransformShowDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransformShowDoc construction/destruction

CTransformShowDoc::CTransformShowDoc()
{
	m_pDib = new CDib;
}

CTransformShowDoc::~CTransformShowDoc()
{
	delete m_pDib;
}

BOOL CTransformShowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTransformShowDoc serialization

void CTransformShowDoc::Serialize(CArchive& ar)
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
// CTransformShowDoc diagnostics

#ifdef _DEBUG
void CTransformShowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTransformShowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTransformShowDoc commands
