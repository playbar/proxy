// ShowDIBDoc.cpp : implementation of the CShowDIBDoc class
//

#include "stdafx.h"
#include "dibapi.h" 
#include "ShowDIB.h"
#include "ShowDIBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowDIBDoc

IMPLEMENT_DYNCREATE(CShowDIBDoc, CDocument)

BEGIN_MESSAGE_MAP(CShowDIBDoc, CDocument)
	//{{AFX_MSG_MAP(CShowDIBDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowDIBDoc construction/destruction

CShowDIBDoc::CShowDIBDoc()
{
	m_hDIB = NULL;
}

CShowDIBDoc::~CShowDIBDoc()
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
}

BOOL CShowDIBDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CShowDIBDoc serialization

void CShowDIBDoc::Serialize(CArchive& ar)
{
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
// CShowDIBDoc diagnostics

#ifdef _DEBUG
void CShowDIBDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShowDIBDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowDIBDoc commands

BOOL CShowDIBDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_hDIB = LoadDIB(lpszPathName);
	if (m_hDIB == NULL)
	{
		// may not be DIB format
		return FALSE;
	}
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);     // start off with unmodified
	
	return TRUE;
}

BOOL CShowDIBDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return SaveDIB(m_hDIB, lpszPathName);
}
