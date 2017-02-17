// EffectShowDoc.cpp : implementation of the CEffectShowDoc class
//

#include "stdafx.h"
#include "EffectShow.h"

#include "EffectShowDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEffectShowDoc

IMPLEMENT_DYNCREATE(CEffectShowDoc, CDocument)

BEGIN_MESSAGE_MAP(CEffectShowDoc, CDocument)
	//{{AFX_MSG_MAP(CEffectShowDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEffectShowDoc construction/destruction

CEffectShowDoc::CEffectShowDoc()
{
	m_pDib = new CDib;
}

CEffectShowDoc::~CEffectShowDoc()
{
	delete m_pDib;
}

BOOL CEffectShowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEffectShowDoc serialization

void CEffectShowDoc::Serialize(CArchive& ar)
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
// CEffectShowDoc diagnostics

#ifdef _DEBUG
void CEffectShowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEffectShowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEffectShowDoc commands
