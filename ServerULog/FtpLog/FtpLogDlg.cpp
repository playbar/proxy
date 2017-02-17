// FtpLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FtpLog.h"
#include "FtpLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpLogDlg dialog

CFtpLogDlg::CFtpLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpLogDlg)
	m_strLogPath = _T("D:\\Log.dat");
	m_strSavePath = _T("D:\\Log.txt");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFtpLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpLogDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strLogPath);
	DDX_Text(pDX, IDC_EDIT2, m_strSavePath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpLogDlg, CDialog)
	//{{AFX_MSG_MAP(CFtpLogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpLogDlg message handlers

BOOL CFtpLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFtpLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFtpLogDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFtpLogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL Log(LPCTSTR lpLogFilePath,LPCTSTR lpszFormat, ...)
{
	try
	{
		TCHAR	szText[1024*2]		= { 0 };
		va_list	ap;
		va_start(ap, lpszFormat);
		::_vsntprintf(szText, 1024*2, lpszFormat, ap);
		va_end(ap);	
		
		FILE	*pFile	= NULL;	
		pFile	= ::_tfopen(lpLogFilePath,  _T("a+b"));
		if ( pFile != NULL )
		{
			::_ftprintf(pFile, _T("%s\r\n"), szText);
			fclose(pFile);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

void CFtpLogDlg::OnOK() 
{
	UpdateData();
	HANDLE hFile	    =::CreateFile(m_strLogPath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwFileSize	= ::GetFileSize(hFile, NULL);
	DWORD dwRet			= 0;
	CHAR  *p			= new CHAR[dwFileSize+1];
	
	
	BOOL	bRet		= ::ReadFile(hFile,p, dwFileSize, &dwRet, NULL);
	if( !bRet || dwRet != dwFileSize )
	{
		return ;
	}
	m_gc.AddPtr(p);
	CDownFileInfo *pFileInfo	= NULL;
	m_DownFileInfo.RemoveAll();
	int index = 0;
	for ( index = 0; index<(int)(dwFileSize/sizeof(CDownFileInfo)); index++)
	{
		pFileInfo = (CDownFileInfo *)(p + index*sizeof(CDownFileInfo));
		m_DownFileInfo.Add(pFileInfo);
	}
	CloseHandle(hFile);
	
	for (index = 0; index<m_DownFileInfo.GetSize();index++)
	{
		CDownFileInfo *pFileInfo = m_DownFileInfo[index];
		
		CHAR buf[80];
		GetDateFormat(LOCALE_SYSTEM_DEFAULT,
			
			0,
			
			&(pFileInfo->m_st),
			
			"dd/MM/yyyy", //转换年月日
			
			buf,
			
			sizeof(buf));

		Log(m_strSavePath,"%s|\t%f|\t%s|\t%d|\t%s|",buf,pFileInfo->m_fDownSpeed,pFileInfo->m_szIp,pFileInfo->m_enuDownFileState,pFileInfo->m_szFileName);
	}

}

void CFtpLogDlg::OnButton1() 
{
	try
	{
		UpdateData();
		CFileDialog	dlg(TRUE, NULL, m_strLogPath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("*.dat|*.dat|*.*|*.*||"));
		if(IDOK == dlg.DoModal())
		{
			m_strLogPath	= dlg.GetPathName();
			UpdateData(FALSE);
		}		
		
	}
	catch (...)
	{
		
	}	
	
}

void CFtpLogDlg::OnButton2() 
{
	try
	{
		UpdateData();
		CFileDialog	dlg(TRUE, NULL, m_strSavePath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("*.txt|*.txt|*.*|*.*||"));
		if(IDOK == dlg.DoModal())
		{
			m_strSavePath	= dlg.GetPathName();
			UpdateData(FALSE);
	}		
		
	}
	catch (...)
	{
		
	}	
	
}
