// ImageBoardDoc.cpp : implementation of the CImageBoardDoc class
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "ImageBoardDoc.h"
#include "JpegFormatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageBoardDoc

IMPLEMENT_DYNCREATE(CImageBoardDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageBoardDoc, CDocument)
	//{{AFX_MSG_MAP(CImageBoardDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageBoardDoc construction/destruction

CImageBoardDoc::CImageBoardDoc()
{
	m_pDib	= new CDib;
	m_pDib->Create(DEFAULT_HEIGHT, DEFAULT_WIDTH);
}

CImageBoardDoc::~CImageBoardDoc()
{
	if (m_pDib != NULL)
		delete m_pDib;
}

BOOL CImageBoardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageBoardDoc serialization

void CImageBoardDoc::Serialize(CArchive& ar)
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
// CImageBoardDoc diagnostics

#ifdef _DEBUG
void CImageBoardDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageBoardDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageBoardDoc commands


void CImageBoardDoc::OnFileSaveAs() 
{
	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szSaveFilter);

	CString strPathName = GetPathName();
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath((LPCSTR)strPathName, drive, dir, fname, ext);
	
	if (! stricmp(ext, ".gif")) // GIF file
		FileDlg.m_ofn.nFilterIndex = 5;
	else if (! stricmp(ext, ".jpg") ||
			 ! stricmp(ext, ".jpe") ||
			 ! stricmp(ext, ".jpeg") ) // JPEG file
		FileDlg.m_ofn.nFilterIndex = 6;
	else if (! stricmp(ext, ".pcx")) // PCX file
		FileDlg.m_ofn.nFilterIndex = 7;
	else if (! stricmp(ext, ".tga")) // TGA file
		FileDlg.m_ofn.nFilterIndex = 8;
	else if (! stricmp(ext, ".tif") || ! stricmp(ext, ".tiff")) // TIFF file
		FileDlg.m_ofn.nFilterIndex = 9;
	else
	{
		switch (m_pDib->GetBitCount())
		{
		case 1:
			FileDlg.m_ofn.nFilterIndex = 1;
			break;
		case 4:
			FileDlg.m_ofn.nFilterIndex = 2;
			break;
		case 8:
			FileDlg.m_ofn.nFilterIndex = 3;
			break;
		case 24:
		default:
			FileDlg.m_ofn.nFilterIndex = 4;
			break;
		}
	}

	if (FileDlg.DoModal() == IDOK)
	{
		switch (FileDlg.m_ofn.nFilterIndex)
		{
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			break;
		case 1:
			m_pDib->ConvertFormat(1);
			break;
		case 2:
			m_pDib->ConvertFormat(4);
			break;
		case 3:
			m_pDib->ConvertFormat(8);
			break;
		case 4:
		default:
			m_pDib->ConvertFormat(24);
			break;
		}

		OnSaveDocument(FileDlg.m_ofn.lpstrFile);
		SetPathName(FileDlg.m_ofn.lpstrFile);
		SetTitle(FileDlg.m_ofn.lpstrFile);
		UpdateAllViews(NULL);
		SetModifiedFlag(FALSE);
	}
}

BOOL CImageBoardDoc::OnOpenDocument(LPCTSTR lpszPathName) 
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

BOOL CImageBoardDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath(lpszPathName, drive, dir, fname, ext);   

	if (! stricmp(ext, ".gif")) // GIF file
	{
		CGif gif(m_pDib);
		return gif.Save(lpszPathName);
	}
	else if (! stricmp(ext, ".jpg") ||
			 ! stricmp(ext, ".jpe") ||
			 ! stricmp(ext, ".jpeg") ) // JPEG file
	{
		CJpegFormatDlg JpegDlg;
		if (JpegDlg.DoModal() == IDOK)
		{
			CJpeg jpeg;
			return jpeg.Save(lpszPathName, m_pDib, JpegDlg.m_bColor, JpegDlg.m_nQuality);
		}
		return FALSE;
	}
	else if (! stricmp(ext, ".pcx")) // PCX file
	{
		CPcx pcx;
		return pcx.Save(lpszPathName, m_pDib);
	}
	else if (! stricmp(ext, ".tga")) // TGA file
	{
		CTga tga;
		return tga.Save(lpszPathName, m_pDib);
	}
	else if (! stricmp(ext, ".tif") || 
		     ! stricmp(ext, ".tiff")) // TIFF file
	{
		CTif tif;
		return tif.Save(lpszPathName, m_pDib);
	}
	
	return CDocument::OnSaveDocument(lpszPathName);
}