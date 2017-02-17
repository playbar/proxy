// GrayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dib.h"
#include "ImageBoard.h"
#include "GrayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrayDlg dialog


CGrayDlg::CGrayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGrayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrayDlg)
	m_strWeightR = _T("0.30");
	m_strWeightG = _T("0.59");
	m_strWeightB = _T("0.11");
	//}}AFX_DATA_INIT
	m_nMethod = MEAN_GRAY;
	m_fWeightR = atof((LPCSTR)m_strWeightR);
	m_fWeightG = atof((LPCSTR)m_strWeightG);
	m_fWeightB = atof((LPCSTR)m_strWeightB);
}


void CGrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrayDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strWeightR);
	DDX_Text(pDX, IDC_EDIT2, m_strWeightG);
	DDX_Text(pDX, IDC_EDIT3, m_strWeightB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGrayDlg, CDialog)
	//{{AFX_MSG_MAP(CGrayDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrayDlg message handlers

void CGrayDlg::OnRadio1() 
{
	m_nMethod = MEAN_GRAY;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
}

void CGrayDlg::OnRadio2() 
{
	m_nMethod = MAXIMUM_GRAY;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
}

void CGrayDlg::OnRadio3() 
{
	m_nMethod = WEIGHT_GRAY;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
}

BOOL CGrayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGrayDlg::OnOK() 
{
	UpdateData(TRUE);
	m_fWeightR = atof((LPCSTR)m_strWeightR);
	m_fWeightG = atof((LPCSTR)m_strWeightG);
	m_fWeightB = atof((LPCSTR)m_strWeightB);
	
	CDialog::OnOK();
}
