// EditText.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "EditText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditText

CEditText::CEditText()
{
}

CEditText::~CEditText()
{
}


BEGIN_MESSAGE_MAP(CEditText, CEdit)
	//{{AFX_MSG_MAP(CEditText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditText message handlers

/*void CEditText::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	CPen pen(PS_DASH,1,RGB(0,0,255));
	CPen *pOldPen = dc.SelectObject(&pen);
	CBrush *pOldBrush = dc.SelectObject(CBrush::FromHandle((HBRUSH)::GetStockObject(NULL_BRUSH)));

	dc.Rectangle(&rc);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	// Do not call CEdit::OnPaint() for painting messages
}

*/