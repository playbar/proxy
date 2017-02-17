// ColorBitsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dib.h"
#include "ImageBoard.h"
#include "ColorBitsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBitsDlg dialog


CColorBitsDlg::CColorBitsDlg(int nColorBits, CWnd* pParent /*=NULL*/)
	: CDialog(CColorBitsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorBitsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nColorBits = nColorBits;
}


void CColorBitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBitsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBitsDlg, CDialog)
	//{{AFX_MSG_MAP(CColorBitsDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO24, OnRadio24)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBitsDlg message handlers

BOOL CColorBitsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	switch (m_nColorBits)
	{
	case 1:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO24))->SetCheck(0);
		break;
	case 4:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO24))->SetCheck(0);
		break;
	case 8:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO24))->SetCheck(0);
		break;
	case 24:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO24))->SetCheck(1);
		break;
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorBitsDlg::OnRadio1() 
{
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	m_nColorBits = 1;
}

void CColorBitsDlg::OnRadio24() 
{
	((CButton *)GetDlgItem(IDC_RADIO24))->SetCheck(1);
	m_nColorBits = 24;
}

void CColorBitsDlg::OnRadio4() 
{
	((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(1);
	m_nColorBits = 4;
}

void CColorBitsDlg::OnRadio8() 
{
	((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(1);
	m_nColorBits = 8;
}

