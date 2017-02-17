// ExposureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "ExposureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExposureDlg dialog


CExposureDlg::CExposureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExposureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureDlg)
	m_nThreshold = 0;
	//}}AFX_DATA_INIT
}


void CExposureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureDlg)
	DDX_Text(pDX, IDC_EDIT1, m_nThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExposureDlg, CDialog)
	//{{AFX_MSG_MAP(CExposureDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExposureDlg message handlers

void CExposureDlg::OnOK() 
{
	UpdateData(TRUE);	
	CDialog::OnOK();
}
