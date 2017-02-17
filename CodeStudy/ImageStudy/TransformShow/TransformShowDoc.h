// TransformShowDoc.h : interface of the CTransformShowDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORMSHOWDOC_H__E4CDE1B0_E755_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_TRANSFORMSHOWDOC_H__E4CDE1B0_E755_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CTransformShowDoc : public CDocument
{
protected: // create from serialization only
	CTransformShowDoc();
	DECLARE_DYNCREATE(CTransformShowDoc)

// Attributes
public:
	CDib*	m_pDib;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformShowDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransformShowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTransformShowDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMSHOWDOC_H__E4CDE1B0_E755_11D2_9481_000021003EA5__INCLUDED_)
