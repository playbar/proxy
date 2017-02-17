// SpreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "imageprocess.h"
#include "SpreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpreadDlg dialog


CSpreadDlg::CSpreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpreadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpreadDlg)
	m_nValue = 1;
	m_strValue = _T("1");
	//}}AFX_DATA_INIT
}


void CSpreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpreadDlg)
	DDX_Slider(pDX, IDC_SLIDER1, m_nValue);
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpreadDlg, CDialog)
	//{{AFX_MSG_MAP(CSpreadDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpreadDlg message handlers

BOOL CSpreadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSliderCtrl * pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	pSlider->SetRange(1, 32, TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpreadDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_strValue.Format("%d", m_nValue);
	UpdateData(FALSE);
	
	*pResult = 0;
}
