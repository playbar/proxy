// PenWidth.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "PenWidth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPenWidth dialog


CPenWidth::CPenWidth(CWnd* pParent /*=NULL*/)
	: CDialog(CPenWidth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPenWidth)
	m_nPenWidth = 0;
	//}}AFX_DATA_INIT
}


void CPenWidth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPenWidth)
	DDX_Text(pDX, IDC_EDIT1, m_nPenWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPenWidth, CDialog)
	//{{AFX_MSG_MAP(CPenWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenWidth message handlers

BOOL CPenWidth::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
