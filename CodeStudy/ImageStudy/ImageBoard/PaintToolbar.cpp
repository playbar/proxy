// PaintToolbar.cpp : implementation of the Floating tool palette class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ImageBoard.h"

#include "PaintToolbar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaintToolbar

BEGIN_MESSAGE_MAP(CPaintToolbar, CToolBar)
	//{{AFX_MSG_MAP(CPaintToolbar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintToolbar construction/destruction

CPaintToolbar::CPaintToolbar()
{
	m_nColumns = 2;
	m_cxLeftBorder = 5;
	m_cyTopBorder = 5;
	m_cxRightBorder = 5;
	m_cyBottomBorder = 5;
}

CPaintToolbar::~CPaintToolbar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPaintToolbar diagnostics

#ifdef _DEBUG
void CPaintToolbar::AssertValid() const
{
	CToolBar::AssertValid();
}

void CPaintToolbar::Dump(CDumpContext& dc) const
{
	CToolBar::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPaintToolbar message handlers

void CPaintToolbar::SetColumns(UINT nColumns)
{
	m_nColumns = nColumns;
	int nCount = GetToolBarCtrl().GetButtonCount();

	for (int i = 0; i < nCount; i++)
	{
		UINT nStyle = GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % nColumns) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		SetButtonStyle(i, nStyle);
	}

	Invalidate();
	GetParentFrame()->RecalcLayout();
}
