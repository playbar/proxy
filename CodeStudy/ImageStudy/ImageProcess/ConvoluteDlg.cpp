// ConvoluteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ConvoluteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConvoluteDlg dialog


CConvoluteDlg::CConvoluteDlg(int nMin, int nMax, int nDef, CWnd* pParent /*=NULL*/)
	: CDialog(CConvoluteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConvoluteDlg)
	m_strValue = _T("");
	m_nStreng = nDef;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMax = nMax;
	m_nMin = nMin;
}


void CConvoluteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConvoluteDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_Streng);
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	DDX_Slider(pDX, IDC_SLIDER1, m_nStreng);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvoluteDlg, CDialog)
	//{{AFX_MSG_MAP(CConvoluteDlg)
	ON_WM_HSCROLL()
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvoluteDlg message handlers

BOOL CConvoluteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Streng.SetRange(m_nMin, m_nMax, TRUE);
	m_strValue.Format("%d", m_nStreng);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConvoluteDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData();
	m_strValue.Format("%d", m_nStreng);
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
