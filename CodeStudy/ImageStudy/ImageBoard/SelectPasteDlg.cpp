// SelectPasteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "SelectPasteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectPasteDlg dialog


CSelectPasteDlg::CSelectPasteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPasteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectPasteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCF = -1;
}


void CSelectPasteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectPasteDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectPasteDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectPasteDlg)
	ON_BN_CLICKED(IDC_RADIO_DIB, OnRadioDib)
	ON_BN_CLICKED(IDC_RADIO_BITMAP, OnRadioBitmap)
	ON_BN_CLICKED(IDC_RADIO_PALETTE, OnRadioPalette)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectPasteDlg message handlers

BOOL CSelectPasteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BOOL bDibAvaible = ::IsClipboardFormatAvailable(CF_DIB); 
	BOOL bBitmapAvaible = ::IsClipboardFormatAvailable(CF_BITMAP);
	BOOL bPaletteAvaible = ::IsClipboardFormatAvailable(CF_PALETTE);
	GetDlgItem(IDC_RADIO_DIB)->EnableWindow(bDibAvaible);
	GetDlgItem(IDC_RADIO_BITMAP)->EnableWindow(bBitmapAvaible);
	GetDlgItem(IDC_RADIO_PALETTE)->EnableWindow(bPaletteAvaible);
	if (bDibAvaible)
	{
		((CButton* )GetDlgItem(IDC_RADIO_DIB))->SetCheck(1);
		m_nCF = CF_DIB;
	}
	else if (bBitmapAvaible)
	{
		((CButton* )GetDlgItem(IDC_RADIO_BITMAP))->SetCheck(1);
		m_nCF = CF_BITMAP;
	}
	else if (bPaletteAvaible)
	{
		((CButton* )GetDlgItem(IDC_RADIO_PALETTE))->SetCheck(1);
		m_nCF = CF_PALETTE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectPasteDlg::OnRadioDib() 
{
	m_nCF = CF_DIB;
}

void CSelectPasteDlg::OnRadioBitmap() 
{
	m_nCF = CF_BITMAP;
}

void CSelectPasteDlg::OnRadioPalette() 
{
	m_nCF = CF_PALETTE;
}
