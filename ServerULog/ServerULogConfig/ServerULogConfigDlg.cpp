// ServerULogConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerULogConfig.h"
#include "ServerULogConfigDlg.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define LOGFILE_INI				"ServerU\\LogFile.ini"
#define SERVERULOGCONFIG_INI	"ServerULogConfig.ini"



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerULogConfigDlg 对话框




CServerULogConfigDlg::CServerULogConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerULogConfigDlg::IDD, pParent)
	, m_strServerUPath(_T(""))
	, m_strLogPath(_T(""))
	, m_strFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerULogConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVERUPATH, m_strServerUPath);
	DDX_Text(pDX, IDC_EDIT_LOGPATH, m_strLogPath);
	DDX_Control(pDX, IDC_LIST_FILTERFILE, m_lsctrlFilterFile);
	DDX_Control(pDX, IDC_LIST_FILEPATH, m_lsctrlFilePath);
}

BEGIN_MESSAGE_MAP(CServerULogConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SERVERUPATH, &CServerULogConfigDlg::OnBnClickedBtnServerupath)
	ON_BN_CLICKED(IDC_BTN_LOGPATH, &CServerULogConfigDlg::OnBnClickedBtnLogpath)
	ON_BN_CLICKED(IDC_BTN_FILEPATH, &CServerULogConfigDlg::OnBnClickedBtnFilepath)
	ON_BN_CLICKED(IDC_BTN_ADDFILTERFILE, &CServerULogConfigDlg::OnBnClickedBtnAddfilterfile)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILTERFILE, &CServerULogConfigDlg::OnNMDblclkListFilterfile)
	
	ON_BN_CLICKED(IDC_BTN_SAVADATA, &CServerULogConfigDlg::OnBnClickedBtnSavadata)
	ON_BN_CLICKED(IDOK, &CServerULogConfigDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILEPATH, &CServerULogConfigDlg::OnNMDblclkListFilepath)
END_MESSAGE_MAP()


// CServerULogConfigDlg 消息处理程序

BOOL CServerULogConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitData();

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerULogConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerULogConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerULogConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerULogConfigDlg::OnBnClickedBtnServerupath()
{
	CString strServerUPath;
	OpenFilePath(strServerUPath);
	if ( strServerUPath.GetLength() > 0 )
	{
		m_strServerUPath = strServerUPath;
	}
	UpdateData( false );
}

void CServerULogConfigDlg::OpenFilePath(CString &strFilePath)
{
	::CoInitialize(NULL);   
	LPITEMIDLIST   pidlPath;   
	char   pszDplName[MAX_PATH];   
	memset( pszDplName, 0, MAX_PATH );
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
		}
		pDllMlc->Free(NULL);   
		pDllMlc->Release();   
	}   
	::CoUninitialize();  
	strFilePath = pszDplName;
}

void CServerULogConfigDlg::OnBnClickedBtnLogpath()
{
	CString strLogPath;
	OpenFilePath( strLogPath );
	if ( strLogPath.GetLength() > 0)
	{
		m_strLogPath = strLogPath;
	}
	UpdateData( false );
}

void CServerULogConfigDlg::OnBnClickedBtnFilepath()
{
	OpenFilePath( m_strFilePath );

	if ( m_strFilePath.GetLength() == 0 )
	{
		return;
	}

	int ilcctrl = m_lsctrlFilePath.GetItemCount();
	ilcctrl ++;
	CString strNum ;
	strNum.Format("%d", ilcctrl);
	int nItemInserted = m_lsctrlFilePath.InsertItem(LVIF_TEXT, ilcctrl, strNum, 0, 0, 0, 0);
	m_lsctrlFilePath.SetItem(nItemInserted, 1, LVIF_TEXT, m_strFilePath, 0, 0, 0, 0);

	UpdateData( false );
}


