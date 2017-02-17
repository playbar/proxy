// ImageBoard.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <dos.h>
#include <direct.h>
#include <WinVer.h>

#include "ImageBoard.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ImageBoardDoc.h"
#include "ImageBoardView.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageBoardApp

BEGIN_MESSAGE_MAP(CImageBoardApp, CWinApp)
	//{{AFX_MSG_MAP(CImageBoardApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDM_PASTEASFILE, OnPasteasfile)
	ON_UPDATE_COMMAND_UI(IDM_PASTEASFILE, OnUpdatePasteasfile)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageBoardApp construction

CImageBoardApp::CImageBoardApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImageBoardApp object

CImageBoardApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImageBoardApp initialization

BOOL CImageBoardApp::InitInstance()
{
	AfxEnableControlContainer();

	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("VC-IP"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_IMAGEBTYPE,
		RUNTIME_CLASS(CImageBoardDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageBoardView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTDLG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);		// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

	CString str;

	// fill memory info
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	// Total Physical mem
	str.Format("%lu KB", MemStat.dwTotalPhys / 1024L);
	SetDlgItemText(IDC_PHYSICAL_MEM, str);
	// Free Physical mem
	str.Format("%lu KB", MemStat.dwAvailPhys / 1024L);
	SetDlgItemText(IDC_AVAIL_MEM, str);
	// Free Virtual mem
	str.Format("%lu KB", MemStat.dwAvailVirtual / 1024L);
	SetDlgItemText(IDC_AVAIL_VIRTUAL_MEM, str);

	// fill CPU info
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	switch (SysInfo.dwProcessorType)
	{
	case PROCESSOR_INTEL_486:
			str = "Intel 80486";
			break;
	case PROCESSOR_INTEL_PENTIUM:
			str = "Intel Pentium";
			break;
	default:
			str = "";
			break;
	}
	SetDlgItemText(IDC_CPU, str);

	// fill OS info
	OSVERSIONINFO OsInfo;
	OsInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&OsInfo))
	{
		// platform
		switch (OsInfo.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_WINDOWS:
			str = "Windows 95";
			break;
		case VER_PLATFORM_WIN32_NT:
			str = "Windows NT";
			break;
		default:
			str = "";
			break;
		}
		SetDlgItemText(IDC_OS, str);

		// version and language
		char   szVer[512];
		BYTE   abData[1024];
		DWORD  handle;
		DWORD  dwSize;
		DWORD *lpBuffer;			// holds lang and codepage
		char lpszSysPath[_MAX_PATH];		// address of buffer for Windows directory	*/

		strcpy(szVer, "Unknown");

		// system path
		GetSystemDirectory(lpszSysPath, _MAX_PATH);
		strcat(lpszSysPath,"\\user.exe");

		dwSize = GetFileVersionInfoSize((LPTSTR)lpszSysPath, &handle);

		if (dwSize != 0)
			if (GetFileVersionInfo((LPTSTR)lpszSysPath, handle, dwSize, abData))
				if (VerQueryValue(abData, "\\VarFileInfo\\Translation", (VOID **)&lpBuffer, (UINT *)&dwSize))
  					VerLanguageName (LOWORD(*lpBuffer), szVer, 512);

		str.Format("%ld.%ld, %s", OsInfo.dwMajorVersion, 
							OsInfo.dwMinorVersion, szVer);
		SetDlgItemText(IDC_VERSION, str);
	}

	// fill disk information
	char szWindowsPath[_MAX_PATH];
	GetWindowsDirectory(szWindowsPath, _MAX_PATH);
	ULARGE_INTEGER FreeBytesAvailable, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	GetDiskFreeSpaceEx((LPSTR)szWindowsPath,
						&FreeBytesAvailable,
						&TotalNumberOfBytes,
						&TotalNumberOfFreeBytes);
	// total space
	str.Format("%lu KB", TotalNumberOfBytes.QuadPart / 1024L);
	SetDlgItemText(IDC_TOTAL_SPACE, str);
	// free space
	str.Format("%lu KB", TotalNumberOfFreeBytes.QuadPart / 1024L);
	SetDlgItemText(IDC_FREE_SPACE, str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CImageBoardApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CImageBoardApp message handlers

void CImageBoardApp::OnFileOpen() 
{
	CFileDialog FileDlg(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter);

	if (FileDlg.DoModal() == IDOK)
		OpenDocumentFile(FileDlg.m_ofn.lpstrFile);
}


BOOL CImageBoardApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}


void CImageBoardApp::OnPasteasfile() 
{
	OnFileNew();

	CMainFrame *pMainWnd = (CMainFrame *)m_pMainWnd;
	CChildFrame *pChildWnd = (CChildFrame *)pMainWnd->MDIGetActive();
	CImageBoardView *pView = (CImageBoardView *)pChildWnd->GetActiveView();
	CImageBoardDoc *pDoc = (CImageBoardDoc *)pChildWnd->GetActiveDocument();

	// get DIB handle from clipboard
	if (pChildWnd->OpenClipboard())
	{
		HDIB hNewDIB = NULL;
		HBITMAP hNewDDB = NULL;
		HPALETTE hPal = NULL;
		if (IsClipboardFormatAvailable(CF_DIB))
		{
			hNewDIB = CopyHandle(GetClipboardData(CF_DIB));
			if (hNewDIB != NULL)
			{
				pDoc->m_pDib->Attach(hNewDIB);
				CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
				pView->SetScrollSizes(MM_TEXT, sizeTotal);
				pDoc->SetModifiedFlag(TRUE);
				pDoc->UpdateAllViews(NULL);
			}
		}
		else if (IsClipboardFormatAvailable(CF_BITMAP))
		{
			hNewDDB = (HBITMAP)GetClipboardData(CF_BITMAP);
			if (IsClipboardFormatAvailable(CF_PALETTE))
				hPal = (HPALETTE)GetClipboardData(CF_PALETTE);
			if (hNewDDB != NULL)
			{
				pDoc->m_pDib->Create(hNewDDB, hPal);
				CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
				pView->SetScrollSizes(MM_TEXT, sizeTotal);
				pDoc->SetModifiedFlag(TRUE);
				pDoc->UpdateAllViews(NULL);
			}
		}

		CloseClipboard();
	}
}

void CImageBoardApp::OnUpdatePasteasfile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB) ||
				   ::IsClipboardFormatAvailable(CF_BITMAP));
}

void CImageBoardApp::AddFile(HDIB hDIB, LPCTSTR lpszTitle) 
{
	OnFileNew();

	CMainFrame *pMainWnd = (CMainFrame *)m_pMainWnd;
	CChildFrame *pChildWnd = (CChildFrame *)pMainWnd->MDIGetActive();
	CImageBoardView *pView = (CImageBoardView *)pChildWnd->GetActiveView();
	CImageBoardDoc *pDoc = (CImageBoardDoc *)pChildWnd->GetActiveDocument();

	if (hDIB != NULL)
	{
		pDoc->m_pDib->Attach(hDIB);
		CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
		pView->SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(lpszTitle);
		pDoc->UpdateAllViews(NULL);
	}
}