// ViewDIBDoc.h : interface of the CViewDIBDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWDIBDOC_H__E0DA1D8C_E308_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_VIEWDIBDOC_H__E0DA1D8C_E308_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CViewDIBDoc : public CDocument
{
protected: // create from serialization only
	CViewDIBDoc();
	DECLARE_DYNCREATE(CViewDIBDoc)

// Attributes
public:
	CDib * m_pDib;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDIBDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewDIBDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CViewDIBDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDIBDOC_H__E0DA1D8C_E308_11D2_9481_000021003EA5__INCLUDED_)
