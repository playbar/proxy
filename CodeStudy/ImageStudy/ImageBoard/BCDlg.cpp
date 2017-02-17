// BCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dib.h"
#include "ImageBoard.h"
#include "BCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCDlg dialog


CBCDlg::CBCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBCDlg)
	m_strBright = _T("0");
	m_strContrast = _T("0");
	//}}AFX_DATA_INIT
	m_nValue1 = 0;
	m_nValue2 = 0;
}


void CBCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCDlg)
	DDX_Control(pDX, IDC_SCROLLBAR2, m_ScrollContrast);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollBright);
	DDX_Text(pDX, IDC_BRIGHT, m_strBright);
	DDX_Text(pDX, IDC_CONTRAST, m_strContrast);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBCDlg, CDialog)
	//{{AFX_MSG_MAP(CBCDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCDlg message handlers

BOOL CBCDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ScrollBright.SetScrollRange(-100,100);
	m_ScrollContrast.SetScrollRange(-100,100);
	m_ScrollBright.SetScrollPos(m_nValue1);
	m_ScrollContrast.SetScrollPos(m_nValue2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBCDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char str[64];
	int nV,	nMin, nMax;

	pScrollBar->GetScrollRange(&nMin, &nMax);

	if (pScrollBar->m_hWnd == m_ScrollBright.m_hWnd)
		nV = m_nValue1;
	else if (pScrollBar->m_hWnd == m_ScrollContrast.m_hWnd)
		nV = m_nValue2;

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

	if (pScrollBar->m_hWnd == m_ScrollBright.m_hWnd)
	{
		itoa(nV, str, 10);
		m_strBright = str;
		m_nValue1 = nV;
	}
	else if (pScrollBar->m_hWnd == m_ScrollContrast.m_hWnd)
	{
		itoa(nV, str, 10);
		m_strContrast = str;
		m_nValue2 = nV;
	}
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
