#if !defined(AFX_PENPARAMSLIST_H__4732AF85_EE89_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_PENPARAMSLIST_H__4732AF85_EE89_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PenParamsList.h : header file
//

#define ITEM_HEIGHT		10
#define PEN_WIDTH		0
#define PEN_STYLE		1

/////////////////////////////////////////////////////////////////////////////
// CPenParamsList window

class CPenParamsList : public CListBox
{
// Construction
public:
	CPenParamsList();

// Attributes
public:
	int		m_nParamIndex;

// Operations
public:
	void	AddLPItem(UINT uItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenParamsList)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPenParamsList();

	// Generated message map functions
protected:
	void	DrawBorder(CDC *pDC);
	//{{AFX_MSG(CPenParamsList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PENPARAMSLIST_H__4732AF85_EE89_11D2_9481_000021003EA5__INCLUDED_)
