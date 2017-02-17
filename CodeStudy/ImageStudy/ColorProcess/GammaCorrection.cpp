// GammaCorrection.cpp : implementation file
//

#include "stdafx.h"
#include "colorprocess.h"
#include "GammaCorrection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGammaCorrection dialog


CGammaCorrection::CGammaCorrection(CWnd* pParent /*=NULL*/)
	: CDialog(CGammaCorrection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGammaCorrection)
	m_strGamma = _T("");
	m_nGamma = 0;
	//}}AFX_DATA_INIT

	m_fGamma = 0.0;
}


void CGammaCorrection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGammaCorrection)
	DDX_Text(pDX, IDC_BRIGHT, m_strGamma);
	DDX_Slider(pDX, IDC_SLIDER1, m_nGamma);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGammaCorrection, CDialog)
	//{{AFX_MSG_MAP(CGammaCorrection)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGammaCorrection message handlers

BOOL CGammaCorrection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->SetRangeMin(1, TRUE);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->SetRangeMax(50, TRUE);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->SetPos(1);
	m_strGamma = "0.1";
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGammaCorrection::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_fGamma = m_nGamma * 0.1;
	m_strGamma.Format("%.2f", m_fGamma);
	UpdateData(FALSE);

	*pResult = 0;
}

