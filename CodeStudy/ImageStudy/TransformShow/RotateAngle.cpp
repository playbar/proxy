// RotateAngle.cpp : implementation file
//

#include "stdafx.h"
#include "TransformShow.h"
#include "RotateAngle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateAngle dialog


CRotateAngle::CRotateAngle(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateAngle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotateAngle)
	m_fAngle = 0.0;
	//}}AFX_DATA_INIT
}


void CRotateAngle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateAngle)
	DDX_Text(pDX, IDC_EDIT1, m_fAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotateAngle, CDialog)
	//{{AFX_MSG_MAP(CRotateAngle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateAngle message handlers

void CRotateAngle::OnOK() 
{
	UpdateData();
	
	CDialog::OnOK();
}

BOOL CRotateAngle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT1)->SetFocus();
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
