// AdjustColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dib.h"
#include "ImageBoard.h"
#include "AdjustColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjustColorDlg dialog


CAdjustColorDlg::CAdjustColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjustColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdjustColorDlg)
	m_strValue1 = _T("0");
	m_strValue2 = _T("0");
	m_strValue3 = _T("0");
	m_strName1 = _T("R");
	m_strName2 = _T("G");
	m_strName3 = _T("B");
	//}}AFX_DATA_INIT

	m_nColorModel = RGB_COLOR;
	m_nValue1 = 0;
	m_nValue2 = 0;
	m_nValue3 = 0;
}


void CAdjustColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustColorDlg)
	DDX_Control(pDX, IDC_SCROLLBAR2, m_ScrollBar2);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_ScrollBar3);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollBar1);
	DDX_Text(pDX, IDC_VALUE1, m_strValue1);
	DDX_Text(pDX, IDC_VALUE2, m_strValue2);
	DDX_Text(pDX, IDC_VALUE3, m_strValue3);
	DDX_Text(pDX, IDC_NAME1, m_strName1);
	DDX_Text(pDX, IDC_NAME2, m_strName2);
	DDX_Text(pDX, IDC_NAME3, m_strName3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjustColorDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustColorDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjustColorDlg message handlers

BOOL CAdjustColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	m_ScrollBar1.SetScrollRange(-255,255);
	m_ScrollBar2.SetScrollRange(-255,255);
	m_ScrollBar3.SetScrollRange(-255,255);
	m_ScrollBar1.SetScrollPos(m_nValue1);
	m_ScrollBar2.SetScrollPos(m_nValue2);
	m_ScrollBar3.SetScrollPos(m_nValue3);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdjustColorDlg::OnRadio1() 
{
	m_nColorModel = RGB_COLOR;
	m_strName1 = "R";
	m_strName2 = "G";
	m_strName3 = "B";
	m_strValue1 = "0";
	m_strValue2 = "0";
	m_strValue3 = "0";
	m_nValue1 = 0;
	m_nValue2 = 0;
	m_nValue3 = 0;
	UpdateData(FALSE);	
	m_ScrollBar1.SetScrollRange(-255,255);
	m_ScrollBar2.SetScrollRange(-255,255);
	m_ScrollBar3.SetScrollRange(-255,255);
	m_ScrollBar1.SetScrollPos(m_nValue1);
	m_ScrollBar2.SetScrollPos(m_nValue2);
	m_ScrollBar3.SetScrollPos(m_nValue3);
}

void CAdjustColorDlg::OnRadio2() 
{
	m_nColorModel = HSI_COLOR;
	m_strName1 = "H";
	m_strName2 = "S";
	m_strName3 = "I";
	m_strValue1 = "0";
	m_strValue2 = "0";
	m_strValue3 = "0";
	m_nValue1 = 0;
	m_nValue2 = 0;
	m_nValue3 = 0;
	UpdateData(FALSE);	
	m_ScrollBar1.SetScrollRange(-180,180);
	m_ScrollBar2.SetScrollRange(-275,275);
	m_ScrollBar3.SetScrollRange(-442,442);
	m_ScrollBar1.SetScrollPos(m_nValue1);
	m_ScrollBar2.SetScrollPos(m_nValue2);
	m_ScrollBar3.SetScrollPos(m_nValue3);
}

void CAdjustColorDlg::OnRadio3() 
{
	m_nColorModel = HLS_COLOR;
	m_strName1 = "H";
	m_strName2 = "L";
	m_strName3 = "S";
	m_strValue1 = "0";
	m_strValue2 = "0";
	m_strValue3 = "0";
	m_nValue1 = 0;
	m_nValue2 = 0;
	m_nValue3 = 0;
	UpdateData(FALSE);	
	m_ScrollBar1.SetScrollRange(-180,180);
	m_ScrollBar2.SetScrollRange(-100,100);
	m_ScrollBar3.SetScrollRange(-100,100);
	m_ScrollBar1.SetScrollPos(m_nValue1);
	m_ScrollBar2.SetScrollPos(m_nValue2);
	m_ScrollBar3.SetScrollPos(m_nValue3);
}

void CAdjustColorDlg::OnRadio4() 
{
	m_nColorModel = CMYK_COLOR;
	m_strName1 = "C";
	m_strName2 = "M";
	m_strName3 = "Y";
	m_strValue1 = "0";
	m_strValue2 = "0";
	m_strValue3 = "0";
	m_nValue1 = 0;
	m_nValue2 = 0;
	m_nValue3 = 0;
	UpdateData(FALSE);	
	m_ScrollBar1.SetScrollRange(-255,255);
	m_ScrollBar2.SetScrollRange(-255,255);
	m_ScrollBar3.SetScrollRange(-255,255);
	m_ScrollBar1.SetScrollPos(m_nValue1);
	m_ScrollBar2.SetScrollPos(m_nValue2);
	m_ScrollBar3.SetScrollPos(m_nValue3);
}

void CAdjustColorDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAdjustColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	char str[64];
	int nV,	nMin, nMax;

	pScrollBar->GetScrollRange(&nMin, &nMax);

	if (pScrollBar->m_hWnd == m_ScrollBar1.m_hWnd)
		nV = m_nValue1;
	else if (pScrollBar->m_hWnd == m_ScrollBar2.m_hWnd)
		nV = m_nValue2;
	else if (pScrollBar->m_hWnd == m_ScrollBar3.m_hWnd)
		nV = m_nValue3;

	switch (nSBCode)
	{
		case SB_LEFT:
			nV = nMin;
			break;
		case SB_RIGHT:
			nV = nMax;
			break;
		case SB_LINELEFT:
			nV--;
			break;
		case SB_LINERIGHT:
			nV++;
			break;
		case SB_PAGELEFT:
			nV -= 20*nMax/100;
			break;
		case SB_PAGERIGHT:
			nV += 20*nMax/100;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			nV = nPos;
			break;
	}

	nV = BOUND(nV, nMin, nMax);
	pScrollBar->SetScrollPos(nV);

	if (pScrollBar->m_hWnd == m_ScrollBar1.m_hWnd)
	{
		itoa(nV, str, 10);
		m_strValue1 = str;
		m_nValue1 = nV;
	}
	else if (pScrollBar->m_hWnd == m_ScrollBar2.m_hWnd)
	{
		itoa(nV, str, 10);
		m_strValue2 = str;
		m_nValue2 = nV;
	}
	else if (pScrollBar->m_hWnd == m_ScrollBar3.m_hWnd)
	{
		itoa(nV, str, 10);
		m_strValue3 = str;
		m_nValue3 = nV;
	}
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

