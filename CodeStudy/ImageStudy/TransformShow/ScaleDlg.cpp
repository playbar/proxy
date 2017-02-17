// ScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TransformShow.h"
#include "ScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog


CScaleDlg::CScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScaleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScaleDlg)
	m_strXRatio = _T("1.0");
	m_strYRatio = _T("1.0");
	m_bProportionXY = TRUE;
	//}}AFX_DATA_INIT
	m_fRatioX = 1.0;
	m_fRatioY = 1.0;
}


void CScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScaleDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strXRatio);
	DDX_Check(pDX, IDC_CHECK1, m_bProportionXY);
	DDX_Text(pDX, IDC_EDIT2, m_strYRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CScaleDlg)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg message handlers

void CScaleDlg::OnCheck1() 
{
	UpdateData();
	if (m_bProportionXY)
	{
		m_strYRatio = m_strXRatio;
		UpdateData(FALSE);
	}
}

void CScaleDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();
	if (m_bProportionXY)
	{
		m_strYRatio = m_strXRatio;
		UpdateData(FALSE);
	}
}

void CScaleDlg::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();
	if (m_bProportionXY)
	{
		m_strXRatio = m_strYRatio;
		UpdateData(FALSE);
	}
}

BOOL CScaleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScaleDlg::OnOK() 
{
	UpdateData();
	m_fRatioX = atof(m_strXRatio);
	m_fRatioY = atof(m_strYRatio);
	
	CDialog::OnOK();
}
