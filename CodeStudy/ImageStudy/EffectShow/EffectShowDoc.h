// EffectShowDoc.h : interface of the CEffectShowDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTSHOWDOC_H__8170300C_E560_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_EFFECTSHOWDOC_H__8170300C_E560_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CEffectShowDoc : public CDocument
{
protected: // create from serialization only
	CEffectShowDoc();
	DECLARE_DYNCREATE(CEffectShowDoc)

// Attributes
public:
	CDib * m_pDib;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectShowDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEffectShowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEffectShowDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTSHOWDOC_H__8170300C_E560_11D2_9481_000021003EA5__INCLUDED_)
