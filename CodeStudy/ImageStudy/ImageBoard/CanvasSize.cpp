// CanvasSize.cpp : implementation file
//

#include "stdafx.h"
#include "ImageBoard.h"
#include "CanvasSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanvasSize dialog


CCanvasSize::CCanvasSize(int nWidth, int nHeight, CWnd* pParent /*=NULL*/)
	: CDialog(CCanvasSize::IDD, pParent)
{
	CString strH, strW;
	strH.Format("%d", nHeight);
	strW.Format("%d", nWidth);
	//{{AFX_DATA_INIT(CCanvasSize)
	m_bProportional = TRUE;
	m_strHeight = strH;
	m_strWidth = strW;
	//}}AFX_DATA_INIT
	m_fRatio = (double)nHeight/(double)nWidth;
	m_nUnit	 = 2;
	m_nPosition = 5;		//Center;
	m_nWidth  = nWidth;
	m_nHeight = nHeight;
	m_nWidth0  = m_nWidth;
	m_nHeight0 = m_nHeight;
}


void CCanvasSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCanvasSize)
	DDX_Control(pDX, IDC_UR, m_btUR);
	DDX_Control(pDX, IDC_UP, m_btUp);
	DDX_Control(pDX, IDC_UL, m_btUL);
	DDX_Control(pDX, IDC_RIGHT, m_btRight);
	DDX_Control(pDX, IDC_LEFT, m_btLeft);
	DDX_Control(pDX, IDC_DR, m_btDR);
	DDX_Control(pDX, IDC_DOWN, m_btDown);
	DDX_Control(pDX, IDC_DL, m_btDL);
	DDX_Control(pDX, IDC_CENTER, m_btCenter);
	DDX_Check(pDX, IDC_PROPORTIONAL, m_bProportional);
	DDX_Text(pDX, IDC_HEIGHT, m_strHeight);
	DDX_Text(pDX, IDC_WIDTH, m_strWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanvasSize, CDialog)
	//{{AFX_MSG_MAP(CCanvasSize)
	ON_BN_CLICKED(IDC_CENTER, OnCenter)
	ON_BN_CLICKED(IDC_CM, OnCm)
	ON_BN_CLICKED(IDC_DL, OnDl)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_DR, OnDr)
	ON_EN_CHANGE(IDC_HEIGHT, OnChangeHeight)
	ON_BN_CLICKED(IDC_INCH, OnInch)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_BN_CLICKED(IDC_PIXEL, OnPixel)
	ON_BN_CLICKED(IDC_PROPORTIONAL, OnProportional)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_BN_CLICKED(IDC_UL, OnUl)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_UR, OnUr)
	ON_EN_CHANGE(IDC_WIDTH, OnChangeWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvasSize message handlers

BOOL CCanvasSize::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CClientDC dc(this);
	m_nPixelsPInchX = dc.GetDeviceCaps(LOGPIXELSX);
	m_nPixelsPInchY = dc.GetDeviceCaps(LOGPIXELSY);

	UpdateData(FALSE);

	// initialize position picture
	VERIFY(m_btUL.LoadBitmaps((m_nPosition==1 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btUL.Invalidate();
	VERIFY(m_btUp.LoadBitmaps((m_nPosition==2 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btUp.Invalidate();
	VERIFY(m_btUR.LoadBitmaps((m_nPosition==3 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btUR.Invalidate();
	VERIFY(m_btLeft.LoadBitmaps((m_nPosition==4 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btLeft.Invalidate();
	VERIFY(m_btCenter.LoadBitmaps((m_nPosition==5 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btCenter.Invalidate();
	VERIFY(m_btRight.LoadBitmaps((m_nPosition==6 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btRight.Invalidate();
	VERIFY(m_btDL.LoadBitmaps((m_nPosition==7 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btDL.Invalidate();
	VERIFY(m_btDown.LoadBitmaps((m_nPosition==8 )? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btDown.Invalidate();
	VERIFY(m_btDR.LoadBitmaps((m_nPosition==9)? IDB_PHOTO:IDB_BLANK, NULL, NULL));
	m_btDR.Invalidate();
	
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCanvasSize::OnCm() 
{
	m_nUnit = 0;
	
	m_strWidth.Format("%.2f", ((double)m_nWidth/(double)m_nPixelsPInchX)*2.54);
	m_strHeight.Format("%.2f", ((double)m_nHeight/(double)m_nPixelsPInchY)*2.54);
	UpdateData(FALSE);
}

void CCanvasSize::OnInch() 
{
	m_nUnit = 1;
	
	m_strWidth.Format("%.2f", (double)m_nWidth/(double)m_nPixelsPInchX);
	m_strHeight.Format("%.2f", (double)m_nHeight/(double)m_nPixelsPInchY);
	UpdateData(FALSE);
}

void CCanvasSize::OnPixel() 
{
	m_nUnit = 2;
	
	m_strWidth.Format("%d", m_nWidth);
	m_strHeight.Format("%d", m_nHeight);
	UpdateData(FALSE);
}


void CCanvasSize::OnCenter() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 5;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnDl() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 7;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnDown() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 8;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnDr() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 9;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnLeft() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 4;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnRight() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 6;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnUl() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 1;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnUp() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 2;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnUr() 
{
	int nOldPosition = m_nPosition;
	m_nPosition = 3;
	SetPositionButtonsImage(nOldPosition);
}

void CCanvasSize::OnProportional() 
{
	UpdateData();
	if (m_bProportional)
	{
		m_nHeight = (int)(m_nWidth*m_fRatio);
		UpdateData(FALSE);
	}
}

void CCanvasSize::OnReset() 
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

void CCanvasSize::OnChangeHeight() 
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

void CCanvasSize::OnChangeWidth() 
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

void CCanvasSize::SetPositionButtonsImage(int nOldPosition)
{
	if (nOldPosition == m_nPosition)
		return;

	switch (nOldPosition)
	{
		case 1:
				VERIFY(m_btUL.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btUL.Invalidate();
				break;
		case 2:
				VERIFY(m_btUp.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btUp.Invalidate();
				break;
		case 3:
				VERIFY(m_btUR.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btUR.Invalidate();
				break;
		case 4:
				VERIFY(m_btLeft.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btLeft.Invalidate();
				break;
		case 5:
				VERIFY(m_btCenter.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btCenter.Invalidate();
				break;
		case 6:
				VERIFY(m_btRight.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btRight.Invalidate();
				break;
		case 7:
				VERIFY(m_btDL.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btDL.Invalidate();
				break;
		case 8:
				VERIFY(m_btDown.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btDown.Invalidate();
				break;
		case 9:
				VERIFY(m_btDR.LoadBitmaps(IDB_BLANK, NULL, NULL));
				m_btDR.Invalidate();
				break;
	}

	switch (m_nPosition)
	{
		case 1:
				VERIFY(m_btUL.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btUL.Invalidate();
				break;
		case 2:
				VERIFY(m_btUp.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btUp.Invalidate();
				break;
		case 3:
				VERIFY(m_btUR.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btUR.Invalidate();
				break;
		case 4:
				VERIFY(m_btLeft.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btLeft.Invalidate();
				break;
		case 5:
				VERIFY(m_btCenter.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btCenter.Invalidate();
				break;
		case 6:
				VERIFY(m_btRight.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btRight.Invalidate();
				break;
		case 7:
				VERIFY(m_btDL.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btDL.Invalidate();
				break;
		case 8:
				VERIFY(m_btDown.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btDown.Invalidate();
				break;
		case 9:
				VERIFY(m_btDR.LoadBitmaps(IDB_PHOTO, NULL, NULL));
				m_btDR.Invalidate();
				break;
	}
}
