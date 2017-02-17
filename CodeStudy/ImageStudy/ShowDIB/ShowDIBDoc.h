// ShowDIBDoc.h : interface of the CShowDIBDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWDIBDOC_H__512EDD06_DFEB_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_SHOWDIBDOC_H__512EDD06_DFEB_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShowDIBDoc : public CDocument
{
protected: // create from serialization only
	CShowDIBDoc();
	DECLARE_DYNCREATE(CShowDIBDoc)

// Attributes
public:
	HANDLE	m_hDIB;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowDIBDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShowDIBDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShowDIBDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWDIBDOC_H__512EDD06_DFEB_11D2_9481_000021003EA5__INCLUDED_)
