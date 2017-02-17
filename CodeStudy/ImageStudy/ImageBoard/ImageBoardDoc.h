// ImageBoardDoc.h : interface of the CImageBoardDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEBOARDDOC_H__CEBADA0C_ED34_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_IMAGEBOARDDOC_H__CEBADA0C_ED34_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"
#include "Gif.h"
#include "Jpeg.h"
#include "Pcx.h"
#include "Tif.h"
#include "Tga.h"

#define DEFAULT_HEIGHT	320
#define DEFAULT_WIDTH	240

class CImageBoardDoc : public CDocument
{
protected: // create from serialization only
	CImageBoardDoc();
	DECLARE_DYNCREATE(CImageBoardDoc)

// Attributes
public:
	CDib	*m_pDib;
	CString m_strTifPathName;
	BOOL m_bLoadTif;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageBoardDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageBoardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageBoardDoc)
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEBOARDDOC_H__CEBADA0C_ED34_11D2_9481_000021003EA5__INCLUDED_)
