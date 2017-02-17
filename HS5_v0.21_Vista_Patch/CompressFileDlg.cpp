// CompressFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CompressFile.h"
#include "CompressFileDlg.h"
#include "Compression.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL UseCutstomResource(LPCSTR lpID,LPCSTR lpType, LPCSTR lpstrFile);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCompressFileDlg dialog




CCompressFileDlg::CCompressFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompressFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCompressFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_COMPRESS, m_btnPatch);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_STC_INFO, m_StcInfo);
	DDX_Control(pDX, IDC_STC_PATCH,m_StcPatch );
}

BEGIN_MESSAGE_MAP(CCompressFileDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_COMPRESS, &CCompressFileDlg::OnBnClickedBtnCompress)
	ON_BN_CLICKED(IDOK, &CCompressFileDlg::OnBnClickedOk)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CCompressFileDlg message handlers


BOOL CCompressFileDlg::OnInitDialog()
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
	int   screenX   =   GetSystemMetrics(SM_CXFULLSCREEN);   
	int   screenY   =   GetSystemMetrics(SM_CYFULLSCREEN);   

	m_bmpBackground.LoadBitmap(IDB_BACK_SURFACE);
	BITMAP      stbmp;
	m_bmpBackground.GetBitmap(&stbmp);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = stbmp.bmWidth;
	rect.bottom = stbmp.bmHeight;
	MoveWindow( &rect, true);
	CenterWindow();

	((CMacrosButton *) GetDlgItem(IDC_BTN_COMPRESS))->MoveWindow(258,240,175,26);
	((CMacrosButton *) GetDlgItem(IDOK))->MoveWindow(258,285,175,26);
	((CStatic*)GetDlgItem(IDC_STC_PATCH))->MoveWindow(235,70,217,115);
	((CStatic*)GetDlgItem(IDC_STC_INFO))->MoveWindow(255,88,165,90);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCompressFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCompressFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		//static bool b = true;
		//CDialog::OnPaint();
		CPaintDC dc(this); 
		CDC	bgdc;
		bgdc.CreateCompatibleDC(NULL);

		CRect   rect;
		GetClientRect(rect);
		ClientToScreen(&rect);

		int  Height,Width;
		Height = rect.Height();
		Width = rect.Width();

		/*if (b)
		{
			char chOutput[256];
			sprintf(chOutput,"Height = %d, Width = %d",Height, Width);
			MessageBox(chOutput);
			b = false;
		}*/
		
		BITMAP      stbmp;
		m_bmpBackground.GetBitmap(&stbmp);
		bgdc.SelectObject(m_bmpBackground);
		dc.BitBlt(0,0, Width, Height,&bgdc, 0, 0, SRCCOPY);
		//dc.StretchBlt(0,0, Width, Height,&bgdc, 0, 0,stbmp.bmWidth, stbmp.bmHeight, SRCCOPY);
		/*for (int nX = 0; nX < Width; nX += stbmp.bmWidth)
			for (int nY = 0; nY < Height; nY += stbmp.bmHeight)
				dc.BitBlt(nX,nY,stbmp.bmWidth,stbmp.bmHeight,&bgdc,0,0,SRCCOPY);*/
	
		
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCompressFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCompressFileDlg::OnBnClickedBtnCompress()
{
	char sInPath[MAX_PATH];
	char sOutPath[MAX_PATH];
	char strDir[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), sOutPath, sizeof(sOutPath));
	char *  lpTmp= strrchr(sOutPath, '\\');
	if (lpTmp != NULL )
	{
		*(lpTmp +1) = 0;
	}
	strcpy(sInPath, sOutPath);
	strcat(sInPath,"Patch.emo");
	strcpy(strDir, sOutPath);
	strcat(sOutPath,"5street\\");
	strcat(strDir, "5street.exe");

	FILE * pf ;
	if (pf = fopen(strDir, "r"))
	{
		char  szDll[256];
		char  szIaf[256];
		strcpy(szDll,sOutPath);
		strcat(szDll, "d3d8.DLL");
		strcpy(szIaf, sOutPath);
		strcat(szIaf, "emmoaz.iaf");
		if (UseCutstomResource(MAKEINTRESOURCE(IDR_DLLD3D8), TEXT("DLL"), szDll) &&
			UseCutstomResource(MAKEINTRESOURCE(IDR_IAFEMMOAZ), TEXT("IAF"), szIaf))
		{
			MessageBox(" Restoration succeeded");
		}
		else
		{
			MessageBox("Restoration failed,Try again");
		}

		/*KUnzip zip;
		if (zip.Unzip(sInPath,sOutPath))
		{
			MessageBox(" Restoration succeeded");
		}
		else
		{
			MessageBox("Restoration failed,Try again");
		}*/
		fclose(pf);
	}
	else
	{
		::CoInitialize(NULL);   
		while(true)
		{
			MessageBox("Please select HighStreet 5 game menu path");	
			LPITEMIDLIST   pidlPath;   
			char   pszDplName[MAX_PATH];   
			BROWSEINFO   biFile;   
			IMalloc   *pDllMlc;   
			biFile.hwndOwner   =   m_hWnd;   
			biFile.pidlRoot   =   NULL;   
			biFile.pszDisplayName   =   NULL;   
			biFile.lpszTitle   =   "Select HighStreet 5 game menu path";   
			biFile.ulFlags   =   BIF_RETURNFSANCESTORS;   
			biFile.lpfn   =   NULL;   
			biFile.lParam   =   NULL;   
			biFile.iImage   =   0;   
			if(SUCCEEDED(SHGetMalloc(&pDllMlc)))   
			{   
				pidlPath=SHBrowseForFolder(&biFile);   
				if(pidlPath   !=   NULL)
				{
					SHGetPathFromIDList(pidlPath,pszDplName);//pszDplName:文件的路径地址  
				}
				else 
				{
					pDllMlc->Free(NULL);   
					pDllMlc->Release();   
					break;  
				}
				pDllMlc->Free(NULL);   
				pDllMlc->Release();   
			}   
			//strcpy(sInPath, pszDplName);
			//strcat(sInPath,"\\Patch.emo");

			strcpy(sOutPath, pszDplName);
			strcat(sOutPath,"\\5street\\");

			strcpy(strDir, pszDplName);
			strcat(strDir, "\\5street.exe");
			if (pf = fopen(strDir, "r"))
			{
				char  szDll[256];
				char  szIaf[256];
				strcpy(szDll,sOutPath);
				strcat(szDll, "d3d8.DLL");
				strcpy(szIaf, sOutPath);
				strcat(szIaf, "emmoaz.iaf");
				if (UseCutstomResource(MAKEINTRESOURCE(IDR_DLLD3D8), TEXT("DLL"), szDll) &&
					UseCutstomResource(MAKEINTRESOURCE(IDR_IAFEMMOAZ), TEXT("IAF"), szIaf))
				{
					MessageBox(" Restoration succeeded");
				}
				else
				{
					MessageBox("Restoration failed,Try again");
				}
				/*KUnzip zip;
				if (zip.Unzip(sInPath,sOutPath))
				{
					MessageBox(" Restoration succeeded");
				}
				else
				{
					MessageBox("Restoration failed,Try again");
				}*/
				fclose(pf);
				break;
			}
		}
		::CoUninitialize(); 
	} 
}

void CCompressFileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

LRESULT CCompressFileDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
	//return CDialog::OnNcHitTest(point);
}


BOOL UseCutstomResource(LPCSTR lpID,LPCSTR lpType, LPCSTR lpstrFile)
{
	//HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_DLLUNZIP32), TEXT("DLL"));
	HRSRC hRsrc = FindResource(NULL, lpID, lpType);
	if (NULL == hRsrc)
	{
		return false;
	}

	DWORD dwSize = SizeofResource( NULL, hRsrc);
	if ( 0 == dwSize)
	{
		return false;
	}
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc );
	if ( NULL == hGlobal )
	{
		return false;
	}
	LPCVOID lpBuffer = LockResource(hGlobal);
	if (NULL == lpBuffer )
	{
		return FALSE;
	}
	BOOL bRt = FALSE;

	TCHAR szCurDir[MAX_PATH] = {0};
	FILE * fp = fopen(lpstrFile,"wb");
	if (fp != NULL)
	{
		if ( dwSize == fwrite(lpBuffer, sizeof(char), dwSize, fp))
		{
			bRt = TRUE;
		}
		fclose(fp);
	}
	return bRt;
}