void CServerULogConfigDlg::InitData()
{
	GetModuleFileName(NULL, m_strModulePath.GetBuffer(MAX_PATH),MAX_PATH);
	m_strModulePath.ReleaseBuffer();
	m_strModulePath = m_strModulePath.Left(m_strModulePath.ReverseFind('\\')+1);
	if(m_strModulePath.Right(1)!='\\')
		m_strModulePath+='\\';

	CString strIniFile;
	strIniFile = m_strModulePath + SERVERULOGCONFIG_INI;
	GetPrivateProfileString("ServerUHome",
		"HomeDir",
		"",
		m_strServerUPath.GetBuffer(MAX_PATH),
		MAX_PATH,(LPCSTR)strIniFile);
	m_strServerUPath.ReleaseBuffer();
	//////////////////////////////////////////////////////////////////////////

	strIniFile  = m_strModulePath + LOGFILE_INI;
	
	GetPrivateProfileString("LogFile",
		"FilePath",
		"",
		m_strLogPath.GetBuffer(MAX_PATH),
		MAX_PATH,(LPCSTR)strIniFile);
	m_strLogPath.ReleaseBuffer();

//////////////////////////////////////////////////////////////////////////
	int iFileConnt=1;
	int nColCount = 0;
	CString strTmp;
	char szIndexKey[16];
	m_lsctrlFilePath.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );
	m_lsctrlFilePath.InsertColumn(nColCount++, _T("Num"), LVCFMT_LEFT, 60);
	m_lsctrlFilePath.InsertColumn(nColCount++, _T("File Path Name"), LVCFMT_LEFT, 260);

	while(TRUE)
	{
		wsprintf(szIndexKey,"%d",iFileConnt);
		GetPrivateProfileString("FilterPath",
			szIndexKey,
			"",//默认值为空
			strTmp.GetBuffer(MAX_PATH),	MAX_PATH,
			(LPCSTR)strIniFile);
		strTmp.ReleaseBuffer();
		if(strTmp.IsEmpty())
			break;
		//	m_strFileArray.Add(strTmp);
		int nItemInserted = m_lsctrlFilePath.InsertItem(LVIF_TEXT, iFileConnt, szIndexKey, 0, 0, 0, 0);
		m_lsctrlFilePath.SetItem(nItemInserted, 1, LVIF_TEXT, strTmp, 0, 0, 0, 0);
		iFileConnt++;
	}

	//////////////////////////////////////////////////////////////////////////
	 nColCount = 0;
	 iFileConnt=1;
	m_lsctrlFilterFile.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );
	m_lsctrlFilterFile.InsertColumn(nColCount++, _T("Num"), LVCFMT_LEFT, 60);
	m_lsctrlFilterFile.InsertColumn(nColCount++, _T("File Name"), LVCFMT_LEFT, 260);

	//m_strFileArray.RemoveAll();
	while(TRUE)
	{
		wsprintf(szIndexKey,"%d",iFileConnt);
		GetPrivateProfileString("FilterFile",
			szIndexKey,
			"",//默认值为空
			strTmp.GetBuffer(MAX_PATH),	MAX_PATH,
			(LPCSTR)strIniFile);
		strTmp.ReleaseBuffer();
		if(strTmp.IsEmpty())
			break;
	//	m_strFileArray.Add(strTmp);
		int nItemInserted = m_lsctrlFilterFile.InsertItem(LVIF_TEXT, iFileConnt, szIndexKey, 0, 0, 0, 0);
		m_lsctrlFilterFile.SetItem(nItemInserted, 1, LVIF_TEXT, strTmp, 0, 0, 0, 0);
		iFileConnt++;
	}

	UpdateData( false );

}

void CServerULogConfigDlg::OnBnClickedBtnAddfilterfile()
{
	CFileDialog dlgFile(TRUE);
	CString strFileName;
	const int c_cMaxFiles = 100;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;

	dlgFile.GetOFN().lpstrFile = strFileName.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cMaxFiles;
	dlgFile.DoModal();
	strFileName.ReleaseBuffer();
	if ( strFileName.GetLength() == 0 )
	{
		return ;
	}

	int ilcctrl = m_lsctrlFilterFile.GetItemCount();
	ilcctrl ++;

	CString strNum ;
	strNum.Format("%d", ilcctrl);
	int nItemInserted = m_lsctrlFilterFile.InsertItem(LVIF_TEXT, ilcctrl, strNum, 0, 0, 0, 0);
	m_lsctrlFilterFile.SetItem(nItemInserted, 1, LVIF_TEXT, strFileName, 0, 0, 0, 0);

	UpdateData( false );
 
}

void CServerULogConfigDlg::OnNMDblclkListFilterfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	

	int iselect = -1;
	if(m_lsctrlFilterFile.GetSelectedCount()<=0)
		return ;
	for(int i=0;i<m_lsctrlFilterFile.GetItemCount();i++)
	{
		if(m_lsctrlFilterFile.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{ 
			iselect = i;
			break;
		}
	}

	m_lsctrlFilterFile.DeleteItem( iselect );

	for ( int i =0; i < m_lsctrlFilterFile.GetItemCount(); i ++ )
	{
		CString strNum ;
		strNum.Format("%d", i + 1);
		m_lsctrlFilterFile.SetItem( i , 0 , LVIF_TEXT, strNum, 0, 0, 0, 0);
	}
	
	UpdateData( false );
}

