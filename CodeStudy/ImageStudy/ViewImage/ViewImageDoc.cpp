// ViewImageDoc.cpp : implementation of the CViewImageDoc class
//

#include "stdafx.h"
#include "ViewImage.h"

#include "ViewImageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewImageDoc

IMPLEMENT_DYNCREATE(CViewImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CViewImageDoc, CDocument)
	//{{AFX_MSG_MAP(CViewImageDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewImageDoc construction/destruction

CViewImageDoc::CViewImageDoc()
{
	m_pDib = new CDib;
}

CViewImageDoc::~CViewImageDoc()
{
	delete m_pDib;
}

BOOL CViewImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CViewImageDoc serialization

void CViewImageDoc::Serialize(CArchive& ar)
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
// CViewImageDoc diagnostics

#ifdef _DEBUG
void CViewImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CViewImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewImageDoc commands

BOOL CViewImageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath(lpszPathName, drive, dir, fname, ext);   

	if (! stricmp(ext, ".gif")) // GIF file
	{
		CGif gif;
		if (! gif.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(gif.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;

		m_pDib->Attach(hDIB);
	
		return TRUE;
	}
	else if (! stricmp(ext, ".jpg") ||
			 ! stricmp(ext, ".jpe") ||
			 ! stricmp(ext, ".jpeg") ) // JPEG file
	{
		CJpeg jpeg;
		if (! jpeg.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(jpeg.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;

		m_pDib->Attach(hDIB);
	
		return TRUE;
	}
	else if (! stricmp(ext, ".pcx")) // PCX file
	{
		CPcx pcx;
		if (! pcx.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(pcx.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;

		m_pDib->Attach(hDIB);
	
		return TRUE;
	}
	else if (! stricmp(ext, ".tga")) // TGA file
	{
		CTga tga;
		if (! tga.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(tga.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;

		m_pDib->Attach(hDIB);
	
		return TRUE;
	}
	else if (! stricmp(ext, ".tif") ||
			 ! stricmp(ext, ".tiff")) // TIF file
	{
		CTif tif;
		if (! tif.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(tif.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;

		m_pDib->Attach(hDIB);
	
		return TRUE;
	}

	return CDocument::OnOpenDocument(lpszPathName);
}
