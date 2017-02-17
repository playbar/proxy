// MaStatic.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "MaStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaStatic

CMaStatic::CMaStatic()
{
}

CMaStatic::~CMaStatic()
{
}


BEGIN_MESSAGE_MAP(CMaStatic, CStatic)
	//{{AFX_MSG_MAP(CMaStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaStatic message handlers

HBRUSH CMaStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor(RGB(255,255,255));
	// Set transparent drawing mode
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);//	BLACK_BRUSH	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
}