bool CServerULogConfigDlg::Savadata()
{

	CString strServerUDaemon;
	CString strServerULogDll;
	strServerUDaemon = m_strServerUPath + "\\ServUDaemon.ini";
	CFileFind   find;   
	if (!find.FindFile(strServerUDaemon) )  
	{
		MessageBox("serverU Path Incorrect，Please choose the correct path");
		find.Close();
		return false;
	}
	find.Close();

	strServerULogDll = m_strModulePath + "ServerU\\ServerULog.dll";
	WritePrivateProfileString("EXTERNAL",
		"EventHookDLL1",
		strServerULogDll.GetBuffer(MAX_PATH),
		(LPCSTR)strServerUDaemon);
	strServerULogDll.ReleaseBuffer();


	//////////////////////////////////////////////////////////////////////////

	CString strIniFile;
	strIniFile = m_strModulePath + SERVERULOGCONFIG_INI;
	WritePrivateProfileString("ServerUHome",
		"HomeDir",
		m_strServerUPath.GetBuffer(MAX_PATH),
		(LPCSTR)strIniFile);
	m_strServerUPath.ReleaseBuffer();
	//////////////////////////////////////////////////////////////////////////

	if( ! PathFileExists(m_strLogPath.GetBuffer()))
	{
		int iRe = MessageBox("Path does not exist, Do you want Create ?" ,"Tips", MB_YESNOCANCEL |MB_DEFBUTTON1 |MB_ICONWARNING );
		if ( iRe == IDYES )
		{
			SHCreateDirectoryEx(NULL, m_strLogPath, NULL);
		}
		else
		{
			MessageBox("Path does not exist");
			return false;
		}

	}

	strIniFile  = m_strModulePath + LOGFILE_INI;

	DeleteFile ( strIniFile );

	WritePrivateProfileString("LogFile",
		"FilePath",
		m_strLogPath.GetBuffer(MAX_PATH),
		(LPCSTR)strIniFile);
	m_strLogPath.ReleaseBuffer();

	//////////////////////////////////////////////////////////////////////////
	for ( int i =0; i < m_lsctrlFilePath.GetItemCount(); i ++ )
	{
		CString strNum;
		strNum.Format( "%d", i + 1);
		CString strFileName ;
		strFileName = m_lsctrlFilePath.GetItemText( i , 1);

		WritePrivateProfileString("FilterPath",
			strNum,
			strFileName.GetBuffer(MAX_PATH),
			(LPCSTR)strIniFile);
		strFileName.ReleaseBuffer();

	}

	//////////////////////////////////////////////////////////////////////////
	for ( int i =0; i < m_lsctrlFilterFile.GetItemCount(); i ++ )
	{
		CString strNum;
		strNum.Format( "%d", i + 1);
		CString strFileName ;
		strFileName = m_lsctrlFilterFile.GetItemText( i , 1);

		WritePrivateProfileString("FilterFile",
			strNum,
			strFileName.GetBuffer(MAX_PATH),
			(LPCSTR)strIniFile);
		strFileName.ReleaseBuffer();

	}
	MessageBox("Save Success!");
	return true;
}


void CServerULogConfigDlg::OnBnClickedBtnSavadata()
{
	UpdateData( true );
	Savadata();

}

void CServerULogConfigDlg::OnBnClickedOk()
{
	
	int iRe = MessageBox("Save Changes ?" ,"Tips", MB_YESNOCANCEL  |MB_DEFBUTTON1 |MB_ICONWARNING );
	if ( iRe == IDYES )
	{
		if ( ! Savadata() )
		{
			return;
		}
	}
	if ( iRe == IDCANCEL )
	{
		return;
	}
	OnOK();

}

void CServerULogConfigDlg::OnClose()
{
	int iRe = MessageBox("Save Changes ?" ,"Tips", MB_YESNOCANCEL |MB_DEFBUTTON1 |MB_ICONWARNING );
	if ( iRe == IDYES )
	{
		if ( ! Savadata() )
		{
			return;
		}

	}
	if ( iRe == IDCANCEL )
	{
		return;
	}

	CDialog::OnClose();
}

void CServerULogConfigDlg::OnNMDblclkListFilepath(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	int iselect = -1;
	if(m_lsctrlFilePath.GetSelectedCount()<=0)
		return ;
	for(int i=0;i<m_lsctrlFilePath.GetItemCount();i++)
	{
		if(m_lsctrlFilePath.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{ 
			iselect = i;
			break;
		}
	}

	m_lsctrlFilePath.DeleteItem( iselect );

	for ( int i =0; i < m_lsctrlFilePath.GetItemCount(); i ++ )
	{
		CString strNum ;
		strNum.Format("%d", i + 1);
		m_lsctrlFilePath.SetItem( i , 0 , LVIF_TEXT, strNum, 0, 0, 0, 0);
	}

	UpdateData( false );

}
