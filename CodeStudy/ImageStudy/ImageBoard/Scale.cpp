// Scale.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "Scale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScale dialog


CScale::CScale(int nWidth, int nHeight, CWnd* pParent /*=NULL*/)
	: CDialog(CScale::IDD, pParent)
{
	CString strH, strW;
	strH.Format("%d", nHeight);
	strW.Format("%d", nWidth);
	//{{AFX_DATA_INIT(CScale)
	m_bProportional = TRUE;
	m_strHeight = strH;
	m_strWidth = strW;
	//}}AFX_DATA_INIT
	m_fRatio = (double)nHeight/(double)nWidth;
	m_nUnit	 = 2;
	m_nWidth  = nWidth;
	m_nHeight = nHeight;
	m_nWidth0  = m_nWidth;
	m_nHeight0 = m_nHeight;
}


void CScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScale)
	DDX_Check(pDX, IDC_PROPORTIONAL, m_bProportional);
	DDX_Text(pDX, IDC_HEIGHT, m_strHeight);
	DDX_Text(pDX, IDC_WIDTH, m_strWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScale, CDialog)
	//{{AFX_MSG_MAP(CScale)
	ON_BN_CLICKED(IDC_CM, OnCm)
	ON_EN_CHANGE(IDC_HEIGHT, OnChangeHeight)
	ON_BN_CLICKED(IDC_INCH, OnInch)
	ON_BN_CLICKED(IDC_PIXEL, OnPixel)
	ON_BN_CLICKED(IDC_PROPORTIONAL, OnProportional)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_EN_CHANGE(IDC_WIDTH, OnChangeWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScale message handlers

void CScale::OnCm() 
{
	m_nUnit = 0;
	
	m_strWidth.Format("%.2f", ((double)m_nWidth/(double)m_nPixelsPInchX)*2.54);
	m_strHeight.Format("%.2f", ((double)m_nHeight/(double)m_nPixelsPInchY)*2.54);
	UpdateData(FALSE);
}

void CScale::OnInch() 
{
	m_nUnit = 1;
	
	m_strWidth.Format("%.2f", (double)m_nWidth/(double)m_nPixelsPInchX);
	m_strHeight.Format("%.2f", (double)m_nHeight/(double)m_nPixelsPInchY);
	UpdateData(FALSE);
}

void CScale::OnPixel() 
{
	m_nUnit = 2;
	
	m_strWidth.Format("%d", m_nWidth);
	m_strHeight.Format("%d", m_nHeight);
	UpdateData(FALSE);
}

void CScale::OnProportional() 
{
	UpdateData();
	if (m_bProportional)
	{
		m_nHeight = (int)(m_nWidth*m_fRatio);
		UpdateData(FALSE);
	}
}

void CScale::OnReset() 
{
	m_nWidth = m_nWidth0;
	m_nHeight = m_nHeight0;

	if (m_nUnit == 0)
	{
		m_strWidth.Format("%.2f",((double)m_nWidth/(double)m_nPixelsPInchX)*2.54);
		m_strHeight.Format("%.2f",((double)m_nHeight/(double)m_nPixelsPInchX)*2.54);
	}
	else if (m_nUnit == 1)
	{
		m_strWidth.Format("%.2f", (double)m_nWidth/(double)m_nPixelsPInchX);
		m_strHeight.Format("%.2f", (double)m_nHeight/(double)m_nPixelsPInchX);
	}
	else if (m_nUnit == 2)
	{
		m_strWidth.Format("%d", m_nWidth);
		m_strHeight.Format("%d", m_nHeight);
	}
	UpdateData(FALSE);
}

void CScale::OnChangeHeight() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();

	if (m_nUnit == 0)
		m_nHeight = (int)(atof(m_strHeight) * (double)m_nPixelsPInchY/2.54);
	else if (m_nUnit == 1)
		m_nHeight = (int)(atof(m_strHeight) * (double)m_nPixelsPInchY);
	else if (m_nUnit == 2)
		m_nHeight = atoi(m_strHeight);
	if (m_bProportional)
	{
		m_nWidth = (int)(m_nHeight/m_fRatio);
		m_strWidth.Format("%d", m_nWidth);
		UpdateData(FALSE);
	}
}

void CScale::OnChangeWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();

	if (m_nUnit == 0)
		m_nWidth = (int)(atof(m_strWidth) * (double)m_nPixelsPInchX/2.54);
	else if (m_nUnit == 1)
		m_nWidth = (int)(atof(m_strWidth) * (double)m_nPixelsPInchX);
	else if (m_nUnit == 2)
		m_nWidth = atoi(m_strWidth);
	if (m_bProportional)
	{
		m_nHeight = (int)(m_nWidth*m_fRatio);
		m_strHeight.Format("%d", m_nHeight);
		UpdateData(FALSE);
	}
}

BOOL CScale::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CClientDC dc(this);
	m_nPixelsPInchX = dc.GetDeviceCaps(LOGPIXELSX);
	m_nPixelsPInchY = dc.GetDeviceCaps(LOGPIXELSY);

	switch (m_nUnit)
	{
	case 0:
		((CButton *)GetDlgItem(IDC_CM))->SetCheck(1);
		break;
	case 1:
		((CButton *)GetDlgItem(IDC_INCH))->SetCheck(1);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_PIXEL))->SetCheck(1);
		break;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
