// JpegFormatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "JpegFormatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJpegFormatDlg dialog


CJpegFormatDlg::CJpegFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJpegFormatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJpegFormatDlg)
	m_nQuality = 75;
	m_strQuality = _T("75");
	//}}AFX_DATA_INIT
	m_bColor = TRUE;
}


void CJpegFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJpegFormatDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_Quality);
	DDX_Slider(pDX, IDC_SLIDER1, m_nQuality);
	DDX_Text(pDX, IDC_VALUE, m_strQuality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJpegFormatDlg, CDialog)
	//{{AFX_MSG_MAP(CJpegFormatDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJpegFormatDlg message handlers

void CJpegFormatDlg::OnRadio1() 
{
	m_bColor = TRUE;
}

void CJpegFormatDlg::OnRadio2() 
{
	m_bColor = FALSE;
}

void CJpegFormatDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData();
	m_strQuality.Format("%d", m_nQuality);
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CJpegFormatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Quality.SetRange(0, 100, TRUE);
	m_strQuality.Format("%d", m_nQuality);
	UpdateData(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(m_bColor);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(! m_bColor);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
