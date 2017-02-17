// CanvasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TransformShow.h"
#include "CanvasDlg.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanvasDlg dialog


CCanvasDlg::CCanvasDlg(int nWidth, int nHeight, CWnd* pParent /*=NULL*/)
	: CDialog(CCanvasDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCanvasDlg)
	m_bProportionXY = TRUE;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	//}}AFX_DATA_INIT
	m_fRatio = (double)nHeight/(double)nWidth;
	m_nPosition = LEFT_UP;
}


void CCanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCanvasDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bProportionXY);
	DDX_Text(pDX, IDC_EDIT1, m_nWidth);
	DDX_Text(pDX, IDC_EDIT2, m_nHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanvasDlg, CDialog)
	//{{AFX_MSG_MAP(CCanvasDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvasDlg message handlers

void CCanvasDlg::OnRadio1() 
{
	m_nPosition = LEFT_UP;
}

void CCanvasDlg::OnRadio2() 
{
	m_nPosition = CENTER_UP;
}

void CCanvasDlg::OnRadio3() 
{
	m_nPosition = RIGHT_UP;
}

void CCanvasDlg::OnRadio4() 
{
	m_nPosition = LEFT_CENTER;
}

void CCanvasDlg::OnRadio5() 
{
	m_nPosition = CENTER_CENTER;
}

void CCanvasDlg::OnRadio6() 
{
	m_nPosition = CENTER_RIGHT;
}

void CCanvasDlg::OnRadio7() 
{
	m_nPosition = LEFT_DOWN;
}

void CCanvasDlg::OnRadio8() 
{
	m_nPosition = CENTER_DOWN;
}

void CCanvasDlg::OnRadio9() 
{
	m_nPosition = RIGHT_DOWN;
}

void CCanvasDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();
	if (m_bProportionXY)
	{
		m_nHeight = (int)(m_nWidth*m_fRatio);
		UpdateData(FALSE);
	}
}

void CCanvasDlg::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();
	if (m_bProportionXY)
	{
		m_nWidth = (int)(m_nHeight/m_fRatio);
		UpdateData(FALSE);
	}
}

void CCanvasDlg::OnCheck1() 
{
	UpdateData();
	if (m_bProportionXY)
	{
		m_nHeight = (int)(m_nWidth*m_fRatio);
		UpdateData(FALSE);
	}
}

BOOL CCanvasDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
