// EmbossmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "imageprocess.h"
#include "EmbossmentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmbossmentDlg dialog


CEmbossmentDlg::CEmbossmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmbossmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmbossmentDlg)
	m_strValue = _T("0");
	m_nValue = 0;
	//}}AFX_DATA_INIT
}


void CEmbossmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmbossmentDlg)
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	DDX_Slider(pDX, IDC_SLIDER1, m_nValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmbossmentDlg, CDialog)
	//{{AFX_MSG_MAP(CEmbossmentDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmbossmentDlg message handlers

void CEmbossmentDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_strValue.Format("%d", m_nValue);
	UpdateData(FALSE);

	*pResult = 0;
}

BOOL CEmbossmentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSliderCtrl * pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	pSlider->SetRange(0, 255, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
